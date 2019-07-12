#include <fstream>

#include "../include/SimpleServer/simpleserver.hpp"

using namespace SimpleServer;

HttpRequest process(HttpRequest& r)
{
    HttpRequest response;

    auto path = split(r.path, "/", true);

    if(path.empty())
        response.body =
        R"(
            <html>
                <head>
                    <title>Homepage</title>
                </head>
                <body>
                    <h3>
                        No index.html here! It's all in the code!
                    </h3>
                    <a href="/img"> Test File Serving </a><br>
                    <a href="/post"> Test Post </a><br>
                    <a href="/fake"> Test 404 </a><br>
                    <br><br>
                    <a href="/file/poster.html"> Post Utility </a><br>
                    </img>
                </body>
            </html>
        )";

    else if(path[0] == "img")
    {
        response.body =
        R"(
            <html>
                <head>
                    <title>img test</title>
                </head>
                <body>
                    <img src="file/logo.png">
                    </img>
                </body>
            </html>
        )";
    }
    else if(path[0] == "number")
    {
        if(r.params["num"] != "")
            response.body = "This is the number : " + r.params["num"] + "!";
        else
            response.body = "There is no number!";
    }
    else if(path[0] == "post")
    {
        response.body =
        R"(
            <html>
                <head>
                    <title>Post Test</title>
                </head>
                <body>
                    <form enctype="application/x-www-form-urlencoded" method="post" action="number">
                        Number:
                        <input type="number" name="num" value="43"><br>
                        <input type="submit" value="Submit"><br>
                    </form>
                </body>
            </html>
        )";
    }
    else if(path[0] == "file")
    {
        std::ifstream file(path[1], std::ifstream::binary);
        if(!file)
            return response;

        file.seekg(0, std::ios::end);
        std::streampos length(file.tellg());

        if (length)
        {
            file.seekg(0, std::ios::beg);
            response.body.resize(length);
            file.read(&response.body.front(), length);
        }

        std::string contentType = "text/plain";

        size_t extensionPos = path[1].find_last_of(".");
        if(extensionPos != std::string::npos)
        {
            std::string extention = path[1].substr(extensionPos);

            if(extention == ".png" || extention == ".jpeg")
                contentType = "image/*";
            else if(extention == ".html")
                contentType = "text/html";
        }

        response.headers["content-type"] = contentType;
        response.status = S_200;
    }
    else
    {
        response.status = S_404;
        response.body =
        R"(
            <html>
                <head>
                    <title>Wrong URL</title>
                </head>
                <body>
                    <h1>Wrong URL</h1>
                    Path is : &gt;)" + r.path + R"(&lt;
                </body>
            </html>
        )";
    }

    return response;
}

int main()
{
    sockInit();

    auto listener = makeListener(8080, false);
    if(SOCKET_IS_INVALID(listener))
        throw std::runtime_error("ERROR on makeListeneer");

    while(true)
    {
        auto client = makeSocket(listener);
        if(SOCKET_IS_INVALID(client))
            throw std::runtime_error("ERROR on makeSocket");

        auto request = HttpParser().receiveRequest(client);
        auto processed = process(request);
        auto response = finishResponse(processed);

        send(client, response.c_str(), response.size(), 0);

        sockClose(client);
    }

    sockClose(listener);

    sockQuit();

    return 0;
}
