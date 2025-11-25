#include "tasks.hpp"
#include <fstream>
#include <sstream>

#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_DIM     "\033[2m"

#define CLR_RED     "\033[31m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_BLUE    "\033[34m"
#define CLR_CYAN    "\033[36m"
#define CLR_WHITE   "\033[37m"

TaskManager::TaskManager() : taskFile(".tasks") {
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
        string description = line.substr(2);

        tasks.push_back({description, isChecked});
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
        file << (task.isChecked ? '1' : '0') << " " << task.description << "\n";
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

    tasks.push_back({description, false});
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

void TaskManager::_formatDisplay(const Task &task, int index) {
    using namespace std;

    if (task.isChecked) {
        cout << CLR_GREEN << CLR_BOLD;
        cout << "  [x] " << CLR_DIM << task.description;
        cout << CLR_RESET << endl;
    } else {
        cout << CLR_YELLOW << CLR_BOLD;
        cout << "  [" << index << "] " << CLR_WHITE << task.description;
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
        for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
            _formatDisplay(tasks[i], i);
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
        ifstream file("./art/tvsk!");
        if (!file.is_open()) {
            cerr << "Error: cannot open logo file ./art/tvsk!" << endl;
            return;
        }
        
        string line;
        while (getline(file, line)) {
            logoLines.push_back(line);
        }
        file.close();
    }
    
    // Display logo once on welcome
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
        ifstream file("./art/tvsk!");
        if (!file.is_open()) {
            cerr << "Error: cannot open logo file ./art/tvsk!" << endl;
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
    
    // Simple animation: flash the logo a few times
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
    
    // Final bright state
    cout << "\033[1;1H";
    cout << CLR_BOLD << CLR_CYAN;
    for (const auto &line : logoLines) {
        cout << line << endl;
    }
    cout << CLR_RESET;
    cout.flush();
}