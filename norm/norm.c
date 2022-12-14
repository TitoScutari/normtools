/**
	norm_random_note
	tito scutari
*/
#define MAXAPI_USE_MSCRT

#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include <stdlib.h>

////////////////////////// object struct
typedef struct _norm
{
	t_object ob;			// the object itself (must be first)
	double mu;
	double sigma;
	void* m_outlet1;
} t_norm;

///////////////////////// function prototypes
void *norm_new();
void norm_free();
void norm_ft1(t_norm* x, double f);
void norm_bang(t_norm* x);
void norm_mu(t_norm* x, double new_mu);
void norm_sigma(t_norm* x, double new_sigma);
double randn(double mu, double sigma);

//////////////////////// global class pointer variable
void *norm_class;


void ext_main(void *r)
{
	t_class *c;

	c = class_new("norm", (method)norm_new, (method)norm_free, (long)sizeof(t_norm),
				  0L /* leave NULL!! */, A_GIMME, 0);

	/* you CAN'T call this from the patcher */
	class_addmethod(c, (method)norm_bang, "bang", 0);
	class_addmethod(c, (method)norm_mu, "float", A_FLOAT, 0);
	class_addmethod(c, (method)norm_ft1, "ft1", A_FLOAT, 0);

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	norm_class = c;
}

void norm_free(t_norm *x)
{
	;
}

void *norm_new()
{
	t_norm *x = (t_norm*)object_alloc(norm_class);
	x->m_outlet1 = floatout(x);
	floatin(x, 1);
	x->mu = 0;
	x->sigma = 1;
	return (x);
}

void norm_ft1(t_norm* x, double f) {
	norm_sigma(x,f);
}

void norm_bang(t_norm* x) {
	outlet_float(x->m_outlet1, randn(x->mu, x->sigma));
}

void norm_mu(t_norm* x, double new_mu) {
	x->mu = new_mu;
	double r = randn(x->mu, x->sigma);
	outlet_float(x->m_outlet1, r);
}

void norm_sigma(t_norm* x, double new_sigma) {
	x->sigma = new_sigma;
}

// tobe lib

double randn(double mu, double sigma) {
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;

	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double)X2);
	}

	do
	{
		U1 = -1 + ((double)rand() / RAND_MAX) * 2;
		U2 = -1 + ((double)rand() / RAND_MAX) * 2;
		W = pow(U1, 2) + pow(U2, 2);
	} while (W >= 1 || W == 0);

	mult = sqrt((-2 * log(W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double)X1);
}
