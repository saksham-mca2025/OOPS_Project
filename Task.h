#pragma once

#include <string>
#include <ctime>

/**
 * @file Task.h
 * @brief Task value type and lifecycle status for the Scheduler.
 */

/**
 * @enum Status
 * @brief Represents the lifecycle status of a Task.
 *
 * @note Typical progression is Staged -> Active -> Finished.
 */
enum class Status {
    /** @brief Task is created but not yet started. */
    Staged,
    /** @brief Task has been started and is in progress. */
    Active,
    /** @brief Task has been completed. */
    Finished
};

/**
 * @class Task
 * @brief Represents a single schedulable job with timing and descriptive information.
 *
 * @details Task is a simple value type stored by the Scheduler. It keeps
 *          identification, textual description, estimated duration, and
 *          wall-clock timestamps for start/finish.
 */
class Task {
public:
    /**
     * @brief Construct a new Task object.
     *
     * @param id Unique identifier to assign to this task.
     * @param description Human-readable description of the task.
     * @param estimate Estimated duration in seconds.
     *
     * @note Side-effect: initializes internal status to Status::Staged and
     *       start/finish times to zero.
     */
    Task(int id, const std::string& description, int estimate);

    /**
     * @brief Mark the task as active and record start time.
     *
     * @note Side-effect: sets `status` to Status::Active and records
     *       `startTime` with std::time(nullptr).
     * @return void
     */
    void markActive();

    /**
     * @brief Mark the task as finished and record finish time.
     *
     * @note Side-effect: sets `status` to Status::Finished and records
     *       `finishTime` with std::time(nullptr).
     * @return void
     */
    void markFinished();

    /**
     * @brief Return a human readable detail string for the task.
     *
     * @details The returned string includes id, description, status, estimate,
     *          and formatted start/finish times when available.
     *
     * @return std::string Formatted details for display.
     */
    std::string getDetails() const;

    /** @brief Unique identifier for the task. */
    int id;

    /** @brief Description of what the task does. */
    std::string description;

    /** @brief Current lifecycle status of the task. */
    Status status;

    /** @brief Start time recorded when task was started (0 if not started). */
    std::time_t startTime;

    /** @brief Finish time recorded when task was completed (0 if not finished). */
    std::time_t finishTime;

    /** @brief Estimated duration in seconds provided by the user. */
    int estimatedDurationSeconds;
};

// -------------------------
// Inline implementations
// -------------------------

inline Task::Task(int id_, const std::string& description_, int estimate)
    : id(id_), description(description_), status(Status::Staged), startTime(0), finishTime(0), estimatedDurationSeconds(estimate) {}

inline void Task::markActive() {
    status = Status::Active;
    startTime = std::time(nullptr);
}

inline void Task::markFinished() {
    status = Status::Finished;
    finishTime = std::time(nullptr);
}

inline std::string Task::getDetails() const {
    std::string statusStr;
    switch (status) {
        case Status::Staged: statusStr = "Staged"; break;
        case Status::Active: statusStr = "Active"; break;
        case Status::Finished: statusStr = "Finished"; break;
    }

    std::string result = "[#" + std::to_string(id) + "] " + description + " | Status: " + statusStr;
    result += " | Estimate: " + std::to_string(estimatedDurationSeconds) + " sec";

    if (startTime != 0) {
        char* s = std::ctime(&startTime); // ctime returns newline-terminated string
        std::string startStr = s ? std::string(s) : std::string("N/A");
        if (!startStr.empty() && startStr.back() == '\n') startStr.pop_back();
        result += " | Start: " + startStr;
    } else {
        result += " | Start: N/A";
    }

    if (finishTime != 0) {
        char* s = std::ctime(&finishTime);
        std::string finishStr = s ? std::string(s) : std::string("N/A");
        if (!finishStr.empty() && finishStr.back() == '\n') finishStr.pop_back();
        result += " | Finish: " + finishStr;
    } else {
        result += " | Finish: N/A";
    }

    return result;
}
