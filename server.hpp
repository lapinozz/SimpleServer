#include <sstream>
#include <string>
#include <map>
#include <chrono>
#include <vector>
#include <functional>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "statusCodes.hpp"
#include "methods.hpp"

#include "utility.hpp"
#include "portableSocket.hpp"

struct HttpRequest
{
    Method method;
    std::string uri;

    std::string path;
    std::map<std::string, std::string> params;

    std::map<std::string, std::string> headers;

    int32_t httpVerMinor = 1;
    int32_t httpVerMajor = 1;

    Status status = S_200;

    std::string body;
};

class HttpParser
{
public:
    enum StateStatus
    {
        READING,
        CONTINUE,
        OK,
        FAIL,
    };

    void begin(bool parsePath = true)
    {
        httpRequest = HttpRequest();
        receiveDone = false;
        lastStatus = OK;
        pos = 0;

        states.clear();
        request.clear();

        if(parsePath)
            pushState(&HttpParser::parsePath);

        pushState(&HttpParser::parseMethod);
    }

    StateStatus read(const char* data, size_t size)
    {
        pos = request.size();
        request.append(data, data + size);

        while(true)
        {
            if(states.empty())
                return lastStatus;

            auto it = states.size() - 1;
            lastStatus = states.back()(this);

            if(lastStatus != READING && lastStatus != CONTINUE)
            {
                states.erase(states.begin() + it);
                pos = 0;
            }

            if(lastStatus == READING || (!receiveDone && request.empty()))
                return READING;

            continue;
        }
    }

    HttpRequest receiveRequest(SOCKET s, bool parsePath = true)
    {
        begin(parsePath);

        while(true)
        {
            char buffer[512];
            auto size = recv(s, buffer, sizeof(buffer), 0);

            assert(size != SOCKET_ERROR);

            if(size == 0)
            {
                sockClose(s);
                break;
            }

            auto status = read(buffer, size);
            if(status != READING)
                break;
        }

        return httpRequest;
    }

private:

    using State = std::function<StateStatus(HttpParser*)>;
    using StateStack = std::vector<State>;

    StateStack states;
    StateStatus lastStatus;

    HttpRequest httpRequest;

    constexpr static auto SPACES = " \t";

    std::string request;
    size_t pos = 0;

    bool receiveDone = false;

    void pushState(const State& state)
    {
        states.push_back(state);
    }

    template <typename...Args>
    void pushState(const State& state, Args&&...args)
    {
        pushState(args...);
        pushState(state);
    }

    StateStatus parsePath()
    {
        pos = httpRequest.uri.find("?");
        httpRequest.path = httpRequest.uri.substr(0, pos);

        std::string params;
        if(pos != std::string::npos)
            params = httpRequest.uri.substr(pos + 1);

        auto it = httpRequest.headers.find("content-type");
        if(it != httpRequest.headers.end() && it->second == "application/x-www-form-urlencoded")
            params += "&" + httpRequest.body;

        if(!params.empty())
        {
            for(const auto& param : split(params, "&", true))
            {
                pos = param.find("=");

                if(pos == 0 || param.empty())
                    continue;

                std::string key = param.substr(0, pos);
                std::string value = "";
                if(pos != std::string::npos)
                    value = param.substr(pos + 1);

                httpRequest.params[key] = value;
            }
        }

        return OK;
    }

    StateStatus removeLeadingSpaces()
    {
        pos = request.find_last_of(SPACES, pos);

        if(pos != std::string::npos)
        {
            if(pos == request.size() - 1)
                return READING;

            request = request.substr(pos + 1);
            pos = 0;

            return OK;
        }
        else
        {
            pos = 0;
            return FAIL;
        }
    }

    StateStatus requireOneSpaceOrMore()
    {
        pushState(&HttpParser::removeLeadingSpaces, &HttpParser::assertStatusIsOk);
        return OK;
    }

    StateStatus waitEnfOfLine()
    {
        pos = request.find('\n', pos);
        if(pos == std::string::npos)
            return READING;
        else
            return OK;
    }

    StateStatus removeEndOfLine()
    {
        if(request[0] == '\n')
        {
            request = request.substr(1);
            return OK;
        }
        else if(request.size() == 1 && request[0] == '\r')
            return READING;
        else if(request.substr(0, 2) == "\r\n")
        {
            request = request.substr(2);
            return OK;
        }

        return FAIL;
    }

    StateStatus requireEndOfLine()
    {
        pushState(&HttpParser::removeEndOfLine, &HttpParser::assertStatusIsOk);
        return OK;
    }

    StateStatus assertStatusIsOk()
    {
        assert(lastStatus == OK);
        return OK;
    }

    StateStatus parseMethod()
    {
        pos = request.find_first_of(SPACES, pos);
        if(pos == std::string::npos)
            return READING;

        httpRequest.method = toMethod(request.substr(0, pos));
        request = request.substr(pos);
        pos = 0;

        pushState(&HttpParser::requireOneSpaceOrMore, &HttpParser::parseUri);
        return OK;
    }

    StateStatus parseUri()
    {
        pos = request.find_first_of(SPACES, pos);
        if(pos == std::string::npos)
            return READING;

        httpRequest.uri = decodeUri(request.substr(0, pos));
        request = request.substr(pos);

        pushState(&HttpParser::requireOneSpaceOrMore, &HttpParser::parseHttpVer);
        return OK;
    }

    StateStatus parseHttpVer()
    {
        if(request.size() < 8)
            return READING;

        assert(request.substr(0, 4) == "HTTP");
        request = request.substr(4);

        assert(request[0] == '/' && request[2] == '.' && "UPDATE THE VERSION CHECK YOU MORRON (atm it's checking for HTTP/x.x");

        httpRequest.httpVerMajor = request[1] - '0';
        httpRequest.httpVerMinor = request[3] - '0';
        request = request.substr(4);

        pushState(&HttpParser::removeLeadingSpaces, &HttpParser::requireEndOfLine, &HttpParser::startParsingHeader);
        return OK;
    }

    StateStatus startParsingHeader()
    {
        pushState(&HttpParser::waitEnfOfLine, &HttpParser::removeEndOfLine, &HttpParser::parseHeader);
        return OK;
    }

    StateStatus parseHeader()
    {
        if(request[0] == '\n' || request.substr(0, 2) == "\r\n")
        {
            pushState(&HttpParser::headerEnd);
            return OK;
        }

        pushState(&HttpParser::requireEndOfLine, &HttpParser::waitEnfOfLine);

        pos = request.find("\r\n");
        if(pos == std::string::npos)
            pos = request.find("\n");

        assert(pos != std::string::npos);
        std::string line = request.substr(0, pos);
        request = request.substr(pos);

        pos = line.find(":");
        assert(pos != std::string::npos && pos);

        std::string header = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        pos = header.find_first_of(SPACES);
        header = header.substr(0, pos);
        std::transform(header.begin(), header.end(), header.begin(), ::tolower);

        pos = value.find_first_not_of(SPACES);
        if(pos != std::string::npos)
            value = value.substr(pos);

        httpRequest.headers[header] = value;

        return CONTINUE;
    }

    StateStatus headerEnd()
    {
        //TODO refactor the whole function as there is much repetition and calculation of unneeded data

        bool hasBody = false;
        bool skipBody = false;

        bool isChunked = false;

        size_t contentLength = -1;
        bool hasContentLength = false;

        auto it = httpRequest.headers.find("content-length");
        if(it != httpRequest.headers.end())
            contentLength = std::stoi(it->second);
        hasContentLength = (contentLength > 0 && contentLength != (size_t)-1);

        it = httpRequest.headers.find("transfer-encoding");
        if(it != httpRequest.headers.end())
            isChunked = it->second == "chunked";

        skipBody = httpRequest.method == CONNECT || httpRequest.method == HEAD;
        hasBody = !skipBody && (isChunked || hasContentLength);

        //should not have content lenght and chunked encoding
        assert(!(isChunked && hasContentLength));

        if(hasBody)
        {
            if(hasContentLength)
                pushState(&HttpParser::requireEndOfLine, &HttpParser::readBody);
            else if(isChunked)
                pushState(&HttpParser::requireEndOfLine, &HttpParser::readChunked);
            else
                assert(false);
        }
        else
            receiveDone = true;

        return OK;
    }

    StateStatus readBody()
    {
        //TODO cache content-length
        auto it = httpRequest.headers.find("content-length");
        assert(it != httpRequest.headers.end());

        size_t contentLength = std::stoi(it->second);

        if(request.size() < contentLength)
            return READING;

        httpRequest.body = request.substr(0, contentLength);
        request = request.substr(contentLength);

        receiveDone = true;

        return OK;
    }

    StateStatus readChunked()
    {
        throw std::runtime_error("readChunked not implemented");
        receiveDone = true;
        return FAIL;
    }
};

inline std::string finishResponse(HttpRequest& r)
{
    static const std::string LINE_END = "\r\n";

    std::string response;

    response += std::string("HTTP/") + char(r.httpVerMajor + '0') + "." + char(r.httpVerMinor + '0') + " ";
    response += statusString[r.status] + LINE_END;

    if(r.headers.find("data") == r.headers.end())
    {
        time_t ltime;
        time(&ltime);
        tm* gmt= gmtime(&ltime);
        char* asctime_remove_nl = asctime(gmt);
        asctime_remove_nl[24] = 0;

        r.headers["date"] = asctime_remove_nl;
    }

    if(r.headers.find("server") == r.headers.end())
        r.headers["server"] = "SimpleServer";

    if(r.headers.find("connection") == r.headers.end())
        r.headers["connection"] = "close";

    if(r.headers["content-type"] == "")
        r.headers["content-type"] = "text/html; charset=ISO-8859-1";

    r.headers["content-lenght"] = std::to_string(r.body.size());

    for(const auto& pair : r.headers)
        response += pair.first + ": " + pair.second + LINE_END;

    response += LINE_END;
    response += r.body;

    return response;
}

