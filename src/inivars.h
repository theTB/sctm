#ifndef INIVARS_H
#define INIVARS_H

#include "sctm.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>


void ini_vars(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);

void ini_phi(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);

void ini_b_z(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);

void ini_y_xi_t(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);

#endif
