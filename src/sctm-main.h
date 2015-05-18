#ifndef SCTM_MAIN_H
#define SCTM_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "sctm.h"
#include "data.h"
#include "params.h"
#include "allocate-vars.h"
#include "inivars.h"
#include "infer.h"
#include "utils.h"
#include "assignment.h"

void gibbs(char* data, char* cmntf, char* odir, int trte, int topics, char* model);

#endif
