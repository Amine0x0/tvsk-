#include "tasks.hpp"

Core::Core() {}

void Core::run()
{
    using namespace std;
    logic();
    exit(0);
}

void Core::logic()
{
    using namespace std;

    visuals.welcome();
    
    TaskManager taskManager;
    taskManager.displayTasks();
    taskManager.run();
}

Core::~Core()
{
}