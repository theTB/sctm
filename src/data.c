#include "data.h"

sctm_data* read_data(char* art_fname, char* cmnt_fname, sctm_params* params) {
	FILE *art, *cmnt;
	int d, D, s, S, c, C, n, N, w;
	int V = 0; //vocab size

	sctm_data* data = (sctm_data*) malloc(sizeof(sctm_data));

	art = fopen(art_fname, "r");

	if (!fscanf(art, "%d", &D))
		debug("error reading");
	data->D = D;
	data->docs = (documents*) malloc(sizeof(documents) * D);

	for (d = 0; d < D; d++) {
		if (!fscanf(art, "%d", &S))
			debug("error reading");
		data->docs[d].S = S; //no. of sentences
		data->docs[d].N = 0;

		if (params->IJ > 0)
			data->docs[d].J = params->IJ;
		else {
			data->docs[d].J = S;
		}

		data->docs[d].sents = (sentence*) malloc(sizeof(sentence) * S);
		for (s = 0; s < S; s++) {
			if (!fscanf(art, "%d", &N))
				debug("error reading");
			data->docs[d].sents[s].N = N;
			data->docs[d].N += N;
			data->docs[d].sents[s].words = (int*) malloc(sizeof(int) * N);
			for (n = 0; n < N; n++) {
				if (!fscanf(art, "%d", &w))
					debug("error reading");
				data->docs[d].sents[s].words[n] = w;

				if (w >= V)
					V = w + 1;
			}
		}

		data->docs[d].cmnts = NULL;
		data->docs[d].C = 0;
	}

	fclose(art);
	data->C = 0;
	data->V = V;
	if (params->CMNTS == 0) return data;

	cmnt = fopen(cmnt_fname, "r");
	if (!fscanf(cmnt, "%d", &D)) debug("error reading cmnts");

	for (d = 0; d < D; d++) {
		if (!fscanf(cmnt, "%d", &C))
			debug("error reading"); //read num comments
		data->docs[d].C = C;
		data->C += C;
		data->docs[d].cmnts = (comment*) malloc(sizeof(comment) * C);
		for (c = 0; c < C; c++) {
			if (!fscanf(cmnt, "%d", &N))
				debug("error reading");
			data->docs[d].cmnts[c].N = N;
			data->docs[d].cmnts[c].words = (int*) malloc(sizeof(int)*N);
			for (n = 0; n < N; n++) {
				if (!fscanf(cmnt, "%d", &w))
					debug("error reading");
				data->docs[d].cmnts[c].words[n] = w;
				if (w >= V)
					V = w + 1;
			}
		}
	}
	fclose(cmnt);
	data->V = V;

	return (data);
}

void load_beta(char* odir,  sctm_data* data, sctm_params* params, sctm_latent* latent) {

	char fname[500];
	int k, v, V, K;
	double b, beta_sum;
	double eps = 1e-6; // for words with 0 probability in all documents

	sprintf(fname,"%s/beta",odir);
	FILE *f = fopen(fname,"r");
	if(!fscanf(f, "%d %d", &K, &V)) debug("error reading beta");
	if (params->model == 0) params->K = K;
	else params->K = K - 1;
	data->V = V;
	double* beta_norm = (double*) malloc(sizeof(double)*K);
	latent->beta = (double**) malloc(sizeof(double*)*K);
	for (k = 0; k < K; k++) {
		latent->beta[k] = (double*) malloc(sizeof(double)*data->V);
		beta_norm[k] = 0;
		for(v=0; v < V; v++){
			if(!fscanf(f,"%lf ",&b)) debug("error reading beta");
			if(isnan(b)){
				printf("%d, %d", k, v); debug("beta nan");
			}
			latent->beta[k][v] = b;
			beta_norm[k] += b;
		}
	}
	fclose(f);
	//for (k=0; k < params->K; k++) printf("k:%d norm:%lf\n",k,beta_norm[k]);
	//debug("chk");
	for (v = 0; v < V; v++) {
		beta_sum = 0;
		for(k=0; k < params->K; k++) beta_sum += latent->beta[k][v];
		if (beta_sum < 1e-6) {
			for(k=0; k < params->K; k++) {
				latent->beta[k][v] += eps;
				beta_norm[k] += eps;
			}
		}
	}
	
	for (k = 0; k < K; k++) for (v=0; v < V; v++) latent->beta[k][v] /= beta_norm[k];
	free(beta_norm);
	
}

void free_data(sctm_data* data) {
	int d, s, c;
	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (s = 0; s < doc->S; s++) {
			sentence* sent = &(doc->sents[s]);
			free(sent->words);
		}
		free(doc->sents);
		for (c = 0; c < doc->C; c++) {
			comment* cmnt = &(doc->cmnts[c]);
			free(cmnt->words);
			//free(cmnt->replies);
		}
		free(doc->cmnts);
	}
	free(data->docs);
	free(data);
}
