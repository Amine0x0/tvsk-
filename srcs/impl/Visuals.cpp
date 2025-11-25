#include "tasks.hpp"
#include <fstream>
#include <sstream>

#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_DIM     "\033[2m"
#define CLR_STRIKE  "\033[9m"

#define CLR_RED     "\033[31m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_BLUE    "\033[34m"
#define CLR_CYAN    "\033[36m"
#define CLR_WHITE   "\033[37m"
#define CLR_ORANGE  "\033[38;5;208m"

TaskManager::TaskManager() {
    using namespace std;
    taskFile = string(getenv("HOME")) + "/.local/share/tvsk/.tasks";
    _loadTasks();
}

void TaskManager::_loadTasks() {
    using namespace std;

    ifstream file(taskFile);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line)) {
        if (line.empty())
            continue;

        bool isChecked = (line[0] == '1');
        TaskPriority priority = TaskPriority::BLUE;
        string description;
        
        size_t firstSpace = line.find(' ');
        if (firstSpace != string::npos) {
            size_t secondSpace = line.find(' ', firstSpace + 1);
            if (secondSpace != string::npos) {
                try {
                    int priorityVal = stoi(line.substr(firstSpace + 1, secondSpace - firstSpace - 1));
                    if (priorityVal >= 0 && priorityVal <= 3) {
                        priority = static_cast<TaskPriority>(priorityVal);
                        description = line.substr(secondSpace + 1);
                    } else {
                        // Invalid priority, treat as old format
                        description = line.substr(firstSpace + 1);
                    }
                } catch (...) {
                    // Old format without priority, description starts after first space
                    description = line.substr(firstSpace + 1);
                }
            } else {
                // Old format without priority
                description = line.substr(firstSpace + 1);
            }
        }

        if (isChecked)
            priority = TaskPriority::GREEN;

        tasks.push_back({description, isChecked, priority});
    }

    file.close();
}

void TaskManager::_saveTasks() {
    using namespace std;

    ofstream file(taskFile);
    if (!file.is_open()) {
        cerr << CLR_RED << "Error: cannot open file " << taskFile << CLR_RESET << endl;
        return;
    }

    for (const auto &task : tasks) {
        file << (task.isChecked ? '1' : '0') << " " << static_cast<int>(task.priority) << " " << task.description << "\n";
    }

    file.close();
}

void TaskManager::_clearScreen() {
    using namespace std;
    cout << "\033[2J\033[H";
}

void TaskManager::_drawFooter() {
    using namespace std;

    cout << endl << CLR_DIM << CLR_WHITE;
    cout << "  [a]dd  [c]heck  [r]emove  [l]ist  [h]elp  [q]uit";
    cout << CLR_RESET << endl;
}

void TaskManager::addTask(std::string description) {
    using namespace std;

    if (description.empty())
        return;

    tasks.push_back({description, false, TaskPriority::BLUE});
    _saveTasks();

    cout << CLR_GREEN << CLR_BOLD << "  Task added" << CLR_RESET << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

void TaskManager::toggleTask(int index) {
    using namespace std;

    if (index < 0 || index >= static_cast<int>(tasks.size())) {
        cout << CLR_RED << CLR_BOLD << "  Invalid task index" << CLR_RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        return;
    }

    tasks[index].isChecked = !tasks[index].isChecked;
    _saveTasks();

    if (tasks[index].isChecked)
        cout << CLR_GREEN << CLR_BOLD << "  Task completed" << CLR_RESET << endl;
    else
        cout << CLR_YELLOW << CLR_BOLD << "  Task unchecked" << CLR_RESET << endl;
    
    this_thread::sleep_for(chrono::milliseconds(500));
}

void TaskManager::removeTask(int index) {
    using namespace std;

    if (index < 0 || index >= static_cast<int>(tasks.size())) {
        cout << CLR_RED << CLR_BOLD << "  Invalid task index" << CLR_RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        return;
    }

    tasks.erase(tasks.begin() + index);
    _saveTasks();

    cout << CLR_GREEN << CLR_BOLD << "  Task removed" << CLR_RESET << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

void TaskManager::clearAllTasks() {
    using namespace std;

    if (tasks.empty()) {
        cout << CLR_YELLOW << CLR_BOLD << "  No tasks to clear" << CLR_RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        return;
    }

    cout << CLR_YELLOW << CLR_BOLD << "  Are you sure? (y/n): " << CLR_RESET;
    cout.flush();
    
    string confirmation;
    if (!getline(cin, confirmation)) {
        cout << endl;
        return;
    }

    if (!confirmation.empty() && tolower(confirmation[0]) == 'y') {
        tasks.clear();
        _saveTasks();
        cout << CLR_GREEN << CLR_BOLD << "  All tasks cleared" << CLR_RESET << endl;
    } else {
        cout << CLR_YELLOW << CLR_BOLD << "  Cancelled" << CLR_RESET << endl;
    }
    
    this_thread::sleep_for(chrono::milliseconds(500));
}

void TaskManager::editPriority(int index, TaskPriority priority) {
    using namespace std;

    if (index < 0 || index >= static_cast<int>(tasks.size())) {
        cout << CLR_RED << CLR_BOLD << "  Invalid task index" << CLR_RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        return;
    }

    if (tasks[index].isChecked) {
        cout << CLR_YELLOW << CLR_BOLD << "  Cannot edit priority of completed tasks" << CLR_RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        return;
    }

    tasks[index].priority = priority;
    _saveTasks();

    string priorityName;
    switch (priority) {
        case TaskPriority::RED:
            priorityName = "High (Red)";
            break;
        case TaskPriority::ORANGE:
            priorityName = "Medium (Orange)";
            break;
        case TaskPriority::BLUE:
            priorityName = "Low (Blue)";
            break;
        case TaskPriority::GREEN:
            priorityName = "Finished";
            break;
    }

    cout << CLR_GREEN << CLR_BOLD << "  Priority set to " << priorityName << CLR_RESET << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

void TaskManager::_formatDisplay(const Task &task, int index) {
    using namespace std;

    if (task.isChecked) {
        cout << CLR_GREEN << CLR_BOLD << CLR_STRIKE;
        cout << "  [x] " << task.description;
        cout << CLR_RESET << endl;
    } else {
        string colorCode;
        string priorityLabel;
        
        switch (task.priority) {
            case TaskPriority::RED:
                colorCode = CLR_RED;
                priorityLabel = "RED";
                break;
            case TaskPriority::ORANGE:
                colorCode = CLR_ORANGE;
                priorityLabel = "ORANGE";
                break;
            case TaskPriority::BLUE:
                colorCode = CLR_BLUE;
                priorityLabel = "BLUE";
                break;
            default:
                colorCode = CLR_YELLOW;
                priorityLabel = "NONE";
                break;
        }

        cout << colorCode << CLR_BOLD;
        cout << "  [" << index << "] [" << priorityLabel << "] " << CLR_WHITE << task.description;
        cout << CLR_RESET << endl;
    }
}

void TaskManager::displayTasks() {
    using namespace std;

    cout << "\033[2J\033[H";

    Visuals visuals;
    visuals.drawAnimatedLogo();

    if (tasks.empty()) {
        cout << CLR_DIM << CLR_WHITE;
        cout << "  No tasks yet. Add one with 'a <task>'" << CLR_WHITE << endl;
        cout << CLR_RESET;
    } else {
        vector<pair<int, int>> redTasks;
        vector<pair<int, int>> orangeTasks;
        vector<pair<int, int>> blueTasks;
        vector<pair<int, int>> finishedTasks;

        for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
            if (tasks[i].isChecked) {
                finishedTasks.push_back({i, i});
            } else {
                switch (tasks[i].priority) {
                    case TaskPriority::RED:
                        redTasks.push_back({i, i});
                        break;
                    case TaskPriority::ORANGE:
                        orangeTasks.push_back({i, i});
                        break;
                    case TaskPriority::BLUE:
                        blueTasks.push_back({i, i});
                        break;
                    default:
                        blueTasks.push_back({i, i});
                        break;
                }
            }
        }

        if (!redTasks.empty()) {
            cout << CLR_RED << CLR_BOLD << "\n  RED" << CLR_RESET << endl;
            for (const auto &pair : redTasks) {
                _formatDisplay(tasks[pair.first], pair.first);
            }
        }

        if (!orangeTasks.empty()) {
            cout << CLR_ORANGE << CLR_BOLD << "\n  ORANGE" << CLR_RESET << endl;
            for (const auto &pair : orangeTasks) {
                _formatDisplay(tasks[pair.first], pair.first);
            }
        }

        if (!blueTasks.empty()) {
            cout << CLR_BLUE << CLR_BOLD << "\n  BLUE" << CLR_RESET << endl;
            for (const auto &pair : blueTasks) {
                _formatDisplay(tasks[pair.first], pair.first);
            }
        }

        if (!finishedTasks.empty()) {
            cout << CLR_GREEN << CLR_BOLD << "\n  FINISHED" << CLR_RESET << endl;
            for (const auto &pair : finishedTasks) {
                _formatDisplay(tasks[pair.first], pair.first);
            }
        }
    }

    _drawFooter();
}

void TaskManager::_showHelp() {
    using namespace std;

    cout << "\033[2J\033[H";

    Visuals visuals;
    visuals.drawAnimatedLogo();

    cout << endl << CLR_BOLD << CLR_WHITE << "Commands:" << CLR_RESET << endl << endl;
    cout << CLR_CYAN << "[a]dd <task>        " << CLR_WHITE << "Add a new task" << CLR_RESET << endl;
    cout << CLR_CYAN << "[c]heck <index>     " << CLR_WHITE << "Toggle task completion" << CLR_RESET << endl;
    cout << CLR_CYAN << "[r]emove <index>    " << CLR_WHITE << "Delete a task" << CLR_RESET << endl;
    cout << CLR_CYAN << "[p]riority <i> <p>  " << CLR_WHITE << "Set priority (0=Blue, 1=Orange, 2=Red)" << CLR_RESET << endl;
    cout << CLR_CYAN << "[clear]             " << CLR_WHITE << "Clear all tasks" << CLR_RESET << endl;
    cout << CLR_CYAN << "[l]ist              " << CLR_WHITE << "Show all tasks" << CLR_RESET << endl;
    cout << CLR_CYAN << "[h]elp              " << CLR_WHITE << "Show this help message" << CLR_RESET << endl;
    cout << CLR_CYAN << "[q]uit              " << CLR_WHITE << "Exit the application" << CLR_RESET << endl;

    _drawFooter();
}

void TaskManager::run() {
    using namespace std;

    displayTasks();

    string command;
    while (true) {
        cout << CLR_BOLD << CLR_BLUE << "\n  > " << CLR_RESET;
        cout.flush();
        
        if (!getline(cin, command)) {
            cout << endl << CLR_GREEN << CLR_BOLD;
            cout << "Bye Bye :> " << CLR_RESET << endl;
            break;
        }

        if (command.empty())
            continue;

        size_t spacePos = command.find(' ');
        if (spacePos == string::npos)
            spacePos = command.length();
        if (command.substr(0, spacePos) == "clear") {
            clearAllTasks();
            displayTasks();
            continue;
        }

        char cmd = tolower(command[0]);
        
        if (!isalpha(cmd)) {
            cout << CLR_RED << "  Invalid input. Type 'h' for help" << CLR_RESET << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            continue;
        }

        if (cmd == 'l') {
            displayTasks();
        } else if (cmd == 'a') {
            size_t pos = command.find(' ');
            if (pos != string::npos) {
                addTask(command.substr(pos + 1));
                displayTasks();
            } else {
                cout << CLR_RED << "  Usage: a <task description>" << CLR_RESET << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }
        } else if (cmd == 'c') {
            size_t pos = command.find(' ');
            if (pos != string::npos) {
                try {
                    int idx = stoi(command.substr(pos + 1));
                    toggleTask(idx);
                    displayTasks();
                } catch (...) {
                    cout << CLR_RED << "  Usage: c <index>" << CLR_RESET << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
            }
        } else if (cmd == 'r') {
            size_t pos = command.find(' ');
            if (pos != string::npos) {
                try {
                    int idx = stoi(command.substr(pos + 1));
                    removeTask(idx);
                    displayTasks();
                } catch (...) {
                    cout << CLR_RED << "  Usage: r <index>" << CLR_RESET << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
            }
        } else if (cmd == 'p') {
            size_t firstSpace = command.find(' ');
            if (firstSpace != string::npos) {
                size_t secondSpace = command.find(' ', firstSpace + 1);
                if (secondSpace != string::npos) {
                    try {
                        int idx = stoi(command.substr(firstSpace + 1, secondSpace - firstSpace - 1));
                        int priorityVal = stoi(command.substr(secondSpace + 1));
                        
                        if (priorityVal < 0 || priorityVal > 2) {
                            cout << CLR_RED << "  Priority must be 0 (Blue), 1 (Orange), or 2 (Red)" << CLR_RESET << endl;
                            this_thread::sleep_for(chrono::milliseconds(500));
                        } else {
                            editPriority(idx, static_cast<TaskPriority>(priorityVal));
                            displayTasks();
                        }
                    } catch (...) {
                        cout << CLR_RED << "  Usage: p <index> <priority>" << CLR_RESET << endl;
                        this_thread::sleep_for(chrono::milliseconds(500));
                    }
                } else {
                    cout << CLR_RED << "  Usage: p <index> <priority>" << CLR_RESET << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
            } else {
                cout << CLR_RED << "  Usage: p <index> <priority>" << CLR_RESET << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }
        } else if (cmd == 'h') {
            _showHelp();
        } else if (cmd == 'q') {
            _clearScreen();
            cout << CLR_GREEN << CLR_BOLD;
            cout << "\n  Goodbye\n" << CLR_RESET << endl;
            break;
        } else {
            cout << CLR_RED << "  Unknown command. Type 'h' for help" << CLR_RESET << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
}

TaskManager::~TaskManager() {
}

void Visuals::nlprint(std::string in)
{
    using namespace std;

    if (in.empty())
        return;
    cout << in << endl;
}

void Visuals::drawfile(std::string Path) {
    std::ifstream file(Path);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << Path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
}

void Visuals::welcome()
{
    using namespace std;
    
    if (logoLines.empty()) {
        string logoPath = string(getenv("HOME")) + "/.local/share/tvsk/art/tvsk!";
        ifstream file(logoPath);
        if (!file.is_open()) {
            cerr << "Error: cannot open logo file " << logoPath << endl;
            return;
        }
        
        string line;
        while (getline(file, line)) {
            logoLines.push_back(line);
        }
        file.close();
    }
    
    cout << "\033[2J\033[H";
    cout << CLR_BOLD << CLR_CYAN;
    for (const auto &line : logoLines) {
        cout << line << endl;
    }
    cout << CLR_RESET;
    cout.flush();
    
    // Add spacing
    for (int i = 0; i < 3; i++)
        cout << endl;
}

void Visuals::_loadLogo()
{
    using namespace std;
    
    if (logoLines.empty()) {
        string logoPath = string(getenv("HOME")) + "/.local/share/tvsk/art/tvsk!";
        ifstream file(logoPath);
        if (!file.is_open()) {
            cerr << "Error: cannot open logo file " << logoPath << endl;
            return;
        }
        
        string line;
        while (getline(file, line)) {
            logoLines.push_back(line);
        }
        file.close();
    }
}

void Visuals::drawAnimatedLogo()
{
    using namespace std;
    
    _loadLogo();
    
    vector<string> effects = {CLR_DIM, CLR_BOLD, CLR_DIM, CLR_BOLD};
    
    for (const auto &effect : effects) {
        cout << "\033[1;1H";  // Move cursor to top
        cout << effect << CLR_CYAN;
        
        for (const auto &line : logoLines) {
            cout << line << endl;
        }
        
        cout << CLR_RESET;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    cout << "\033[1;1H";
    cout << CLR_BOLD << CLR_CYAN;
    for (const auto &line : logoLines) {
        cout << line << endl;
    }
    cout << CLR_RESET;
    cout.flush();
}