#pragma once

#include <string>
#include "quaternion.h"

typedef void(*comp_func)(quaternion_t x[N],  double y[N]);
void add_function(comp_func f, std::string name, int flop);
