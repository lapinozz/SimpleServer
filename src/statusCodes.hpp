#pragma once

#include <assert.h>

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
