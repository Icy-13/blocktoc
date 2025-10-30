#include <math.h>
static struct {
    double output;
    double I_gain;
    double setpoint;
} state;

void state_generated_init() {
}

void state_generated_step() {
    state.I_gain = 2 * state.setpoint ;
    state.output =  state.I_gain ;
    // Update memory blocks
}

typedef struct {
    const char* name;
    double* address;
    int is_input;
} state_ExtPort;

static const state_ExtPort ext_ports[] = {
    {"setpoint", &state.setpoint, 1 },
    {"output", &state.output, 0 },
    { 0, 0, 0 },
};

const state_ExtPort * const state_generated_ext_ports = ext_ports;
const size_t state_generated_ext_ports_size = sizeof(ext_ports);
