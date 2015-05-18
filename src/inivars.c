#include "inivars.h"

void ini_vars(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int trte) {

	if (params->model == 4) return;

	ini_phi(data, params, latent, counts);

	ini_b_z(data, params, latent, counts, trte);

	if (params->CMNTS)
		ini_y_xi_t(data, params, latent, counts, trte);

}

void ini_phi(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts) {
	int k, v, sum;

	for (k = 0; k < params->K; k++) {
		counts->phiEta_v[k] = 0;
		sum = 0;
		for (v = 0; v < data->V; v++) {
			if (params->word_sparsity > 0) {
				if (myrand() <= params->kappa)
					latent->phi[k][v] = 1;
				else
					latent->phi[k][v] = 0;
			} else
				latent->phi[k][v] = 1;

			counts->phi_k[v] += latent->phi[k][v];
			counts->phiEta_v[k] += latent->phi[k][v] * params->eta;
			sum += latent->phi[k][v];
		}
		if (sum == 0)
			debug("in ini_phi sum 0");
	}
}

void ini_b_z(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int trte) {
	int d, i, n, v, j, k, l;
	double p, r, *P, p11, p12, p21, p22;
	int sum;
	P = (double*) malloc(sizeof(double) * params->K);

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (i = 0; i < doc->S; i++) {
			sentence* sent = &(doc->sents[i]);
			for (n = 0; n < sent->N; n++) {
				v = sent->words[n];

				counts->n_jkv[d][i]++;

				if (params->model == 2) {
					r = myrand();
					p = r * doc->J;
					j = myfloor(p);
					if (r > 1 || j >= doc->J)
						debug("in ini vars for j");
				} else j=0;


				latent->b[d][i][n] = j;

				counts->n_ikv[d][j]++;
				counts->n_kv[d][i][j]++;

				for (k = 0; k < params->K; k++) {
					p11 = latent->phi[k][v] * params->eta + counts->n_dij[k][v];
					p21 = counts->phiEta_v[k] + counts->n_dijv[k];

					p12 = params->alpha
							+ counts->n_iv[d][j][k];
					p22 = params->K*params->alpha + counts->n_ikv[d][j];

					if (p12 == 0 || p11 == 0)
						p = 0;
					else
						p = p11 / p21 * p12 / p22;

					if (isnan(p) || isinf(p))
						debug("in ini z");

					if (k > 0)
						P[k] = P[k - 1] + p;
					else
						P[k] = p;
				}
				r = myrand();
				r *= P[params->K - 1];

				if (isnan(r)) {
					printf("%lf, %lf\n", r, P[params->K - 1]);
					fflush(stdout);
				}

				for (k = 0; k < params->K; k++)
					if (r <= P[k])
						break;

				latent->z[d][i][n] = k;
				j = latent->b[d][i][n];

				counts->n_dij[k][v]++;
				counts->n_iv[d][j][k]++;
				counts->n_dijv[k]++;
				counts->n_ijv[d][k]++;
				counts->n_jv[d][i][k]++;

			} //n
			for (j = 0; j < doc->J; j++) {
				counts->n_kvc[d][i][j] = 0;
				for (l = j; l < doc->J; l++)
					counts->n_kvc[d][i][j] += counts->n_kv[d][i][l];
			}

		} //i

		for (j = 0; j < doc->J; j++) {
			sum = 0;
			for (k = 0; k < params->K; k++) {
				sum += counts->n_iv[d][j][k];
			}
			if (sum != counts->n_ikv[d][j]) {
				printf("%d, %d, %d", j, sum, counts->n_ikv[d][j]);
				debug("in ini vars for sum counts->n_ikv[d][j]");
			}
		}

	} //d

	for (k = 0; k < params->K; k++) {
		sum = 0; //printf("%d",model->V); debug("chk");
		for (v = 0; v < data->V; v++)
			sum += counts->n_dij[k][v];
		if (sum != counts->n_dijv[k]) {
			printf("%d, %d, %d", k, sum, counts->n_dijv[k]);
			debug("in ini vars for sum counts->n_dimjv[k]");
		}
	}

	free(P);

}

void ini_y_xi_t(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int trte) {
	int d, i, a, n, k, v, sum_xi;
	double r, p1, p2, p;

	double* P = (double*) malloc(sizeof(double) * params->K);
	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (i = 0; i < doc->C; i++) {
			comment* cmnt = &(doc->cmnts[i]);
			sum_xi = 0;
			for (a = 0; a < doc->S; a++) {
				if (params->sents_sparsity) {
					if (myrand() <= params->xi_thr || (a==doc->S-1 && sum_xi==0)) {
						latent->xi[d][i][a] = 1;
						sum_xi++;
					}
					else
						latent->xi[d][i][a] = 0;
				}
				else
					latent->xi[d][i][a] = 1;

				if (latent->xi[d][i][a] == 1) {
					sentence* sent = &(doc->sents[a]);
					for (n = 0; n < sent->N; n++) {
						k = latent->z[d][a][n];
						counts->m[d][i][k]++;
						counts->m_k[d][i]++;
					}
				}
			}
			for (n = 0; n < cmnt->N; n++) {
				v = cmnt->words[n];
				p = 1;
				p1 = 1;
				p2 = 1;
				for (k = 0; k < params->K; k++) {
					p1 = (double) (latent->phi[k][v] * params->eta
							+ counts->n_dij[k][v]); ////
					p2 = counts->phiEta_v[k] + counts->n_dijv[k]; ////
					p = p1 / p2;
					p1 = (double) counts->m[d][i][k];
					p2 = (double) counts->m_k[d][i];
					if (p2 != 0)
						p *= (p1 / p2);
					else if (p2 == 0) p = 0;
					P[k] = 0;
					if (isnan(p)) {
						printf("k:%d,p1:%f,p2:%f,p:%f", k, p1, p2, p);
						fflush(stdout);
						debug("in ini-vars");
					}
					if (k == 0)
						P[k] = p;
					else
						P[k] = P[k - 1] + p;
				}

				r = myrand();
				r *= P[params->K - 1];
				for (k = 0; k < params->K; k++) {
					if (r <= P[k])
						break;
				}
				latent->y[d][i][n] = k;
				if (myrand() < params->eps) {
					latent->t[d][i][n] = 0;
					k = params->K;
				} else {
					latent->t[d][i][n] = 1;
					counts->m_1[d][i][k]++;
					counts->m_1k[d][i]++;
				}

				counts->n_dij[k][v]++;
				counts->n_dijv[k]++;


			} // n
		} // i
	}
	free(P);
}
