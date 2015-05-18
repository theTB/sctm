#include "infer-b.h"

void infer_b(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts) {
	int d, i, n, j, k, l;
	double p, p1, p2, p4, r;
	int p3;
	double* P; //int sum;
	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		P = (double*) malloc(sizeof(double) * doc->J);
		for (i = 0; i < doc->S; i++) {
			sentence* sent = &(doc->sents[i]);
			for (n = 0; n < sent->N; n++) {
				j = latent->b[d][i][n];
				k = latent->z[d][i][n];

				counts->n_iv[d][j][k] -= 1;
				counts->n_ikv[d][j] -= 1;
				counts->n_kv[d][i][j]--;
				for (l = 0; l <= j; l++)
					counts->n_kvc[d][i][l]--;

				p4 = 1;
				for (j = 0; j < doc->J; j++) {

					p1 = params->alpha
							+ counts->n_iv[d][j][k];
					p2 = params->K*params->alpha + counts->n_ikv[d][j];
					if (p1 == 0)
						p = 0;
					else
						p = p1 / p2;

					if (j < (doc->J - 1)) {

						p3 = counts->n_kvc[d][i][j];

						p1 = (params->b_tau + counts->n_kv[d][i][j])
								/ (params->b_tau + params->b_iota + p3);

						p1 *= p4;
						p4 *= (1 - p1);
						//p5 += p1;
					} else {

						p3 = counts->n_kvc[d][i][j];
						p1 = p4;

						if (p1 > 1 || p1 < 0) {
							debug("in trunc b 2 err");
						}
						//p5 += p1;
					}

					p *= p1;
					if (j == 0)
						P[j] = p;
					else
						P[j] = P[j - 1] + p;

					if (p > 1 || p < 0 || isnan(p)) {
						printf("%d, %lf", counts->n_kv[d][i][j], p);
						debug("in infer b error in p");
					}
				}

				r = myrand();
				r *= P[doc->J - 1];
				for (j = 0; j < doc->J; j++) {
					if (r <= P[j])
						break;
				}

				latent->b[d][i][n] = j;

				counts->n_iv[d][j][k] += 1;
				counts->n_ikv[d][j] += 1;
				counts->n_kv[d][i][j]++;

				for (l = 0; l <= j; l++)
					counts->n_kvc[d][i][l]++;

			} //n

		} //i

		free(P);
	} //d
}

