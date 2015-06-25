#include "infer-cmnt.h"

void infer_y(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts, int iter) {
	int d, i, n, v, k, t;
	double p, r;
	double* P = (double*) malloc(sizeof(double) * (params->K + 1));

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (i = 0; i < doc->C; i++) {
			comment* cmnt = &(doc->cmnts[i]);
			for (n = 0; n < cmnt->N; n++) {
				v = cmnt->words[n];
				k = latent->y[d][i][n];
				t = latent->t[d][i][n];
				if (t == 1) { //counts only changes when token is sampling from article
					counts->n_dij[k][v]--;
					counts->n_dijv[k]--;
					counts->m_1[d][i][k]--;
				}

				for (k = 0; k < params->K; k++) {
					p = 1.;
					if (params->trte == 1 && t == 1) p = latent->beta[k][v];
					else if (t == 1) {						
						p = (double) (latent->phi[k][v] * params->eta + counts->n_dij[k][v]) /
							(counts->phiEta_v[k] + counts->n_dijv[k]);
					}
					p *= counts->m[d][i][k] *1.0 / counts->m_k[d][i];
					if (counts->m[d][i][k]==0) {
						p = 0.;
					}
					if (isnan(p)) {
						printf("k:%d,p:%f \n", k, p);
						printf("p1:: phi:%d eta:%f counts->n:%d\np2:: counts->phiEta:%f counts->n:%d",
								latent->phi[k][v], params->eta,
								counts->n_dij[k][v], counts->phiEta_v[k],
								counts->n_dijv[k]);
						debug("error in infer-cmnt 1");
					}

					P[k] = 0;
					if (k == 0)
						P[k] = p;
					else
						P[k] = P[k - 1] + p;
					if (isnan(P[k])) {
						printf("k:%d v:%d p:%f t:%d m:%d m_k:%d beta:%lf\n", k, v, p,t,counts->m[d][i][k],counts->m_k[d][i], latent->beta[k][v]);
						debug("error in infer-y");
					}

				}

				r = myrand();
				if (P[params->K - 1] < 1e-15 || r == 0) {
					printf("d:%d i:%d n:%d, P[K]:%lf\n", d,i,n,P[params->K-1]);
					//for (k=0; k < params->K; k++) printf("m[d][i][%d]:%d\n",k, counts->m[d][i][k]);
					//printf("m_k[d][i]:%d\n",counts->m_k[d][i]);
					//fflush(stdout);
					debug("error in infer-y: all probs zero");
				}
				r *= P[params->K - 1];
				for (k = 0; k < params->K; k++) {
					if (r <= P[k])
						break;
				}
				//if (k >= params->K) {
				//	printf("\nK:%d, r:%f, P[K-1]:%f, p:%f", k, r,
				//			P[params->K - 1], p);
				//	fflush(stdout);
				//	debug("in infer-cmt");
				//}

				latent->y[d][i][n] = k;
				if (t==1) {
					counts->n_dij[k][v]++;
					counts->n_dijv[k]++;
					counts->m_1[d][i][k]++;
				}
			}
		}
	}
	free(P);

}


void infer_xi(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts) {
	int d, i, a, k, n;
	double p1, p2, p11, p22, norm;

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (i = 0; i < doc->C; i++) {
			comment* cmnt = &(doc->cmnts[i]);
			for (a = 0; a < doc->S; a++) {
				sentence* sent = &(doc->sents[a]);

				p1 = 0.;
				p2 = 0.;
				for (n = 0; n < cmnt->N; n++) {
					k = latent->y[d][i][n];
//					if (latent->t[d][i][n] == 0)
//						continue;
					if (latent->xi[d][i][a] == 1) {
						p11 = log(counts->m[d][i][k] * 1.0) - log(counts->m_k[d][i]*1.0);
						// if only article sentence (among selected) in which k topic occurs:
						if (counts->m_k[d][i] - sent->N == 0) p22 = -INFINITY;
						else p22 = log((counts->m[d][i][k] - counts->n_jv[d][a][k]) * 1.0)
								 - log((counts->m_k[d][i] - sent->N)*1.0);
					} else {
						p11 = log((counts->m[d][i][k] + counts->n_jv[d][a][k]) * 1.0)
								- log((counts->m_k[d][i] + sent->N)*1.0);
						// if only article sentence (among selected) in which y topic occurs:
						if (counts->m_k[d][i] == 0) p22 = -INFINITY;
						else p22 = log(counts->m[d][i][k] * 1.0) - log(counts->m_k[d][i]*1.0);
					}
					p1 += p11;
					p2 += p22;
					if (isinf(p2) && !isinf(p1)) break;
				}

				p1 += log(params->vr1 * 1.0) - log((params->vr1 + params->vr2)*1.0);
				p2 += log(params->vr2 * 1.0) - log((params->vr1 + params->vr2)*1.0);

				norm = 0;
				if (isinf(p2) && p2 < 0) {
					p1 = 1.0;
					p2 = 0.0;
				} else{
					norm = log_sum(p1,p2);
					p1 -= norm;
					p1 = exp(p1);
				}

				if (isnan(p1) || isinf(p1) || isinf(norm)) {
					printf("\nd:%d i:%d a:%d p1:%lf p2:%lf\n",d,i,a,p1,p2);
					debug("incorrect probs: infer_xi");
				}

				if (myrand() <= p1) {
					if (latent->xi[d][i][a] == 0) {
						for (k=0; k < params->K; k++)
							counts->m[d][i][k] += counts->n_jv[d][a][k];
						counts->m_k[d][i] += sent->N;
					}
					latent->xi[d][i][a] = 1;
				}
				else {
					if (latent->xi[d][i][a] == 1) {
						for (k=0; k < params->K; k++)
							counts->m[d][i][k] -= counts->n_jv[d][a][k];
						counts->m_k[d][i] -= sent->N;
					}
					latent->xi[d][i][a] = 0;
				}
				latent->xi_prob[d][i][a] = p1;
			} //a
		} //i
	} //d
}

void infer_t(sctm_data* data, sctm_params* params, sctm_latent* latent,
		sctm_counts* counts) {
	int d, c, k, n, v;
	double p01, p02, p11, p12, p1, p0;

	for (d = 0; d < data->D; d++) {
		documents* doc = &(data->docs[d]);
		for (c = 0; c < doc->C; c++) {
			comment* cmnt = &(doc->cmnts[c]);
			for (n = 0; n < cmnt->N; n++) {
				k = latent->y[d][c][n];
				v = cmnt->words[n];
				if (latent->t[d][c][n] == 1) {
					counts->m_1[d][c][k] -= 1;
					counts->m_1k[d][c] -= 1;
					counts->n_dij[k][v] -= 1;
					counts->n_dijv[k] -= 1;
				} else {
					counts->n_dij[params->K][v] -= 1;
					counts->n_dijv[params->K] -= 1;
				}
				p11 = params->lambda_1 + counts->m_1k[d][c];
				p01 = params->lambda_2 + (cmnt->N - counts->m_1k[d][c] - 1);

				if (params->trte==1) {
					p12 = latent->beta[k][v];
					p02 = latent->beta[params->K][v];
				}
				else {
					p12 = (latent->phi[k][v]*params->eta + counts->n_dij[k][v])
							/ (counts->phiEta_v[k] + counts->n_dijv[k]);
					p02 = (params->eta + counts->n_dij[params->K][v])
									/ (data->V*params->eta + counts->n_dijv[params->K]);
				}

				p1 = p11*p12;
				p0 = p01*p02;

				if (isnan(p1+p0) || isinf(p1+p0)) debug("error in probs: infer_t");

				p1 /= (p1+p0);

				if (myrand() <= p1) {
					counts->m_1[d][c][k] += 1;
					counts->m_1k[d][c] += 1;
					latent->t[d][c][n] = 1;
					counts->n_dij[k][v] += 1;
					counts->n_dijv[k] += 1;
				} else {
					counts->n_dij[params->K][v] += 1;
					counts->n_dijv[params->K] += 1;
					latent->t[d][c][n] = 0;
				}

			} //n
		} //c
	} //d
}
