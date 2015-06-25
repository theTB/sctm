#include "params.h"
#include <stdio.h>

sctm_params* get_params(int trte, int topics, char* modelName) {

	sctm_params* params = (sctm_params*) malloc(sizeof(sctm_params));

	params->trte = trte;

	params->sents_sparsity = 1;

	params->ITER = 500;
	params->burn_in = 400;
	params->save_state = 10; //interval between saved states
	params->save_step = 10;
	

	// Sparsity params
	params->delta = 1e-3;
	params->lambda = 1e4;
	params->mu = 1e0;
	params->nu = 1e-3;

	params->nu_lambda = params->nu * params->lambda;
	params->mu_delta = params->mu * params->delta;

	// b
	params->b_tau = 1e-2;
	params->b_iota = 1e-1;

	// theta, beta
	params->K = topics;
	params->alpha = 0.01;
	params->eta = 1;

	// comments
	params->vr1 = 1;
	params->vr2 = 4; //1
	params->lambda_1 = 10.; //t prior
	params->lambda_2 = 1.;

	// initialization thresholds
	params->kappa = 1.0;
	params->xi_thr = 1.0;
	params->eps = 0.5;

	params->ro1 = 1;
	params->ro2 = 9;

	if (strcmp("lda", modelName) == 0) {
		printf("\nLDA");
		params->model = 0;
		params->CMNTS = 0;
		params->IJ = 1;
		params->word_sparsity = 1;  // set this to 1 for sparse topics (0 for not)
	}
	else if (strcmp("corrlda", modelName) == 0) {
		printf("\nCorrespondence LDA");
		params->model = 1;
		params->CMNTS = 1;
		params->sents_sparsity = 0;
		params->IJ = 1;
		params->word_sparsity = 1; // set this to 1 for sparse topics (0 for not)
	}
	else if (strcmp("sctm", modelName) == 0) {
		printf("\nSpecific Correspondence Topic Model");
		params->model = 2;
		params->CMNTS = 1;
		params->sents_sparsity = 1;
		params->IJ = 5;  // set the number of topic vectors (J) here. Value of 0 means J=number of sentences
		params->word_sparsity = 1;
	}
	else {
		printf("\nUnrecognised %s model, Options are: lda, corrlda, sctm\n\n",modelName);
		exit(0);
	}

	return (params);

}

void free_params(sctm_params* params) {
	free(params);
}
