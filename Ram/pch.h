#pragma once

#include "Grammar\Statement.h"

#define PRINT_ENVS 1
#define PRINT_VALS 0
#define PRINT_LINES 0

#define PRINT_VAL(val, execEnv) { if(PRINT_VALS) std::cout << val->ToString() << std::endl; }
#define PRINT_ENV(env) { if(PRINT_ENVS) std::cout << std::endl << std::endl << env.ToString() << std::endl; }
#define PRINT_LINE(line) { if(PRINT_LINES) std::cout << GetSrcLine(line) << std::endl; }