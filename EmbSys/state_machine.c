

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

State state, state_prev;
void app_run_10ms()
{
	state = getDesiredState();
	if (state != state_prev)
	{
		exitActionSM();
		state_prev = state;
		entryActionSM();
	}
	runActionSM();
}