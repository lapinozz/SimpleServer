add_library(Networking INTERFACE IMPORTED)

if (WIN32)
    target_link_libraries(Networking INTERFACE -lws2_32 -lwsock32)
endif (WIN32)