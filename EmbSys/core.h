#pragma once

#define RUN_CYCLIC_OBJECT(obj) \
    do { \
        if ((obj)->cObject.run) { \
            (obj)->cObject.run(obj); \
        } \
    } while(0)

struct cyclic_object {
	int cycT;
	void (*init)(struct cyclic_object* inst);
	void (*run)(struct cyclic_object* inst);
	void (*exit)(struct cyclic_object* inst);
};

