#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "sctm.h"
#include "utils.h"

sctm_data* read_data(char* art_fname, char* cmnt_fname, sctm_params* params);
void free_data(sctm_data* data);
void load_beta(char* odir, sctm_data* data, sctm_params* params, sctm_latent* latent);

#endif
