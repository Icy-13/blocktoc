#include <math.h>
static struct {
    double command;
    double Unit_Delay2;
    double Unit_Delay1;
    double Add;
    double in1;
} state;

void state_generated_init() {
    state.Unit_Delay1 = 0.0;
    state.Unit_Delay2 = 0.0;
}

void state_generated_step() {
    state.Add = + state.in1 + state.Unit_Delay1 ;
    state.command =  state.Unit_Delay2 ;
    // Update memory blocks
    state.Unit_Delay1 =  state.Unit_Delay2 ;
    state.Unit_Delay2 =  state.Add ;
}

typedef struct {
    const char* name;
    double* address;
    int is_input;
} state_ExtPort;

static const state_ExtPort ext_ports[] = {
    {"in1", &state.in1, 1 },
    {"command", &state.command, 0 },
    { 0, 0, 0 },
};

const state_ExtPort * const state_generated_ext_ports = ext_ports;
const size_t state_generated_ext_ports_size = sizeof(ext_ports);
