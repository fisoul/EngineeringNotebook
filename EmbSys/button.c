#include <stdbool.h>
#include "button.h"

void button_run(struct button_t* inst)
{
	switch (inst->state)
	{
		// idle
	case 0:
		inst->pressed = false;
		inst->p_edge = inst->n_edge = false;
		inst->timer = 0;
		if (inst->raw_input)
		{
			inst->state = 1;
		}
		break;
		// filter
	case 1:
		if (inst->raw_input)
		{
			inst->timer += inst->cObject.cycT;
			if (inst->timer >= inst->filterTime)
			{
				inst->pressed = true;
				inst->p_edge = true;
				inst->n_edge = false;
				inst->state = 2;
			}
		} 
		else
		{
			inst->timer = 0;
			inst->state = 0;
		}
		break;
		// pressed
	case 2:
		if (!inst->raw_input)
		{
			inst->p_edge = false;
			inst->n_edge = true;
			inst->pressed = false;
			inst->state = 0;
		}
		else
		{
			inst->pressed = true;
			inst->p_edge = false;
			inst->n_edge = false;
		}
		break;
	}
}