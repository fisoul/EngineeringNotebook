#pragma once

#include "core.h"

struct button_t {
	struct cyclic_object cObject;
	int state;
	bool raw_input;
	bool pressed;
	bool p_edge;
	bool n_edge;
	int filterTime;
	int timer;
};

void button_run(struct button_t* inst);

