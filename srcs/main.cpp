#include "tasks.hpp"

int main()
{
    
    if(getenv("HOME") == nullptr){
        return 1;
    }

    Core *instance = new Core();

    try {
        instance->run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    delete instance;
    return 0;
}
