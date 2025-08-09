#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "button.h"

bool simInput[] = { false, true, true, true, false, true, true, false, false, false };

int main(int argc, const char* argv[])
{
	printf("Hello, Embedded Systems!\n");
	struct button_t button1 = {   
		.cObject = { .cycT = 50 }, // Example cyclic time
		.state = 0,
		.raw_input = false,
		.pressed = false,
		.p_edge = false,
		.n_edge = false,
		.filterTime = 100, // Example filter time
		.timer = 0
	};
	button1.cObject.init = NULL; // Initialize if needed
	button1.cObject.run = button_run;
	int count = sizeof(simInput) / sizeof(simInput[0]);
	int index = 0;
	while (1)
	{
		button1.raw_input = simInput[index];
		button1.cObject.run(&button1);
		printf("Button State: %d, Input: %d, Pressed: %d, P-Edge: %d, N-Edge: %d\n",
			button1.raw_input, button1.state, button1.pressed, button1.p_edge, button1.n_edge);
		if (++index >= count)
		{
			break;
		}
		Sleep(50);
	}
	return 0;
}

typedef enum {
	STATE_INIT,
	STATE_RUNNING,
	STATE_ERROR
} State;

static State getDesiredState()
{
	// This function would typically contain logic to determine the next state
	// For this example, we will just return STATE_RUNNING
	return STATE_RUNNING;
}

static void runActionSM()
{
	
}

static void entryActionSM()
{

}

static void exitActionSM()
{

}

void app_run_10ms()
{

	getDesiredState();

}
