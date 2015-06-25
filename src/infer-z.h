#ifndef INFER_Z_H
#define INFER_Z_H

#include "sctm.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>

void infer_z(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int iter, char* odir);

#endif
