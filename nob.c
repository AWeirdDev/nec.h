#define NOB_IMPLEMENTATION
#include "vendor/nob.h"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    nob_log(NOB_INFO, "building shit");
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_inputs(&cmd, "./main.c");
    nob_cc_output(&cmd, "./main");
    if (!nob_cmd_run(&cmd)) return 1;

    nob_log(NOB_INFO, "running shit");
    nob_cmd_append(&cmd, "./main");
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
