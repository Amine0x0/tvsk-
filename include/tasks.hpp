#ifndef TASKS_HPP
#define TASKS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

enum class TaskPriority {
    BLUE = 0,      // Low priority
    ORANGE = 1,    // Medium priority
    RED = 2,       // High priority
    GREEN = 3      // Finished/Completed
};

struct Task {
    std::string description;
    bool        isChecked;
    TaskPriority priority;
};

class TaskManager {
    private:
        std::vector<Task> tasks;
        std::string       taskFile;

        void    _loadTasks();
        void    _saveTasks();
        void    _formatDisplay(const Task &task, int index);
        void    _clearScreen();
        void    _drawHeader();
        void    _drawFooter();
        void    _showHelp();
        char    _priorityToColor(TaskPriority priority) const;
    public:
        TaskManager();
        void    addTask(std::string);
        void    toggleTask(int);
        void    removeTask(int);
        void    editPriority(int, TaskPriority);
        void    clearAllTasks();
        void    displayTasks();
        void    run();
        ~TaskManager();
};

class Visuals {
    private:
        std::vector<std::string> logoLines;
        
        void    _loadLogo();
    public:
        void    nlprint(std::string);
        void    drawfile(std::string);
        void    welcome();
        void    drawAnimatedLogo();
};

class Core {
    private:
        Visuals visuals;

    public:
        Core();
        void    run();
        void    logic();
        ~Core();
};

#endif