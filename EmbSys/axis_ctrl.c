#include <stdint.h>
#include <float.h>

struct encoder_if_t {
	uint8_t count_dir;
	uint32_t units;
	uint32_t motor_rev;
};
struct controller_t {
	float kp;
};


struct axis_t {
	struct encoder_if_t encoder_if;
};