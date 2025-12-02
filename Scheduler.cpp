#include "Scheduler.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

/**
 * @file Scheduler.cpp
 * @brief Implementation of Scheduler methods.
 *
 * @details The implementations mirror the documented behaviour in
 *          `Scheduler.h` and include console output for user feedback.
 */

/**
 * @brief Construct a new Scheduler object and initialize id counter.
 *
 * @note Initializes `nextId` to 1.
 * @return void
 */
Scheduler::Scheduler() : nextId(1) {}


void Scheduler::addTask(const std::string& description, int estimate) {
    Task t(nextId++, description, estimate);
    stagedTasks.push_back(t);
    std::cout << "Added task [#" << t.id << "] to staged tasks.\n";
}


void Scheduler::startTask(int id) {
    Task* t = findTaskById(id, stagedTasks);
    if (!t) {
        std::cout << "Task [#" << id << "] not found in staged tasks.\n";
        return;
    }

    // Mark active on the actual object in stagedTasks, then move it
    t->markActive();
    // copy to active list and remove from staged
    activeTasks.push_back(*t);
    // remove original from stagedTasks
    stagedTasks.erase(std::remove_if(stagedTasks.begin(), stagedTasks.end(), [id](const Task& x){ return x.id == id; }), stagedTasks.end());

    std::cout << "Started task [#" << id << "].\n";
}


void Scheduler::finishTask(int id) {
    Task* t = findTaskById(id, activeTasks);
    if (!t) {
        std::cout << "Task [#" << id << "] not found in active tasks.\n";
        return;
    }

    t->markFinished();
    finishedLog.push_back(*t);
    activeTasks.erase(std::remove_if(activeTasks.begin(), activeTasks.end(), [id](const Task& x){ return x.id == id; }), activeTasks.end());

    logTaskToCSV(*t);
    std::cout << "Finished task [#" << id << "].\n";
}


void Scheduler::viewStagedTasks() const {
    std::cout << "--- Staged Tasks (" << stagedTasks.size() << ") ---\n";
    if (stagedTasks.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : stagedTasks) {
        std::cout << t.getDetails() << "\n";
    }
}


void Scheduler::viewActiveTasks() const {
    std::cout << "--- Active Tasks (" << activeTasks.size() << ") ---\n";
    if (activeTasks.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : activeTasks) {
        std::cout << t.getDetails() << "\n";
    }
}


void Scheduler::printLog() const {
    std::cout << "--- Finished Tasks Log (" << finishedLog.size() << ") ---\n";
    if (finishedLog.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : finishedLog) {
        std::cout << t.getDetails();
        if (t.startTime != 0 && t.finishTime != 0) {
            long long seconds = static_cast<long long>(std::difftime(t.finishTime, t.startTime));
            long long minutes = seconds / 60;
            long long rem = seconds % 60;
            std::cout << " | Actual: " << seconds << " s (" << minutes << " m " << rem << " s)";
        }
        std::cout << "\n";
    }
}


Task* Scheduler::findTaskById(int id, std::vector<Task>& list) {
    for (auto& t : list) {
        if (t.id == id) return &t;
    }
    return nullptr;
}

const std::vector<Task>& Scheduler::getStagedTasks() const {
    return stagedTasks;
}

const std::vector<Task>& Scheduler::getActiveTasks() const {
    return activeTasks;
}

const std::vector<Task>& Scheduler::getFinishedTasks() const {
    return finishedLog;
}

void Scheduler::logTaskToCSV(const Task& task) {
    const std::string filename = "finished_tasks.csv";
    bool fileExists = false;
    
    // Check if file exists
    std::ifstream checkFile(filename);
    fileExists = checkFile.good();
    checkFile.close();
    
    std::ofstream csvFile(filename, std::ios::app);
    if (!csvFile) {
        std::cerr << "Error: Could not open CSV file for logging.\n";
        return;
    }
    
    // Write header if file is new
    if (!fileExists) {
        csvFile << "ID,Description,Estimated Duration (sec),Start Time,Finish Time,Actual Duration (sec)\n";
    }
    
    // Format timestamps
    auto formatTime = [](std::time_t t) -> std::string {
        if (t == 0) return "N/A";
        std::tm* tm = std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    };
    
    // Calculate actual duration
    long long actualDuration = 0;
    if (task.startTime != 0 && task.finishTime != 0) {
        actualDuration = static_cast<long long>(std::difftime(task.finishTime, task.startTime));
    }
    
    // Escape description for CSV (handle commas and quotes)
    std::string escapedDesc = task.description;
    if (escapedDesc.find(',') != std::string::npos || escapedDesc.find('"') != std::string::npos) {
        // Replace " with ""
        size_t pos = 0;
        while ((pos = escapedDesc.find('"', pos)) != std::string::npos) {
            escapedDesc.replace(pos, 1, "\"\"");
            pos += 2;
        }
        escapedDesc = "\"" + escapedDesc + "\"";
    }
    
    // Write task data
    csvFile << task.id << ","
            << escapedDesc << ","
            << task.estimatedDurationSeconds << ","
            << formatTime(task.startTime) << ","
            << formatTime(task.finishTime) << ","
            << actualDuration << "\n";
    
    csvFile.close();
    std::cout << "Task logged to " << filename << "\n";
}
