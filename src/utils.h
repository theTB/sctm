#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <execinfo.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_errno.h>
#include <limits.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

//#include "digamma.h"

#define myrand() runiform(0,1)

void softmax(const double* a, int t, int len, double* out);
double vdot_dd(double* a, double* b, int len);
void vsum(double* a, double* b, double* out, int len);
void vaxpy(double* a, const double* b, double alpha, int len);
gsl_rng * random_generator(long seed);
void free_random_generator(gsl_rng * random_number_generator);
void rdirichlet(double alpha, int* n, int K, double* theta);
double runiform(double a, double b);
int randint(int n);


double norm_2(double* x, int K, int sq);
double norm_diff_2(double* x, double* y, int K, int sq);
double sigmoid(double x);

double log_sum(double log_a, double log_b);
double trigamma(double x);
double digamma(double x);
double log_gamma(double x);
void make_directory(char* name);
int argmax(double* x, int n);
int argmin(int* x, int n);
//double min(double* x, int n);
double mod(double);
int myfloor(double x);
int myceiling(double x);
int myround(double x);
void normalize(double* x, int d);
void exp_normalize(double* x, int d);
double mean(double* x, int d);
void standardize(double* x, int d);
void shift(double* x, int d);
void l2_normalize(double* x, int d);

void debug(const char* msg);
double Abs(double);
#endif
