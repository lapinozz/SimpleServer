#pragma once

enum Method : int32_t {GET, HEAD, POST, OPTIONS, CONNECT, TRACE, PUT, PATCH, DELETE};

static const char* methodsString[] =
{
    "GET",
    "HEAD",
    "POST",
    "OPTIONS",
    "CONNECT",
    "TRACE",
    "PUT",
    "PATCH",
    "DELETE"
};

static Method toMethod(std::string method)
{
    //this line is to get ride of unused variable warning
    (void)methodsString;

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

