// Description: Task Scheduler implementation for embedded systems

// Include
// CRT
#include <stdio.h>

// Platform
#ifdef _WIN32
#include <Windows.h>
#elif __linux__

#elif __APPLE__

#endif

// Project
#include "task_scheduler.h"

// type definitions

// data definitions
static TASK_SCHEDULER task_scheduler_instance = { 0 }; // Global instance of the task scheduler
static const int DEFAULT_CYCLE_TIME[TASK_SCHEDULER_MAX_TASKS] = { 20, 40, 100, 200, 500, 1000, 2000, 10000 }; // Default cycle times for tasks

// --------------------------------------------------------------------------------------------
//									   PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------
static void task_init(TASK* task);
static void task_run(TASK* task);
static void task_exit(TASK* task);
static const char* program_state_to_string(PROGRAM_STATE state);
static const char* task_state_to_string(TASK_STATE state);
static void task_scheduler_sim_timer(int milliseconds);

// --------------------------------------------------------------------------------------------
//									   PUBLIC FUNCTIONS
// --------------------------------------------------------------------------------------------
TASK_SCHEDULER* task_scheduler_init(int task_num, int* cycle_time_ms) {
	TASK_SCHEDULER* scheduler = &task_scheduler_instance;
	int _task_num = task_num > TASK_SCHEDULER_MAX_TASKS ? TASK_SCHEDULER_MAX_TASKS : task_num;
	int* _cycle_time_ms = cycle_time_ms ? cycle_time_ms : (int*)DEFAULT_CYCLE_TIME;
	if (scheduler) 
	{
		scheduler->task_count = _task_num;
		scheduler->quit = 0; // Initialize quit flag
		for (int i = 0; i < scheduler->task_count; i++)
		{
			scheduler->tasks[i].state = TASK_STATE_CREATED;
			scheduler->tasks[i].id = i + 1; // Initialize with invalid ID
			int _cycle_time = _cycle_time_ms[i] / MIN_TICK * MIN_TICK;
			scheduler->tasks[i].cyc_time = _cycle_time;
		}
	}
	return scheduler;
}

int task_scheduler_run(TASK_SCHEDULER* scheduler)
{
	int ret = -1;
	TASK_SCHEDULER* inst = &task_scheduler_instance;
	for (int i = 0; i < inst->task_count; i++) 
	{
		TASK* task = &inst->tasks[i];
		if (task->state == TASK_STATE_CREATED) 
		{
			task_init(task); // Initialize the task
		}
		if (task->state == TASK_STATE_INIT) 
		{
			task->state = TASK_STATE_RUNNING; // Run the task
		}
		inst->timer[i] = 0; // Reset timer for each task
	}
	// run timer
	while (!scheduler->quit)
	{
		for (int i = 0; i < inst->task_count; i++) 
		{
			TASK* task = &inst->tasks[i];
			inst->timer[i] += MIN_TICK; // Increment timer by 20 ms
			if (inst->timer[i] >= task->cyc_time) 
			{
				inst->timer[i] = 0; // Reset timer
				if (task->state == TASK_STATE_RUNNING) 
				{
					task_run(task); // Call the run function of the task
				}
			}
		}
		task_scheduler_sim_timer(MIN_TICK); // Wait for the task scheduler cycle time
	}
	return ret;
}

TASK* task_scheduler_get_task(int id) 
{
	TASK_SCHEDULER* inst = &task_scheduler_instance;
	if (id < 1 || id > inst->task_count) 
	{
		return NULL; // Invalid task ID
	}
	return &inst->tasks[id - 1]; // Return the task with the given ID
}

int task_add_program(TASK* task, PROGRAM* program)
{
	if (task && program) 
	{
		if (task->program_count < TASK_MAX_PROGRAMS)
		{
			task->programs[task->program_count] = program; // Add the program to the task
			task->program_count++;
			program->state = PROGRAM_STATE_CREATED; // Set program state to CREATED
			return 0; // Success
		}
		else 
		{
			return -1; // Task has reached maximum number of programs
		}
	}
	return -1; // Invalid task or program
}

int task_remove_program(TASK* task, PROGRAM* program)
{
	if (task && program) 
	{
		for (int i = 0; i < task->program_count; i++) 
		{
			if (task->programs[i] == program) 
			{
				for (int j = i; j < task->program_count - 1; j++) 
				{
					task->programs[j] = task->programs[j + 1]; // Shift programs left
				}
				task->programs[task->program_count - 1] = NULL; // Clear last program
				task->program_count--; // Decrease program count
				program->state = PROGRAM_STATE_STOPPED; // Set program state to STOPPED
				return 0; // Success
			}
		}
	}
	return -1; // Program not found in the task
}

int program_start(PROGRAM* program)
{
	if (program) 
	{
		if (program->state == PROGRAM_STATE_CREATED || program->state == PROGRAM_STATE_STOPPED) 
		{
			if (program->init) 
			{
				program->init(); // Call the init function of the program
			}
			program->state = PROGRAM_STATE_RUNNING; // Set program state to RUNNING
			return 0; // Success
		}
		else 
		{
			return -1; // Program is already running or in error state
		}
	}
	return -1; // Invalid program
}

int program_stop(PROGRAM* program)
{
	if (program) 
	{
		if (program->state == PROGRAM_STATE_RUNNING) 
		{
			if (program->exit) 
			{
				program->exit(); // Call the exit function of the program
			}
			program->state = PROGRAM_STATE_STOPPED; // Set program state to STOPPED
			return 0; // Success
		}
		else 
		{
			return -1; // Program is not running
		}
	}
	return -1; // Invalid program
}

void task_scheduler_info(PTASK_SCHEDULER scheduler)
{
	printf("Task Scheduler, number of tasks: %d\n", scheduler->task_count);
	for (int i = 0; i < scheduler->task_count; i++) 
	{
		TASK* task = &scheduler->tasks[i];
		if (task)
		{
			printf("    ");
			task_info(task); // Print task information
			for (int j = 0; j < task->program_count; j++)
			{
				PROGRAM* program = task->programs[j];
				if (program)
				{
					printf("        ");
					program_info(program); // Print program information
				}
			}
		}
	}
}
void task_info(PTASK task)
{
	printf("Task[%d]: %d ms, %s\n", task->id, task->cyc_time, task_state_to_string(task->state));
	for (int i = 0; i < task->program_count; i++) 
	{
		PROGRAM* program = task->programs[i];
		
	}
}
void program_info(PPROGRAM program)
{
	printf("Program[%d]: %s, %s\n", program->id, program->name ? program->name : "Unnamed", program_state_to_string(program->state));
}

// --------------------------------------------------------------------------------------------
//									   PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------
static void task_scheduler_sim_timer(int milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds);
#elif __linux__
	usleep(milliseconds * 1000); // Convert milliseconds to microseconds
#elif __APPLE__
	usleep(milliseconds * 1000); // Convert milliseconds to microseconds
#endif
}

static void task_init(TASK* task)
{
	if (task)
	{
		for (int i = 0; i < task->program_count; i++) 
		{
			PROGRAM* program = task->programs[i];
			if (program && program->init) 
			{
				program->init(); // Call the init function of the program
			}
		}
		task->state = TASK_STATE_INIT; // Set task state to CREATED
	}
}

static void task_run(TASK* task)
{
	if (task)
	{
		for (int i = 0; i < task->program_count; i++)
		{
			PROGRAM* program = task->programs[i];
			if (program && program->run)
			{
				program->run(); // Call the run function of the program
			}
		}
		task->state = TASK_STATE_RUNNING; // Set task state to RUNNING
	}
}

static void task_exit(TASK* task)
{
	if (task)
	{
		for (int i = 0; i < task->program_count; i++)
		{
			PROGRAM* program = task->programs[i];
			if (program && program->exit)
			{
				program->exit(); // Call the exit function of the program
			}
		}
		task->state = TASK_STATE_STOPPED; // Set task state to STOPPED
	}
}

static const char* program_state_to_string(PROGRAM_STATE state) 
{
	switch (state) 
	{
		case PROGRAM_STATE_CREATED: return "Created";
		case PROGRAM_STATE_INIT: return "Initialized";
		case PROGRAM_STATE_RUNNING: return "Running";
		case PROGRAM_STATE_STOPPED: return "Stopped";
		case PROGRAM_STATE_ERROR: return "Error";
		default: return "Unknown";
	}
}

static const char* task_state_to_string(TASK_STATE state) 
{
	switch (state) 
	{
		case TASK_STATE_CREATED: return "Created";
		case TASK_STATE_INIT: return "Initialized";
		case TASK_STATE_RUNNING: return "Running";
		case TASK_STATE_STOPPED: return "Stopped";
		case TASK_STATE_ERROR: return "Error";
		default: return "Unknown";
	}
}
