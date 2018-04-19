#include <iostream>
#include <thread>
#include "server.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
    //__try
    try {
        Server server("127.0.0.1", 8080);
        server.close();
        getchar();
    }

    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        getchar();
    }
    
    return 0;
}