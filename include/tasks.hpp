#ifndef TASKS_HPP
#define TASKS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

struct Task {
    std::string description;
    bool        isChecked;
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
    public:
        TaskManager();
        void    addTask(std::string);
        void    toggleTask(int);
        void    removeTask(int);
        void    displayTasks();
        void    run();
        ~TaskManager();
};

class Visuals {
    public:
        void    nlprint(std::string);
        void    drawfile(std::string);
        void    welcome();
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