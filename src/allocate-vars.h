#ifndef ALLOCATE_H
#define ALLOCATE_H

#include "sctm.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>

#define myrand1() 1
#define myrand0() 0

void allocate_vars(sctm_data* data, sctm_params* params,
		sctm_latent* latent, sctm_counts* counts);

void free_vars(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);


#endif
