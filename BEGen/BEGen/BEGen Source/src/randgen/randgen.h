/*
 * randgen.h
 *
 *  Modified on: May 1, 2009
 *    Adapted by: Mo Sadoghi
 *
 *
 * Name              : rvgs.c  (Random Variate GeneratorS)
 * Author            : Steve Park & Dave Geyer
 * Language          : ANSI C
 * Latest Revision   : 10-28-98
 */

#ifndef RANDGEN_H_
#define RANDGEN_H_

#include <assert.h>
#include "../shared/constant.h"
#include "../shared/stdint.h"

double random_gen();
double random_exp_gen(double m);

long bernoulli(double p);
long binomial(long n, double p);
long equilikely(long a, long b);
long equilikely_exp(long a, long b, double m);
long geometric(double p);
long pascal(long n, double p);
long poisson(double m);
uint32_t zipf_first_instance(double alpha, uint32_t n);
uint32_t zipf_second_instance(double alpha, uint32_t n);
uint32_t zipf_third_instance(double alpha, uint32_t n);

double exp(double m);
double uniform(double a, double b);
double uniform_exp(double a, double b, double m);
double exponential(double m);
double erlang(long n, double b);
double normal(double m, double s);
double lognormal(double a, double b);
double chisquare(long n);
double student(long n);


#endif /* RANDGEN_H_ */
