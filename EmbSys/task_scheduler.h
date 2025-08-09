#pragma once

// task_scheduler.h
// Description: Task Scheduler header file for embedded systems
// System - [tasks] (fixed cycle time)
// Task - [programs]

#define TASK_SCHEDULER_MAX_TASKS 8 // Maximum number of tasks in the scheduler
#define MIN_TICK 20

#define TASK_MAX_PROGRAMS 20 // Maximum number of programs per task

// --------------------------------------------------------------------------------------------
//										CMD INTERACTION
// --------------------------------------------------------------------------------------------
/*
*	status
*	task-info -id <task_id>
*	program-info -name <program_name>
*   program-start -name <program_name>
*	program-stop -name <program_name>
* 
* 
* 
*/

// --------------------------------------------------------------------------------------------
//										TYPE DEFINITIONS
// --------------------------------------------------------------------------------------------
typedef enum {
	PROGRAM_STATE_UNKNOWN = 0, // Program is not initialized
	PROGRAM_STATE_CREATED = 1, // Program is created
	PROGRAM_STATE_INIT = 2, // Program is initialized
	PROGRAM_STATE_RUNNING = 3, // Program is currently running
	PROGRAM_STATE_STOPPED = 4, // Program has been stopped by external request
	PROGRAM_STATE_ERROR = 5  // Program encountered an error
} PROGRAM_STATE;

typedef struct program_t {
	int id; // Program identifier
	const char* name; // Program name
	PROGRAM_STATE state; // Current state of the program
	void (*init)(void); // Pointer to initialization function
	void (*run)(void);  // Pointer to run function
	void (*exit)(void); // Pointer to exit function, do cleanup
} PROGRAM, * PPROGRAM;

typedef enum {
	TASK_STATE_UNKNOWN = 0, // Task is not initialized
	TASK_STATE_CREATED = 1,	// Task is created
	TASK_STATE_INIT	   = 2,	// Task is initialized
	TASK_STATE_RUNNING = 3,	// Task is currently running
	TASK_STATE_STOPPED = 4,	// Task has been stopped by external request
	TASK_STATE_ERROR   = 5		// Task encountered an error
} TASK_STATE;

typedef struct task_t {
	int id; // Task identifier, priority
	int cyc_time; // Cyclic time in milliseconds
	TASK_STATE state; // Current state of the task
	int tolerance_time; // Tolerance time in milliseconds
	int program_count; // Number of programs associated with this task
	PROGRAM* programs[TASK_MAX_PROGRAMS]; // Array of programs associated with this task
} TASK, * PTASK;

typedef struct task_scheduler_t {
	int task_count; // Number of tasks in the scheduler
	TASK tasks[TASK_SCHEDULER_MAX_TASKS]; // Array of tasks
	int timer[TASK_SCHEDULER_MAX_TASKS];
	volatile int quit; // Flag to indicate if the scheduler should quit
} TASK_SCHEDULER, * PTASK_SCHEDULER;

// --------------------------------------------------------------------------------------------
//									   PUBLIC FUNCTIONS
// --------------------------------------------------------------------------------------------
PTASK_SCHEDULER task_scheduler_init(int task_num, int* cycle_time_ms); // Init the singleton task scheduler instance
int task_scheduler_run(TASK_SCHEDULER* scheduler); // Run the task scheduler
TASK* task_scheduler_get_task(int id); // Get a task by ID


int task_add_program(TASK* task, PROGRAM* program); // Add a program to a task by task ID
int task_remove_program(TASK* task, PROGRAM* program); // Remove a program from a task by program ID

int program_start(PROGRAM* program); // Start a program
int program_stop(PROGRAM* program); // Stop a program

void task_scheduler_info(PTASK_SCHEDULER scheduler); // Print task scheduler information
void task_info(PTASK task); // Print task information
void program_info(PPROGRAM program); // Print program information
