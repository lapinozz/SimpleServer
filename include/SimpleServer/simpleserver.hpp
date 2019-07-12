#pragma once

#include <chrono>
#include <vector>
#include <cassert>
#include <cstring>
#include <map>
#include <functional>
#include <algorithm>
#include <sstream>

namespace SimpleServer
{
    /// utility.hpp
    inline bool isHex(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    inline uint64_t getTimeMs()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    inline std::string decodeUri(std::string uri)
    {
        size_t pos;
        while((pos = uri.find("%", pos)) != std::string::npos)
        {
            if(pos + 2 >= uri.size() || !isHex(uri[pos + 1]) || !isHex(uri[pos + 2]))
            {
                pos++;
                continue;
            }

            std::stringstream h;
            h << uri.substr(pos + 1, 2);
            h << std::hex;

            int i;
            h >> i;

            uri.replace(pos, 3, {(char)i});

            pos++;
        }

        return uri;
    }

    inline void split(std::vector<std::string>& tokens, const std::string &text, const std::string& delimiter = " ", bool skipEmtpy = false)
    {
        std::size_t start = 0;
        std::size_t end = 0;

        while((end = text.find(delimiter, start)) != std::string::npos)
        {
            if(!skipEmtpy || end != start)
                tokens.push_back(text.substr(start, end - start));

            start = end + delimiter.size();
        }

        if(!skipEmtpy || start < text.size())
            tokens.push_back(text.substr(start));
    }

    inline std::vector<std::string> split(const std::string &text, const std::string& delimiter = " ", bool skipEmtpy = false)
    {
        std::vector<std::string> tokens;
        split(tokens, text, delimiter, skipEmtpy);
        return tokens;
    }
    ///utilit.hpp end

    ///statusCodes.hpp
    enum Status {S_100, S_101, S_102, S_200, S_201, S_202, S_203, S_204, S_205, S_206, S_207, S_210, S_226, S_300, S_301, S_302, S_303, S_304, S_305, S_306, S_307, S_308, S_310, S_400, S_401, S_402, S_403, S_404, S_405, S_406, S_407, S_408, S_409, S_410, S_411, S_412, S_413, S_414, S_415, S_416, S_417, S_418, S_421, S_422, S_423, S_424, S_425, S_426, S_428, S_429, S_431, S_449, S_450, S_451, S_456, S_499, S_500, S_501, S_502, S_503, S_504, S_505, S_506, S_507, S_508, S_509, S_510, S_511, S_520};

    inline Status toStatus(int status)
    {
        if(status == 100) return S_100;
        else if(status == 101) return S_101;
        else if(status == 102) return S_102;
        else if(status == 200) return S_200;
        else if(status == 201) return S_201;
        else if(status == 202) return S_202;
        else if(status == 203) return S_203;
        else if(status == 204) return S_204;
        else if(status == 205) return S_205;
        else if(status == 206) return S_206;
        else if(status == 207) return S_207;
        else if(status == 210) return S_210;
        else if(status == 226) return S_226;
        else if(status == 300) return S_300;
        else if(status == 301) return S_301;
        else if(status == 302) return S_302;
        else if(status == 303) return S_303;
        else if(status == 304) return S_304;
        else if(status == 305) return S_305;
        else if(status == 306) return S_306;
        else if(status == 307) return S_307;
        else if(status == 308) return S_308;
        else if(status == 310) return S_310;
        else if(status == 400) return S_400;
        else if(status == 401) return S_401;
        else if(status == 402) return S_402;
        else if(status == 403) return S_403;
        else if(status == 404) return S_404;
        else if(status == 405) return S_405;
        else if(status == 406) return S_406;
        else if(status == 407) return S_407;
        else if(status == 408) return S_408;
        else if(status == 409) return S_409;
        else if(status == 410) return S_410;
        else if(status == 411) return S_411;
        else if(status == 412) return S_412;
        else if(status == 413) return S_413;
        else if(status == 414) return S_414;
        else if(status == 415) return S_415;
        else if(status == 416) return S_416;
        else if(status == 417) return S_417;
        else if(status == 418) return S_418;
        else if(status == 421) return S_421;
        else if(status == 422) return S_422;
        else if(status == 423) return S_423;
        else if(status == 424) return S_424;
        else if(status == 425) return S_425;
        else if(status == 426) return S_426;
        else if(status == 428) return S_428;
        else if(status == 429) return S_429;
        else if(status == 431) return S_431;
        else if(status == 449) return S_449;
        else if(status == 450) return S_450;
        else if(status == 451) return S_451;
        else if(status == 456) return S_456;
        else if(status == 499) return S_499;
        else if(status == 500) return S_500;
        else if(status == 501) return S_501;
        else if(status == 502) return S_502;
        else if(status == 503) return S_503;
        else if(status == 504) return S_504;
        else if(status == 505) return S_505;
        else if(status == 506) return S_506;
        else if(status == 507) return S_507;
        else if(status == 508) return S_508;
        else if(status == 509) return S_509;
        else if(status == 510) return S_510;
        else if(status == 511) return S_511;
        else if(status == 520) return S_520;

        assert(false && "INVALID STATUS");
    }

    static const char* statusString[] =
    {
        "100 Continue",
        "101 Switching Protocols",
        "102 Processing",
        "200 OK",
        "201 Created",
        "202 Accepted",
        "203 Non-Authoritative Information",
        "204 No Content",
        "205 Reset Content",
        "206 Partial Content",
        "207 Multi-Status",
        "210 Content Different",
        "226 IM Used",
        "300 Multiple Choices",
        "301 Moved Permanently",
        "302 Moved Temporarily",
        "303 See Other",
        "304 Not Modified",
        "305 Use Proxy",
        "306 (aucun)",
        "307 Temporary Redirect",
        "308 Permanent Redirect",
        "310 Too many Redirects",
        "400 Bad Request",
        "401 Unauthorized",
        "402 Payment Required",
        "403 Forbidden",
        "404 Not Found",
        "405 Method Not Allowed",
        "406 Not Acceptable",
        "407 Proxy Authentication Required",
        "408 Request Time-out",
        "409 Conflict",
        "410 Gone",
        "411 Length Required",
        "412 Precondition Failed",
        "413 Request Entity Too Large",
        "414 Request-URI Too Long",
        "415 Unsupported Media Type",
        "416 Requested range unsatisfiable",
        "417 Expectation failed",
        "418 I’m a teapot",
        "421 Bad mapping / Misdirected Request",
        "422 Unprocessable entity",
        "423 Locked",
        "424 Method failure",
        "425 Unordered Collection",
        "426 Upgrade Required",
        "428 Precondition Required",
        "429 Too Many Requests",
        "431 Request Header Fields Too Large",
        "449 Retry With",
        "450 Blocked by Windows Parental Controls",
        "451 Unavailable For Legal Reasons",
        "456 Unrecoverable Error",
        "499 Client has closed connection",
        "500 Internal Server Error",
        "501 Not Implemented",
        "502 Bad Gateway ou Proxy Error",
        "503 Service Unavailable",
        "504 Gateway Time-out",
        "505 HTTP Version not supported",
        "506 Variant Also Negotiates",
        "507 Insufficient storage",
        "508 Loop detected",
        "509 Bandwidth Limit Exceeded",
        "510 Not extended",
        "511 Network authentication required",
        "520 Web server is returning an unknown error"
    };
    ///statusCodes.hpp end

    ///methods.hpp
    enum Method : int32_t {GET, HEAD, POST, OPTIONS, CONNECT, TRACE, PUT, PATCH, DELETE};

    inline Method toMethod(std::string method)
    {
        if(method == "GET") return GET;
        else if(method == "HEAD") return HEAD;
        else if(method == "POST") return POST;
        else if(method == "OPTIONS") return OPTIONS;
        else if(method == "CONNECT") return CONNECT;
        else if(method == "TRACE") return TRACE;
        else if(method == "PUT") return PUT;
        else if(method == "PATCH") return PATCH;
        else if(method == "DELETE") return DELETE;

        assert(false && "INVALID METHOD");
    }
    ///methods.hpp end

    ///portableSocket.hpp
    #ifdef _WIN32
      /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
      #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501  /* Windows XP. */
      #endif
      #include <winsock2.h>
      #include <Ws2tcpip.h>

      #define NON_BLOCK_FLAG FIONBIO

      #define SOCKET_IS_INVALID(_v)(_v == INVALID_SOCKET)
      using SOCKLEN = int;
    #else
      /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
      #include <sys/socket.h>
      #include <arpa/inet.h>
      #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
      #include <unistd.h> /* Needed for close() */
      #include <fcntl.h>

      #define NON_BLOCK_FLAG O_NONBLOCK

      #define SOCKET_IS_INVALID(_v)(_v < 0)

      #define INVALID_SOCKET -1
      #define SOCKET_ERROR  -1
      using SOCKET = int;
      using SOCKLEN = unsigned int;
    #endif

    int sockInit(void)
    {
      #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(1,1), &wsa_data);
      #else
        return 0;
      #endif
    }

    int sockQuit(void)
    {
      #ifdef _WIN32
        return WSACleanup();
      #else
        return 0;
      #endif
    }

    int sockClose(SOCKET sock)
    {
      #ifdef _WIN32
        #define CLOSE_SOCKET_FUNCTION closesocket
        #define SHUTDOWN_FLAG SD_SEND
      #else
        #define CLOSE_SOCKET_FUNCTION close
        #define SHUTDOWN_FLAG SHUT_WR
      #endif

      int status = 0;

      status = shutdown(sock, SHUTDOWN_FLAG);
      if(status != 0)
        return status;

    //TODO set a timeout here
      while(true)
      {
          char buff[16];
          auto size = recv(sock, buff, sizeof(buff), 0);

          if(size <= 0)
            break;
      }

      status = CLOSE_SOCKET_FUNCTION(sock);

      return status;

      #undef CLOSE_SOCKET_FUNCTION
      #undef SHUTDOWN_FLAG
    }

    bool setBlocking(int fd, bool blocking)
    {
        #ifdef WIN32
           unsigned long mode = blocking ? 0 : 1;
           return (ioctlsocket(fd, FIONBIO, &mode) == 0);
        #else
           int flags = fcntl(fd, F_GETFL, 0);
           if (flags < 0) return false;
           flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
           return (fcntl(fd, F_SETFL, flags) == 0);
        #endif
    }

    SOCKET makeListener(int port, bool blocking = true)
    {
        auto listSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(SOCKET_IS_INVALID(listSocket))
            return listSocket;

        int iSetOption = 1;
        setsockopt(listSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

        struct sockaddr_in listAddr;
        memset(&listAddr, 0, sizeof(listAddr));

        listAddr.sin_family = AF_INET;
        listAddr.sin_addr.s_addr = INADDR_ANY;
        listAddr.sin_port = htons(port);

        auto error = bind(listSocket, (struct sockaddr *) &listAddr, sizeof(listAddr));
        if(error == SOCKET_ERROR)
            return error;

        listen(listSocket, 5);

        setBlocking(listSocket, blocking);

        return listSocket;
    }

    SOCKET makeSocket(SOCKET listener, uint64_t timeoutMs = 0, bool blocking = true)
    {
        auto newSocket = INVALID_SOCKET;

        struct sockaddr_in newSocketAddr;
        SOCKLEN addrSize = sizeof(newSocketAddr);

        auto time = getTimeMs();

        while(SOCKET_IS_INVALID(newSocket) && (timeoutMs == 0 || getTimeMs() - time < timeoutMs))
            newSocket = accept(listener, (struct sockaddr*)&newSocketAddr, &addrSize);

        setBlocking(newSocket, blocking);

        return newSocket;
    }
    ///portableSocket.hpp end

    ///server.hpp
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
    ///server.hpp end
}
