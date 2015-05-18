#ifndef SCTM_H
#define SCTM_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef struct {
	//Sparsity Variables
	double *phiEta_v; // phi*eta (sum over v)
	int *phi_k; // phi (sum over k)
	//Notation: d=doc, i=sentence, j=topic_vector, k=topic, v=vocab_word

	// article related counts
	int **n_dij, *n_dijv; // # v assigned to k & sum over v
	int ***n_iv, **n_ikv; // # topic k used from theta_j in doc d & sum over k
	int ***n_jv; // # topic k assigned in sent i in doc d
	int **n_ijv; // # topic k assigned in doc d
	int ***n_kv, ***n_kvc, **n_jkv; // # theta_j used in sent i in doc d and sum over j of all l>=j (refer to sbp eqn)

	//comments related counts
	int ***m, **m_k; // m[d][c][k] # topic k in doc d for comment c consider only selected sents & sum over k

	int ***m_1, //m_u[d][c][k] # tokens in comment c on doc d assigned topic k from article (i.e. with t_dci = 1)
		**m_1k; //sum over k of m_1

} sctm_counts;

typedef struct {
	int ***z, //doc,sent,word
		***b, //doc,sent,word
		***y, //doc, cmnt, word
		***t, //doc, cmnt, word
		***xi, //doc, cmnt, sent
		**phi; //topic, word

	double **beta, //topic, word
			***xi_prob, //doc, cmnt, sent
			***y_dist, //doc, cmnt, topic
			***z_dist; //doc, sent, topic
} sctm_latent;

typedef struct {
	int ITER, save_step, show_step, comp_likelihood, burn_in, save_state;
	double alpha, eta, delta, lambda, mu, nu;
	double nu_lambda, mu_delta;

	double b_tau, b_iota;
	double kappa, xi_thr; //initialization
	int word_sparsity, sents_sparsity, CMNTS, trte;
	int IJ, K;
	double vr1, vr2; //xi
	double ro1, ro2; //for beta prior on noise topic
	double lambda_1, lambda_2; //prior for epsilon (user/article selector)

	double eps; //for initialization of epsilon_c -> t
	int model;  // 0: LDA  1: CorrLDA 2: SCTM

} sctm_params;

typedef struct {
	int N;
	int* words;
} sentence;

typedef struct {
	int N; //N: number of words
	int* words;
} comment;

typedef struct {
	int N, S, J, C; //S:sentences, C:comments
	sentence* sents;
	comment* cmnts;
} documents;


typedef struct {
	int D, V, C; //V:vocab size
	documents* docs;
} sctm_data;

#endif
