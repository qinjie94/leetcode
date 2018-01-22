/*
 * randgen.c
 *
 *  Modified on: May 1, 2009
 *   Adapted by: Mo Sadoghi
 *
 *
 *
 *      Generator         Range (x)     Mean         Variance
 *
 *      Bernoulli(p)      x = 0,1       p            p*(1-p)
 *      Binomial(n, p)    x = 0,...,n   n*p          n*p*(1-p)
 *      Equilikely(a, b)  x = a,...,b   (a+b)/2      ((b-a+1)*(b-a+1)-1)/12
 *      Geometric(p)      x = 0,...     p/(1-p)      p/((1-p)*(1-p))
 *      Pascal(n, p)      x = 0,...     n*p/(1-p)    n*p/((1-p)*(1-p))
 *      Poisson(m)        x = 0,...     m            m
 *
 * and seven continuous distributions
 *
 *      Uniform(a, b)     a < x < b     (a + b)/2    (b - a)*(b - a)/12
 *      Exponential(m)    x > 0         m            m*m
 *      Erlang(n, b)      x > 0         n*b          n*b*b
 *      Normal(m, s)      all x         m            s*s
 *      Lognormal(a, b)   x > 0            see below
 *      Chisquare(n)      x > 0         n            2*n
 *      Student(n)        all x         0  (n > 1)   n/(n - 2)   (n > 2)
 *
 * For the a Lognormal(a, b) random variable, the mean and variance are
 *
 *                        mean = exp(a + 0.5*b*b)
 *                    variance = (exp(b*b) - 1) * exp(2*a + b*b)
 *
 * Name              : rvgs.c  (Random Variate GeneratorS)
 * Author            : Steve Park & Dave Geyer
 * Language          : ANSI C
 * Latest Revision   : 10-28-98
 * --------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "randgen.h"


double random_gen() {
	double  randseed = (double)rand () / (double) RAND_MAX;
	return randseed;
}

double random_exp_gen(double m) {
	double  randseed = exponential(m) / (double) RAND_MAX;;
	return randseed;
}


long bernoulli(double p){
	return ((random_gen() < (1.0 - p)) ? 0 : 1);
}


long binomial(long n, double p){
	long i, x = 0;

	for (i = 0; i < n; i++)
		x += bernoulli(p);
	return (x);
}


long equilikely(long a, long b){
	return (a + (long) ((b - a) * random_gen()));
}

long equilikely_exp(long a, long b, double m){
	return (a + (long) ((b - a) * exponential(m)));
}


long geometric(double p){
	return ((long) (log(1.0 - random_gen()) / log(p)));
}


long pascal(long n, double p){
	long i, x = 0;

	for (i = 0; i < n; i++)
		x += geometric(p);
	return (x);
}


long poisson(double m){
	double t = 0.0;
	long x = 0;

	while (t < m) {
		t += exponential(1.0);
		x++;
	}
	return (x - 1);
}



uint32_t zipf_first_instance(double alpha, uint32_t n){
  static int first = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (first == TRUE)
  {
    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, alpha));
    c = 1.0 / c;
    first = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = uniform(0,1);
  }
  while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, alpha);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}


uint32_t zipf_second_instance(double alpha, uint32_t n){
  static int second = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (second == TRUE)
  {
    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, alpha));
    c = 1.0 / c;
    second = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = uniform(0,1);
  }
  while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, alpha);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}



uint32_t zipf_third_instance(double alpha, uint32_t n){
  static int second = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (second == TRUE)
  {
    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, alpha));
    c = 1.0 / c;
    second = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = uniform(0,1);
  }
  while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, alpha);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}


double uniform(double a, double b){
	double c = (a + (b - a) * random_gen());
	return c;
}


double uniform_exp(double a, double b, double m){
	double c = (a + (b - a) * random_exp_gen(m));
	return c;
}


double exponential(double m){
	return (-m * log(1.0 - random_gen()));
}

double exp(double m){
	return (m * pow(M_E, -m*random_gen()));
}


double erlang(long n, double b){
	long i;
	double x = 0.0;

	for (i = 0; i < n; i++)
		x += exponential(b);
	return (x);
}


double normal(double m, double s){
	const double p0 = 0.322232431088;
	const double q0 = 0.099348462606;
	const double p1 = 1.0;
	const double q1 = 0.588581570495;
	const double p2 = 0.342242088547;
	const double q2 = 0.531103462366;
	const double p3 = 0.204231210245e-1;
	const double q3 = 0.103537752850;
	const double p4 = 0.453642210148e-4;
	const double q4 = 0.385607006340e-2;
	double u, t, p, q, z;

	u = random_gen();
	if (u < 0.5)
		t = sqrt(-2.0 * log(u));
	else
		t = sqrt(-2.0 * log(1.0 - u));
	p = p0 + t * (p1 + t * (p2 + t * (p3 + t * p4)));
	q = q0 + t * (q1 + t * (q2 + t * (q3 + t * q4)));
	if (u < 0.5)
		z = (p / q) - t;
	else
		z = t - (p / q);
	return (m + s * z);
}


double lognormal(double a, double b){
	return (exp(a + b * normal(0.0, 1.0)));
}


double chisquare(long n){
	long i;
	double z, x = 0.0;

	for (i = 0; i < n; i++) {
		z = normal(0.0, 1.0);
		x += z * z;
	}
	return (x);
}


double student(long n){
	return (normal(0.0, 1.0) / sqrt(chisquare(n) / n));
}


