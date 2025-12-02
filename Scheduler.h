#pragma once

#include "Task.h"
#include <vector>
#include <string>

/**
 * @file Scheduler.h
 * @brief Declaration of the Scheduler class which manages Task lifecycle.
 */

/**
 * @class Scheduler
 * @brief Manages collections of tasks and their state transitions.
 *
 * @details Scheduler owns three containers grouping tasks by lifecycle:
 *          stagedTasks (not started), activeTasks (in progress), and
 *          finishedLog (completed). All state transition logic and timing
 *          calculations are implemented here so UI code remains thin.
 */
class Scheduler {
public:
    /**
     * @brief Construct a new Scheduler object.
     *
     * @note Initializes internal id counter used to assign unique task ids.
     */
    Scheduler();

    /**
     * @brief Add a new task to the staged list.
     *
     * @param description Human-readable description of the task.
     * @param estimate Estimated duration in seconds.
     * @note Side-effect: appends a Task to `stagedTasks` and increments `nextId`.
     * @return void
     */
    void addTask(const std::string& description, int estimate);

    /**
     * @brief Start a staged task by id.
     *
     * Moves a task from `stagedTasks` to `activeTasks` and records its start time.
     *
     * @param id Unique task identifier to start.
     * @note Side-effect: removes the task from `stagedTasks`, appends it to
     *       `activeTasks`, and modifies the Task's `status` and `startTime`.
     * @return void
     */
    void startTask(int id);

    /**
     * @brief Finish an active task by id.
     *
     * Moves a task from `activeTasks` to `finishedLog` and records its finish time.
     *
     * @param id Unique task identifier to finish.
     * @note Side-effect: removes the task from `activeTasks`, appends it to
     *       `finishedLog`, and modifies the Task's `status` and `finishTime`.
     * @return void
     */
    void finishTask(int id);

    /**
     * @brief Print all staged tasks to standard output.
     *
     * @note No modifications to internal containers are performed.
     * @return void
     */
    void viewStagedTasks() const;

    /**
     * @brief Print all active tasks to standard output.
     *
     * @note No modifications to internal containers are performed.
     * @return void
     */
    void viewActiveTasks() const;

    /**
     * @brief Print the finished task log including actual durations.
     *
     * @note The actual duration is computed as (finishTime - startTime) and
     *       displayed in seconds and minutes when both timestamps are present.
     * @return void
     */
    void printLog() const;

    /**
     * @brief Helper to find a task by id inside a provided list.
     *
     * @param id Task id to search for.
     * @param list Vector to search in.
     * @return Task* Pointer to the task inside the vector, or nullptr if not found.
     *
     * @note The returned pointer refers to an element inside the given
     *       vector and becomes invalid if that vector is modified (reallocation
     *       or element erasure).
     */
    Task* findTaskById(int id, std::vector<Task>& list);

    /**
     * @brief Get the staged tasks list.
     *
     * @return const std::vector<Task>& Reference to staged tasks.
     */
    const std::vector<Task>& getStagedTasks() const;

    /**
     * @brief Get the active tasks list.
     *
     * @return const std::vector<Task>& Reference to active tasks.
     */
    const std::vector<Task>& getActiveTasks() const;

    /**
     * @brief Get the finished tasks list.
     *
     * @return const std::vector<Task>& Reference to finished tasks.
     */
    const std::vector<Task>& getFinishedTasks() const;

private:
    /**
     * @brief Log a finished task to CSV file.
     *
     * @param task The completed task to log.
     * @note Appends task data to finished_tasks.csv in the current directory.
     * @return void
     */
    void logTaskToCSV(const Task& task);

    /** @brief Tasks waiting to be started. */
    std::vector<Task> stagedTasks;

    /** @brief Tasks currently in progress. */
    std::vector<Task> activeTasks;

    /** @brief Completed task history. */
    std::vector<Task> finishedLog;

    /** @brief Internal counter to generate unique ids. */
    int nextId;
};
