#include "infer-z.h"

void infer_z(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int iter, char* odir) {
	int d, i, i1, n, v, j, k, c, no_sample;
	double p, p11, p12, p21, p22, r, beta_sum;
	double* P = (double*) malloc(sizeof(double) * params->K);
	int cnt;
	double p_beta, p_art, p_cmnt;


	for (d = 0; d < data->D; d++) {
		cnt = 0;
		documents* doc = &(data->docs[d]);

		for (i = 0; i < doc->S; i++) {
			sentence* sent = &(doc->sents[i]);
			for (n = 0; n < sent->N; n++) {
				cnt++;
				v = sent->words[n];
				j = latent->b[d][i][n];
				k = latent->z[d][i][n];

				/* If there is a comment token selecting from article with topic k and
				this is the only occurrence of k in doc then this assignment will not change*/

				if (params->CMNTS) {
					no_sample = 0;
					for (c=0; c < doc->C; c++)
						if (latent->xi[d][c][i] == 1 && counts->m[d][c][k]==1) {
							no_sample = 1;
							break;
						}
					if (no_sample == 1) continue;

					for (c = 0; c < doc->C; c++) {
						if (latent->xi[d][c][i] == 1)
							counts->m[d][c][k]--;
					}
				}

				counts->n_dij[k][v] -= 1;
				counts->n_dijv[k] -= 1;
				counts->n_ijv[d][k] -= 1;
				counts->n_jv[d][i][k] -= 1;

				if (counts->n_iv[d][j][k] <= 0) {
					printf("%d, %d, %d, %d", j, doc->J, counts->n_ikv[d][j],
							counts->n_kv[d][i][j]);
					debug("n_iv 0");
				}
				counts->n_iv[d][j][k] -= 1;
				if (counts->n_iv[d][j][k] < 0) {
					debug("counts->n_iv[d][j][k]<0");
				}

				counts->n_ikv[d][j] -= 1;
				beta_sum = 0;
				
				for (k = 0; k < params->K; k++) {

					if (params->trte == 1) {
						p_beta = latent->beta[k][v];
						beta_sum += p_beta;
					}
					else {
						p11 = (double) (latent->phi[k][v] * params->eta
								+ counts->n_dij[k][v]);
						p21 = counts->phiEta_v[k] + counts->n_dijv[k];

						//if (p11 < 0) {
						//	printf("p11 %d", counts->n_dij[k][v]);
						//	debug("in z");
						//}
						//if (p21 <= 0) {
						//	printf("p21 phi_eta:%lf c	ounts:%d", counts->phiEta_v[k], counts->n_dijv[k]);
						//	debug("in z");
						//}
						p_beta = p11 / p21;
					}
					p12 = (double) (params->alpha
							+ counts->n_iv[d][j][k]);
					p22 = params->K*params->alpha + counts->n_ikv[d][j];

					//if (p12 < 0) {
					//	printf("p12 %d", counts->n_iv[d][j][k]);
					//	debug("in z");
					//}
					//if (p22 < 0) {
					//	printf("p22 %d", counts->n_ikv[d][j]);
					//	debug("in z");
					//}

					if (p12 == 0 && params->word_sparsity == 1) {
						p_art = 0;
						if (k > 0) P[k] = P[k-1] + p_art;
						else P[k] = p_art;
						continue;
					}
					else
						p_art = p_beta * (p12 / p22);

					p = p_art;
					if (isinf(p_art) && params->word_sparsity == 0) {
						printf("p11:%lf p12:%lf p21:%lf p22:%lf",p11,p12,p21,p22);
						debug("chk");
					}
					//p_cmnt = 0;
					p_cmnt = 1.0;
					if (params->CMNTS) {
						for (c = 0; c < doc->C; c++) {
							if (latent->xi[d][c][i] == 0) continue;
							if (counts->m_1[d][c][k] == 0) continue;
							//p_cmnt += counts->m_u[d][c][k] * (log((counts->m[d][c][k] + 1) * 1.0) -log(counts->m[d][c][k]*1.0));
							p_cmnt *= pow((counts->m[d][c][k] + 1) * 1.0/counts->m[d][c][k], counts->m_1[d][c][k]);
							if (isnan(p_cmnt) || isinf(p_cmnt)) {
								//assignment(odir, data, params, latent, counts, -1);
								printf("d:%d c:%d C:%d i:%d n:%d k:%d m:%d m_1:%d m_k:%d p_cmnt:%lf\n",
										d,c,doc->C,i,n,k,counts->m[d][c][k],counts->m_1[d][c][k], counts->m_k[d][c], p_cmnt);
								debug("chk");
							}
						}

						if (isnan(p_cmnt) || isinf(p_cmnt)) {
							debug("error in p_cmnt");
						}

					}
					p *= p_cmnt;


					if (isnan(p)) {
						printf("p:%lf,  p_beta:%lf, p_art:%lf, p_cmnt:%lf\n",
								p, p_beta, p_art, p_cmnt);
						debug("error in z");
					}

					if (k==0) P[k] = p;
					else P[k] = P[k-1] + p;

					if (isnan(P[k]) || isinf(P[k])) {
						printf("k:%d v:%d p:%lf,  p_beta:%lf, p_art:%lf, p_cmnt:%lf P[k]:%lf\n",
								k, v, p, p_beta, p_art, p_cmnt, P[k]);
						debug("error in z");
					}
				}
				//printf("k:%d p_beta:%lf\n",k, latent->beta[k][v]);
				//debug("chk");
				r = myrand();
				r *= P[params->K - 1];
				for (k = 0; k < params->K; k++) {
					if (r < P[k])
						break;
				}
				if (k == params->K) {
					if (params->trte == 1) {
						k = floor(myrand()*params->K);
					}
					else {
						printf("K:%d, P[K-1]:%lf trte:%d\n", params->K, P[params->K-1], params->trte);
						debug("in z: error in probs: k==K");
					}
				}

				latent->z[d][i][n] = k;

				counts->n_dij[k][v] += 1;
				counts->n_dijv[k] += 1;
				counts->n_ijv[d][k] += 1;
				counts->n_jv[d][i][k] += 1;

				j = latent->b[d][i][n];
				counts->n_iv[d][j][k] += 1;

				counts->n_ikv[d][j] += 1;

				if (params->CMNTS) {
					for (i1 = 0; i1 < doc->C; i1++) {
						if (latent->xi[d][i1][i] == 1)
							counts->m[d][i1][k]++;
					}
				}

			} //n
		} //i


	} //d
	free(P);
}

