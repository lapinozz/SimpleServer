#pragma once

#include <string>
#include <sstream>
#include <vector>

static bool isHex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

uint64_t getTimeMs()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

static std::string decodeUri(std::string uri)
{
    size_t pos = 0;
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

static void split(std::vector<std::string>& tokens, const std::string &text, const std::string& delimiter = " ", bool skipEmtpy = false)
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

static std::vector<std::string> split(const std::string &text, const std::string& delimiter = " ", bool skipEmtpy = false)
{
    std::vector<std::string> tokens;
    split(tokens, text, delimiter, skipEmtpy);
    return tokens;
}
