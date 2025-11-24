# Simple Job Scheduler (C++ implementation)
> Made by - Subhajit Das (60) & Saksham Kumar Gupta (50)
---

This project implements a console-based Job Scheduler using C++ to manage the lifecycle of tasks. Tasks transition through three distinct states: **Staged** (waiting to start), **Active** (in progress), and **Finished** (completed).

The application is built using a modular object-oriented design, separating core entities (`Task`) from the management logic (`Scheduler`) and the user interface (`main.cpp`).

---

## Project Structure

The codebase is organized into three main C++ components:

1.  **`Task.h`:** Defines the `Task` data structure and its `Status` enum.
2.  **`Scheduler.h` / `Scheduler.cpp`:** Defines and implements the `Scheduler` class, which handles all task management and state transitions.
3.  **`main.cpp`:** Provides the interactive console menu for the user.

---

## Core Components and Functions

### 1. Task Class (in `Task.h`)

This class represents a single unit of work, along with its description, time estimates, and actual timing data.

| Function Name | Description |
| :--- | :--- |
| `Task(int id, const std::string& description, int estimate)` | Constructor to create a new task in the **Staged** state. |
| `void markActive()` | Sets the task's status to **Active** and records the `startTime`. |
| `void markFinished()` | Sets the task's status to **Finished** and records the `finishTime`. |
| `std::string getDetails() const` | Returns a formatted string with all task details for display. |

### 2. Scheduler Class (in `Scheduler.h` and `Scheduler.cpp`)

This class manages the three internal lists of tasks (`stagedTasks`, `activeTasks`, `finishedLog`) and handles all state transitions.

| Function Name | Description |
| :--- | :--- |
| `Scheduler()` | Constructor. Initializes the internal task ID counter (`nextId`) to 1. |
| `void addTask(const std::string& description, int estimate)` | Creates a new task and adds it to the **Staged** list. |
| `void startTask(int id)` | Finds the task by ID in the **Staged** list, marks it as active, and moves it to the **Active** list. |
| `void finishTask(int id)` | Finds the task by ID in the **Active** list, marks it as finished, and moves it to the **Finished Log**. |
| `void viewStagedTasks() const` | Prints all tasks currently in the **Staged** list. |
| `void viewActiveTasks() const` | Prints all tasks currently in the **Active** list. |
| `void printLog() const` | Prints the **Finished Log**, including a calculation of the actual duration (finish time minus start time). |
| `Task* findTaskById(int id, std::vector<Task>& list)` | Private utility function to locate a task within a specified vector by its ID. |

### 3. `main.cpp`

The application entry point, responsible for running the main menu loop and managing user input.

| Function Name | Description |
| :--- | :--- |
| `int main()` | Initializes the `Scheduler` and runs the main command loop, handling user choices (1-6) and input validation. |

---

## Usage Instructions

1.  **Compile:** Compile all three source files (`main.cpp`, `Scheduler.cpp`, `Task.h`) together.
2.  **Run:** Execute the compiled application.
3.  **Menu:** Follow the on-screen menu prompts:
    * Use option `1` to **Add** a new task.
    * Use options `2` or `3` to change a task's status using its unique **ID**.
    * Use options `4`, `5`, or `6` to **View** the tasks in each state.
    * Use option `0` to **Exit**.