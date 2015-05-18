#include "allocate-vars.h"

void allocate_vars(sctm_data* data, sctm_params* params,
		sctm_latent* latent, sctm_counts* counts) {

	int i, j, k, d, v, n, c;
	int K = params->K;
	int V = data->V;

	counts->phi_k = (int*) malloc(sizeof(int) * V);
	for (v = 0; v < data->V; v++)
		counts->phi_k[v] = 0;
	counts->phiEta_v = (double*) malloc(sizeof(double) * (K+1));

	counts->n_dijv = (int*) malloc(sizeof(int) * (K+1));  //+1 for noise topic
	counts->n_dij = (int**) malloc(sizeof(int*) * (K+1));
	for (k = 0; k < K+1; k++) {
		counts->n_dijv[k] = 0;
		counts->n_dij[k] = (int*) malloc(sizeof(int) * V);
		for (v = 0; v < V; v++)
			counts->n_dij[k][v] = 0;
	}

	counts->n_ikv = (int**) malloc(sizeof(int*) * data->D);
	counts->n_iv = (int***) malloc(sizeof(int**) * data->D);
	counts->n_jv = (int***) malloc(sizeof(int**) * data->D);
	counts->n_ijv = (int**) malloc(sizeof(int*) * data->D);

	counts->n_jkv = (int**) malloc(sizeof(int*) * data->D);
	counts->n_kv = (int***) malloc(sizeof(int**) * data->D);
	counts->n_kvc = (int***) malloc(sizeof(int**) * data->D);


	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);

		counts->n_jkv[d] = (int*) malloc(sizeof(int) * doc->S);
		counts->n_jv[d] = (int**) malloc(sizeof(int*) * doc->S);
		counts->n_kv[d] = (int**) malloc(sizeof(int*) * doc->S);
		counts->n_kvc[d] = (int**) malloc(sizeof(int*) * doc->S);

		counts->n_iv[d] = (int**) malloc(sizeof(int*) * data->docs[d].J);
		counts->n_ikv[d] = (int*) malloc(sizeof(int) * data->docs[d].J);

		counts->n_ijv[d] = (int*) malloc(sizeof(int) * K);

		for (j = 0; j < data->docs[d].J; j++) {
			counts->n_iv[d][j] = (int*) malloc(sizeof(int) * K);
			counts->n_ikv[d][j] = 0;
			for (k = 0; k < K; k++)
				counts->n_iv[d][j][k] = 0;
		}

		for (i = 0; i < doc->S; i++) {
			counts->n_kv[d][i] = (int*) malloc(sizeof(int) * data->docs[d].J);
			counts->n_kvc[d][i] = (int*) malloc(sizeof(int) * data->docs[d].J);
			counts->n_jkv[d][i] = 0; // n_jkv[d][i]
			for (j = 0; j < data->docs[d].J; j++) {
				counts->n_kv[d][i][j] = 0;
				counts->n_kvc[d][i][j] = 0;
			}
			counts->n_jv[d][i] = (int*) malloc(sizeof(int) * K);
			for (k=0; k < params->K; k++) counts->n_jv[d][i][k] = 0;
		}

		for (k = 0; k < K; k++)
			counts->n_ijv[d][k] = 0;
	}


	counts->m = (int***) malloc(sizeof(int**) * data->D);
	counts->m_k = (int**) malloc(sizeof(int*) * data->D);
	counts->m_1 = (int***) malloc(sizeof(int**) * data->D);
	counts->m_1k = (int**) malloc(sizeof(int*) * data->D);

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		counts->m[d] = (int**) malloc(sizeof(int*) * doc->C);
		counts->m_k[d] = (int*) malloc(sizeof(int) * doc->C);
		counts->m_1[d] = (int**) malloc(sizeof(int*) * doc->C);
		counts->m_1k[d] = (int*) malloc(sizeof(int) * doc->C);
		for (i = 0; i < doc->C; i++) {
			counts->m[d][i] = (int*) malloc(sizeof(int) * K);
			counts->m_1[d][i] = (int*) malloc(sizeof(int) * K);
			for (k = 0; k < K; k++) {
				counts->m[d][i][k] = 0;
				counts->m_1[d][i][k] = 0;
			}
			counts->m_k[d][i] = 0;
			counts->m_1k[d][i] = 0;
		}
	}

	latent->z = (int***) malloc(sizeof(int**) * data->D);
	latent->b = (int***) malloc(sizeof(int**) * data->D);
	latent->y = (int***) malloc(sizeof(int**) * data->D);
	latent->t = (int***) malloc(sizeof(int**) * data->D);
	latent->xi = (int***) malloc(sizeof(int**) * data->D);
	latent->xi_prob = (double***) malloc(sizeof(double**) * data->D);

	latent->y_dist = (double***) malloc(sizeof(double**) * data->D);
	latent->z_dist = (double***) malloc(sizeof(double**) * data->D);

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		latent->z[d] = (int**) malloc(sizeof(int*) * doc->S);
		latent->b[d] = (int**) malloc(sizeof(int*) * doc->S);
		latent->z_dist[d] = (double**) malloc(sizeof(double*) * doc->S);
		for (i = 0; i < doc->S; i++) {
			sentence* sent = &(doc->sents[i]);
			latent->z[d][i] = (int*) malloc(sizeof(int) * sent->N);
			latent->b[d][i] = (int*) malloc(sizeof(int) * sent->N);
			latent->z_dist[d][i] = (double*) malloc(sizeof(double) * params->K);
			for (n = 0; n < sent->N; n++) {
				latent->z[d][i][n] = -1;
				latent->b[d][i][n] = -1;
			}
			for (k=0; k < params->K; k++) latent->z_dist[d][i][k] = 0;
		}


		latent->y[d] = (int**) malloc(sizeof(int*) * doc->C);
		latent->t[d] = (int**) malloc(sizeof(int*) * doc->C);
		latent->xi[d] = (int**) malloc(sizeof(int*) * doc->C);
		latent->xi_prob[d] = (double**) malloc(sizeof(double*) * doc->C);
		latent->y_dist[d] = (double**) malloc(sizeof(double*) * doc->C);

		for (c = 0; c < doc->C; c++) {
			comment* cmnt = &(doc->cmnts[c]);
			latent->y[d][c] = (int*) malloc(sizeof(int) * cmnt->N);
			latent->t[d][c] = (int*) malloc(sizeof(int) * cmnt->N);
			for (n = 0; n < cmnt->N; n++) {
				latent->y[d][c][n] = -1;
				latent->t[d][c][n] = -1;
			}
			latent->xi[d][c] = (int*) malloc(sizeof(int) * doc->S);
			latent->xi_prob[d][c] = (double*) malloc(sizeof(double) * doc->S);
			for (i = 0; i < doc->S; i++) {
				latent->xi[d][c][i] = -1;
				latent->xi_prob[d][c][i] = 0;
			}
			latent->y_dist[d][c] = (double*) malloc(sizeof(double) * (params->K+1));
			for (k=0; k < params->K+1; k++) latent->y_dist[d][c][k] = 0.;
		}

	}



	latent->phi = (int**) malloc(sizeof(int*)*(K+1));
	latent->beta = (double**) malloc(sizeof(double*)*(K+1));
	for (k=0; k < K+1; k++) {
		latent->phi[k] = (int*) malloc(sizeof(int)* V);
		latent->beta[k] = (double*) malloc(sizeof(double)* V);
		for (v=0; v < V; v++) {
			latent->phi[k][v] = 0;
			latent->beta[k][v] = 0;
		}
	}



}



void free_vars(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts) {
	int d, k, j, c;

	free(counts->phi_k);
	free(counts->phiEta_v);

	for (k = 0; k < params->K+1; k++) {
		free(counts->n_dij[k]);
		free(latent->phi[k]);
		free(latent->beta[k]);
	}
	free(counts->n_dij);
	free(latent->phi);
	free(latent->beta);

	free(counts->n_dijv);

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);

		for (j = 0; j < data->docs[d].J; j++) {
			free(counts->n_iv[d][j]);
		}
		for (j = 0; j < doc->S; j++) {
			free(counts->n_kv[d][j]);
			free(counts->n_kvc[d][j]);
			free(counts->n_jv[d][j]);

			free(latent->z[d][j]);
			free(latent->b[d][j]);
			free(latent->z_dist[d][j]);
		}

		free(counts->n_ikv[d]);
		free(counts->n_jkv[d]);
		free(counts->n_jv[d]);

		free(counts->n_iv[d]);
		free(counts->n_kv[d]);
		free(counts->n_kvc[d]);
		free(counts->n_ijv[d]);

		free(latent->z[d]);
		free(latent->b[d]);
		free(latent->z_dist[d]);

		for (c = 0; c < doc->C; c++) {
			free(latent->y[d][c]);
			free(latent->t[d][c]);
			free(latent->xi[d][c]);
			free(latent->xi_prob[d][c]);
			free(latent->y_dist[d][c]);
		}
		free(latent->y[d]);
		free(latent->t[d]);
		free(latent->xi[d]);
		free(latent->xi_prob[d]);
		free(latent->y_dist[d]);
	}

	free(counts->n_jkv);
	free(counts->n_kv);
	free(counts->n_kvc);
	free(counts->n_ikv);
	free(counts->n_iv);
	free(counts->n_jv);
	free(counts->n_ijv);

	free(latent->z);
	free(latent->b);
	free(latent->y);
	free(latent->t);
	free(latent->xi);
	free(latent->xi_prob);
	free(latent->y_dist);
	free(latent->z_dist);

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (j = 0; j < doc->C; j++) {
			free(counts->m[d][j]);
			free(counts->m_1[d][j]);
		}
		free(counts->m[d]);
		free(counts->m_1[d]);
		free(counts->m_k[d]);
		free(counts->m_1k[d]);
	}
	free(counts->m);
	free(counts->m_1);
	free(counts->m_k);
	free(counts->m_1k);

	free(counts);
	free(latent);
}


