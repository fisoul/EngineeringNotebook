
struct moveParameters_t {
	float velocity;
	float acc;
	float dec;
	float jerk;
};

enum position_generator_state_e {
	PG_STATE_IDLE = 0,
	PG_STATE_ACTIVE = 1
};	

typedef struct setPointGenerator_t {
	int targetPosition;
	struct moveParameters_t moveParameters;
	int active;
	int position;
	float velocity;
} setPointGenerator;

void setPointGenerator_run(setPointGenerator* inst)
{
	switch (inst->active) {
		case PG_STATE_IDLE:
			// Idle state logic
			break;
		case PG_STATE_ACTIVE:
			
			break;
		default:
			// Handle unexpected state
			break;
	}
}

