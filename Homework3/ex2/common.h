#pragma once

#include <string>
#include <cmath>


typedef void(*comp_func)(double* x, double* y, double* z, int n);
void add_function(comp_func f, std::string name, int flop);