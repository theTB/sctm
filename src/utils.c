#include "utils.h"
extern gsl_rng * RANDOM_NUMBER;

void softmax(const double* a, int t, int len, double* out) {
	// softmax of inputs
	// a: input array
	// t: softmax parameter
	// len: length of input
	// out: output array
	int i;
	double max_a, norm;
	max_a = a[0];
	for (i = 1; i < len; i++) if (a[i] > max_a) max_a = a[i];
	norm = 0;
	for (i = 1; i < len; i++) {
		out[i] = exp(t*(a[i]-max_a));
		norm += out[i];
	}
	for (i = 1; i < len; i++) out[i] /= norm;
}

double vdot_dd(double* a, double* b, int len) {
	/*
	 * vector dot product (a^T b), where a and b are of size len
	 * result is stored in out
	 */
	int i;
	double out = 0;
	for (i=0; i < len; i++) {
		out += a[i]*b[i];
	}
	return out;
}

void vsum(double* a, double* b, double* out, int len) {
	/*
	 * vector sum (a+b), where a and b are of size len
	 * result is stored in out
	 */
	int i;
	for (i=0; i < len; i++) {
		out[i] = a[i]+b[i];
	}
}

void vaxpy(double* a, const double* b, double alpha, int len) {
	/*
	 * vector addition a = a + \alpha* b, where a and b are of size len
	 * result is stored in out
	 */
	int i;
	for (i=0; i < len; i++) {
		a[i] = a[i] + alpha*b[i];
	}
}


gsl_rng * random_generator(long seed) {
  gsl_rng * rgen = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set(rgen, (long) seed); // init the seed

  return rgen;
}

void free_random_generator(gsl_rng * random_number_generator) {
  gsl_rng_free(random_number_generator);
}

int randint(int n) {
	/*
	 * returns a random integer in [0,n-1]
	 */
	if (n <= 0) debug("invalid input to randint");
	unsigned long int r;
	r = gsl_rng_uniform_int(RANDOM_NUMBER, (int)n);
	if (r > INT_MAX) debug("problem in randint");
	return (int)r;
}



void rdirichlet(double alpha, int* n, int K, double* theta) {
	//alpha - symmetric hyperparameter
	//n - integer hyperparameter offsets (can be posterior counts)
	int k;
	double* a = (double*) malloc(sizeof(double)*K);
	for (k=0; k < K; k++) a[k] = alpha + n[k];
	gsl_ran_dirichlet (RANDOM_NUMBER, (size_t)K, a, theta);
	free(a);
}

double runiform(double a, double b) {
	// Returns uniform random variable on (a,b)
	//return (a + (b-a)*gsl_rng_uniform(RANDOM_NUMBER));
	return (a + (b-a)*gsl_rng_uniform_pos(RANDOM_NUMBER));
}

double sigmoid(double x) {
	double l;
	if (x < 0) {
		l = exp(x);
		return (l/(1+l));
	}
	l = exp(-x);
	return (1.0/(1+l));
}

double norm_2(double* x, int K, int sq) {
	//K: size of x
	//sq: 1 if norm^square is required
	int k;
	double norm=0;
	for (k=0; k < K; k++) norm += x[k]*x[k];
	if (sq != 1) norm = pow(norm, 0.5);
	return (norm);

}

double norm_diff_2(double* x, double* y, int K, int sq) {
	//K: size of x & y
	//sq: 1 if norm^square is required
	int k;
	double norm = 0;
	//printf("x:");
	//for (k=0; k < K; k++) printf("%lf ",x[k]);
	for (k=0; k < K; k++) norm += (x[k]-y[k])*(x[k]-y[k]);
	if (sq != 1) norm = pow(norm, 0.5);
	//printf("norm:%lf\n",norm); fflush(stdout);
	//debug("chk");
	return (norm);
}

/*
 * given log(a) and log(b), return log(a + b)
 *
 */

double log_sum(double log_a, double log_b) {
	double v;

	if (log_a < log_b) {
		v = log_b + log(1 + exp(log_a - log_b));
	} else {
		v = log_a + log(1 + exp(log_b - log_a));
	}
	return (v);
}

/**
 * Proc to calculate the value of the trigamma, the second
 * derivative of the loggamma function. Accepts positive matrices.
 * From Abromowitz and Stegun.  Uses formulas 6.4.11 and 6.4.12 with
 * recurrence formula 6.4.6.  Each requires workspace at least 5
 * times the size of X.
 *
 **/

double trigamma(double x) {
	double p;
	int i;

	x = x + 6;
	p = 1 / (x * x);
	p =
			(((((0.075757575757576 * p - 0.033333333333333) * p
					+ 0.0238095238095238) * p - 0.033333333333333) * p
					+ 0.166666666666667) * p + 1) / x + 0.5 * p;
	for (i = 0; i < 6; i++) {
		x = x - 1;
		p = 1 / (x * x) + p;
	}
	return (p);
}

/*
 * taylor approximation of first derivative of the log gamma function
 *
 */

double digamma(double x) {
	//return(digammal(x));
	double p;
	x = x + 6;
	p = 1 / (x * x);
	p = (((0.004166666666667 * p - 0.003968253986254) * p + 0.008333333333333)
			* p - 0.083333333333333) * p;
	p = p + log(x) - 0.5 / x - 1 / (x - 1) - 1 / (x - 2) - 1 / (x - 3)
			- 1 / (x - 4) - 1 / (x - 5) - 1 / (x - 6);
	return p;

}

double log_gamma(double x) {

	return (lgamma(x));
	/*
	 double z=1/(x*x);

	 x=x+6;
	 z=(((-0.000595238095238*z+0.000793650793651)
	 *z-0.002777777777778)*z+0.083333333333333)/x;
	 z=(x-0.5)*log(x)-x+0.918938533204673+z-log(x-1)-
	 log(x-2)-log(x-3)-log(x-4)-log(x-5)-log(x-6);
	 return z;
	 */
}

/*
 * make directory
 *
 */

void make_directory(char* name) {
	mkdir(name, S_IRUSR | S_IWUSR | S_IXUSR);
}

/*
 * argmax
 *
 */

int argmax(double* x, int n) {
	int i;
	double max = x[0];
	int argmax = 0;
	for (i = 1; i < n; i++) {
		if (x[i] > max) {
			max = x[i];
			argmax = i;
		}
	}
	return (argmax);
}

int argmin(int* x, int n) {
	int i;
	double min = x[0];
	int argmin = 0;
	for (i = 1; i < n; i++) {
		if (x[i] < min) {
			min = x[i];
			argmin = i;
		}
	}
	return (argmin);
}

double getmin(double* x, int n) {
	int i;
	double min = x[0];
	for (i = 1; i < n; i++) {
		if (x[i] < min) {
			min = x[i];
		}
	}
	return (min);
}

double mod(double x) {
	if (x < 0)
		x *= -1;
	return (x);
}
int myfloor(double x) {
	int y = (int) x;
	return (y);
}
int myceiling(double x) {
	int y = (int) x + 1;
	return (y);
}
int myround(double x) {
	if ((x - floor(x)) > 0.5)
		return (myceiling(x));
	else
		return (myfloor(x));
}

void normalize(double* x, int d) {
	double sum = 0;
	int i;
	for (i = 0; i < d; i++) {
		sum += x[i];
	}
	for (i = 0; i < d; i++) {
		x[i] /= sum;
	}

}

void l2_normalize(double* x, int d) {
	double sum = 0;
	int i;
	for (i = 0; i < d; i++) {
		sum += x[i] * x[i];
	}
	if (sum == 0) {
		printf("err in l2 norm\n");
		exit(1);
	}
	for (i = 0; i < d; i++) {
		x[i] /= sqrt(sum);
	}

}

void exp_normalize(double* x, int d) {
	double sum = 0;
	int i;
	for (i = 0; i < d; i++) {
		sum += exp(x[i]);
	}
	for (i = 0; i < d; i++) {
		x[i] = exp(x[i]) / sum;
	}

}

double getmean(double* x, int d) {
	int i;
	double sum = 0;
	for (i = 0; i < d; i++)
		sum += x[i];

	return (sum / (double) d);
}

void standardize(double* x, int d) {
	int i;
	double mean = getmean(x, d);
	for (i = 0; i < d; i++)
		x[i] -= mean;
}

void shift(double* x, int d) {
	int i;
	double min = getmin(x, d);
	for (i = 0; i < d; i++)
		x[i] -= min;
}

void debug(const char* msg) {
	printf("\n%s\npress Enter to resume...", msg);
	getchar();
}

double Abs(double x) {
	if (x < 0)
		return (x * -1);
	else
		return x;
}
