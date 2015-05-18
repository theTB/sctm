#ifndef INFER_CMNT_H
#define INFER_CMNT_H

#include "sctm.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>

void infer_y(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int iter);
void infer_xi(sctm_data* cdata, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);
void infer_t(sctm_data* cdata, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);

#endif
