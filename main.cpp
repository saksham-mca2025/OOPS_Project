#include "Scheduler.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

/**
 * @file main.cpp
 * @brief Console menu for interacting with the Scheduler.
 *
 * @details This provides a simple text-based interface to add, start,
 * finish, and view tasks managed by the Scheduler class. The UI code
 * performs only basic input validation and delegates all business logic
 * to the Scheduler instance.
 */

// -------------------------
// MAIN FUNCTION
// -------------------------

int main() {
    Scheduler scheduler;
    bool running = true;

    while (running) {
        std::cout << "\n=== Simple Job Scheduler ===\n";
        std::cout << "1) Add Task\n";
        std::cout << "2) Start Task (by ID)\n";
        std::cout << "3) Finish Task (by ID)\n";
        std::cout << "4) View Staged Tasks\n";
        std::cout << "5) View Active Tasks\n";
        std::cout << "6) View Finished Log\n";
        std::cout << "0) Exit\n";
        std::cout << "Choose an option: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string junk;
            std::getline(std::cin, junk);
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string desc;
                int estimate = 0;
                std::cout << "Enter task description: ";
                std::getline(std::cin, desc);
                std::cout << "Estimated duration (seconds): ";
                if (!(std::cin >> estimate)) {
                    std::cin.clear();
                    std::string junk;
                    std::getline(std::cin, junk);
                    std::cout << "Invalid estimate. Task not added.\n";
                } else {
                    scheduler.addTask(desc, estimate);
                }
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter task id to start: ";
                if (std::cin >> id) scheduler.startTask(id);
                else { std::cin.clear(); std::string junk; std::getline(std::cin, junk); std::cout << "Invalid id.\n"; }
                break;
            }
            case 3: {
                int id;
                std::cout << "Enter task id to finish: ";
                if (std::cin >> id) scheduler.finishTask(id);
                else { std::cin.clear(); std::string junk; std::getline(std::cin, junk); std::cout << "Invalid id.\n"; }
                break;
            }
            case 4: scheduler.viewStagedTasks(); break;
            case 5: scheduler.viewActiveTasks(); break;
            case 6: scheduler.printLog(); break;
            case 0: running = false; break;
            default: std::cout << "Unknown option. Try again.\n"; break;
        }
    }

    std::cout << "Exiting scheduler. Goodbye.\n";
    return 0;
}
