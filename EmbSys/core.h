#pragma once

struct cyclic_object {
	int cycT;
	void (*init)(struct cyclic_object* inst);
	void (*run)(struct cyclic_object* inst);
	void (*exit)(struct cyclic_object* inst);
};