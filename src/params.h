#ifndef PARAMS_H
#define PARAMS_H

#include "sctm.h"

sctm_params* get_params(int trte, int topics, char* model);
void free_params(sctm_params* params);

#endif
