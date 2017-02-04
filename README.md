# ![Simple Server](./logo.png)

---

## Files

* The src folder contains the original files
* The simpleserver.hpp contains all the files packaged for convenience
* main.cpp is an example of a simple server
* logo.png is the logo and is used for example
* poster.html is an example to help test the behavior of your server

## A HTTP server for C++

* Header Only
* No dependency
* Cross platform (Windows and all POSIX compliant platform)
* Simple

## Purpose

This is not an high-end server, it's a small and simple minimal server that was made for example, to have a web interface for debugging a game.
I made this little utility because I wanted to make a reflection layer available as a web API.

## Compiling

You will need C++11. Althought if you don't wan't it, it would be easy to remove the C++11 code.

On POSIX platforms(linux, Mac, etc), you only need to include simpleserver.hpp
On Windows you will also need to link to ws2_32.lib and wsock32.lib wich are both system lib
