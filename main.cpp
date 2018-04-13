#include <iostream>
#include "server.h"


int main()
{
    try {
        Server server("127.0.0.1", "8080");
        server.write("hello from server");
        std::string buffer = server.read();
        std::cout << "take from client : "<< buffer << std::endl;
        server.close();
    }

    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

