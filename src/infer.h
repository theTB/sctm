#ifndef INFER_H
#define INFER_H

#include "sctm.h"
#include "utils.h"
#include "assignment.h"
#include "infer-z.h"
#include "infer-b.h"
#include "infer-cmnt.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>

#define myrand1() 1
#define myrand0() 0

void infer(char* odir, sctm_data* data, sctm_params* params,
		sctm_latent* latent, sctm_counts* counts);

void infer_phi(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts);


#endif
