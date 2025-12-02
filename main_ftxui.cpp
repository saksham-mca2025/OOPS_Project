#include "Scheduler.h"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <sstream>
#include <vector>

/**
 * @file main_ftxui.cpp
 * @brief FTXUI interface for interacting with the Scheduler.
 *
 * @details This provides a text-based UI using the FTXUI library to add,
 *          start, finish, and view tasks managed by the Scheduler class.
 */

using namespace ftxui;

std::string statusToString(Status s) {
    switch (s) {
        case Status::Staged: return "Staged";
        case Status::Active: return "Active";
        case Status::Finished: return "Finished";
        default: return "Unknown";
    }
}

int main() {
    Scheduler scheduler;
    
    // State variables
    int selected = 0;
    std::string input_description = "";
    std::string input_estimate = "";
    std::string input_task_id = "";
    std::string status_message = "";
    
    auto screen = ScreenInteractive::Fullscreen();
    
    // Menu entries
    std::vector<std::string> menu_entries = {
        "Add Task",
        "Start Task",
        "Finish Task",
        "View Staged Tasks",
        "View Active Tasks",
        "View Finished Log",
        "Exit"
    };
    
    // Input components
    auto input_desc_component = Input(&input_description, "Task description");
    auto input_est_component = Input(&input_estimate, "Estimate (seconds)");
    auto input_id_component = Input(&input_task_id, "Task ID");
    
    // Menu
    auto menu = Menu(&menu_entries, &selected);
    
    // Action handler
    auto action_button = Button("Execute", [&] {
        status_message = "";
        
        switch (selected) {
            case 0: { // Add Task
                if (!input_description.empty() && !input_estimate.empty()) {
                    try {
                        int estimate = std::stoi(input_estimate);
                        scheduler.addTask(input_description, estimate);
                        status_message = "✓ Task added successfully!";
                        input_description = "";
                        input_estimate = "";
                    } catch (...) {
                        status_message = "✗ Invalid estimate value";
                    }
                } else {
                    status_message = "✗ Please fill in all fields";
                }
                break;
            }
            case 1: { // Start Task
                if (!input_task_id.empty()) {
                    try {
                        int id = std::stoi(input_task_id);
                        scheduler.startTask(id);
                        status_message = "✓ Task started!";
                        input_task_id = "";
                    } catch (...) {
                        status_message = "✗ Invalid task ID";
                    }
                } else {
                    status_message = "✗ Please enter a task ID";
                }
                break;
            }
            case 2: { // Finish Task
                if (!input_task_id.empty()) {
                    try {
                        int id = std::stoi(input_task_id);
                        scheduler.finishTask(id);
                        status_message = "✓ Task finished and logged to CSV!";
                        input_task_id = "";
                    } catch (...) {
                        status_message = "✗ Invalid task ID";
                    }
                } else {
                    status_message = "✗ Please enter a task ID";
                }
                break;
            }
            case 6: // Exit
                screen.ExitLoopClosure()();
                break;
        }
    });
    
    // Combine components
    auto input_container = Container::Vertical({
        input_desc_component,
        input_est_component,
        input_id_component,
        action_button,
    });
    
    auto main_container = Container::Horizontal({
        menu,
        input_container,
    });
    
    auto renderer = Renderer(main_container, [&] {
        // Build task display based on selected view
        Elements task_display;
        
        if (selected == 3) { // Staged Tasks
            task_display.push_back(text("Staged Tasks") | bold | color(Color::Cyan));
            task_display.push_back(separator());
            auto& tasks = scheduler.getStagedTasks();
            if (tasks.empty()) {
                task_display.push_back(text("(none)") | dim);
            } else {
                for (const auto& t : tasks) {
                    task_display.push_back(text("[#" + std::to_string(t.id) + "] " + t.description + 
                        " | " + std::to_string(t.estimatedDurationSeconds) + " sec"));
                }
            }
        } else if (selected == 4) { // Active Tasks
            task_display.push_back(text("Active Tasks") | bold | color(Color::Yellow));
            task_display.push_back(separator());
            auto& tasks = scheduler.getActiveTasks();
            if (tasks.empty()) {
                task_display.push_back(text("(none)") | dim);
            } else {
                for (const auto& t : tasks) {
                    task_display.push_back(text("[#" + std::to_string(t.id) + "] " + t.description + 
                        " | " + std::to_string(t.estimatedDurationSeconds) + " sec"));
                }
            }
        } else if (selected == 5) { // Finished Tasks
            task_display.push_back(text("Finished Tasks") | bold | color(Color::Green));
            task_display.push_back(separator());
            auto& tasks = scheduler.getFinishedTasks();
            if (tasks.empty()) {
                task_display.push_back(text("(none)") | dim);
            } else {
                for (const auto& t : tasks) {
                    long long duration = 0;
                    if (t.startTime != 0 && t.finishTime != 0) {
                        duration = static_cast<long long>(std::difftime(t.finishTime, t.startTime));
                    }
                    task_display.push_back(text("[#" + std::to_string(t.id) + "] " + t.description + 
                        " | Actual: " + std::to_string(duration) + " sec"));
                }
            }
        }
        
        // Build input panel based on selected action
        Element input_panel;
        if (selected == 0) {
            input_panel = vbox({
                text("Add New Task") | bold | color(Color::Cyan),
                separator(),
                input_desc_component->Render(),
                input_est_component->Render(),
                separator(),
                action_button->Render(),
            }) | border;
        } else if (selected == 1 || selected == 2) {
            input_panel = vbox({
                text(selected == 1 ? "Start Task" : "Finish Task") | bold | color(Color::Cyan),
                separator(),
                input_id_component->Render(),
                separator(),
                action_button->Render(),
            }) | border;
        } else if (selected == 3 || selected == 4 || selected == 5) {
            input_panel = vbox(task_display) | border | flex;
        } else {
            input_panel = vbox({
                text("Select an action from the menu") | dim,
            }) | border;
        }
        
        return vbox({
            text("╔════════════════════════════════════════════╗") | bold | color(Color::Blue),
            text("║      Simple Job Scheduler (FTXUI)          ║") | bold | color(Color::Blue),
            text("╚════════════════════════════════════════════╝") | bold | color(Color::Blue),
            separator(),
            hbox({
                vbox({
                    text("Main Menu") | bold | underlined,
                    separator(),
                    menu->Render(),
                }) | size(WIDTH, EQUAL, 30) | border,
                separator(),
                input_panel | flex,
            }) | flex,
            separator(),
            text("Status: " + status_message) | color(
                status_message.find("✓") != std::string::npos ? Color::Green : 
                status_message.find("✗") != std::string::npos ? Color::Red : Color::GrayDark
            ),
            separator(),
            text("Use ↑/↓ arrows | Tab to switch | Enter to execute/select") | dim,
        });
    });
    
    screen.Loop(renderer);
    
    return 0;
}
