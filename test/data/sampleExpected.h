#include <math.h>
static struct {
    double command;
    double Unit_Delay1;
    double Ts;
    double P_gain;
    double I_gain;
    double Add3;
    double Add2;
    double Add1;
    double feedback;
    double setpoint;
} state;

void state_generated_init() {
    state.Unit_Delay1 = 0.0;
}

void state_generated_step() {
    state.Add1 = + state.setpoint - state.feedback ;
    state.P_gain = 3 * state.Add1 ;
    state.I_gain = 2 * state.Add1 ;
    state.Ts = 0.01 * state.I_gain ;
    state.Add2 = + state.Unit_Delay1 + state.Ts ;
    state.Add3 = + state.Add2 + state.P_gain ;
    state.command =  state.Add3 ;
    // Update memory blocks
    state.Unit_Delay1 =  state.Add2 ;
}

typedef struct {
    const char* name;
    double* address;
    int is_input;
} state_ExtPort;

static const state_ExtPort ext_ports[] = {
    {"setpoint", &state.setpoint, 1 },
    {"feedback", &state.feedback, 1 },
    {"command", &state.command, 0 },
    { 0, 0, 0 },
};

const state_ExtPort * const state_generated_ext_ports = ext_ports;
const size_t state_generated_ext_ports_size = sizeof(ext_ports);
