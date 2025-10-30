#include <math.h>
static struct {
    double out;
    double in3;
    double Add3;
    double in2;
    double in1;
} state;

void state_generated_init() {
}

void state_generated_step() {
    state.Add3 = + state.in1 - state.in2 - state.in3 ;
    state.out =  state.Add3 ;
    // Update memory blocks
}

typedef struct {
    const char* name;
    double* address;
    int is_input;
} state_ExtPort;

static const state_ExtPort ext_ports[] = {
    {"in1", &state.in1, 1 },
    {"in2", &state.in2, 1 },
    {"in3", &state.in3, 1 },
    {"out", &state.out, 0 },
    { 0, 0, 0 },
};

const state_ExtPort * const state_generated_ext_ports = ext_ports;
const size_t state_generated_ext_ports_size = sizeof(ext_ports);
