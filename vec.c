#include "vec.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _VEC_MIN_LENGTH(a, b) ({ \
		double _a = VEC_HEADER(a)->length; \
		double _b = VEC_HEADER(b)->length; \
		_a < _b ? _a : _b; \
		})

static inline bool length_will_overflow(const uint64_t length)
{
	if (length > (sizeof(uint64_t) >> 1) - sizeof(struct vec_header))
		return true;

	return false;
}

double *vec_new(const uint64_t length, const double * const values)
{
	if (length_will_overflow(length))
		return NULL;

	struct vec_header *header = malloc(sizeof(struct vec_header) * (length + 1));

	header->temp   = true;
	header->length = length;

	memcpy(header->vector, values, length * sizeof(double));

	return (double *)(header->vector);
}

double *vec_blank(const uint64_t length)
{
	if (length_will_overflow(length))
		return NULL;

	struct vec_header *header = calloc(length + 1, sizeof(struct vec_header));

	header->temp   = true;
	header->length = length;

	return (double *)(header->vector);
}

void vec_del(double * const vector)
{
	free(vector - 1);
}

void vec_deltemp(double * const vector)
{
	if (VEC_HEADER(vector)->temp)
		vec_del(vector);
}

inline double *vec_temp(double * const vector)
{
	VEC_HEADER(vector)->temp = 1;
	return vector;
}

inline double *vec_perm(double * const vector)
{
	VEC_HEADER(vector)->temp = 0;
	return vector;
}


double vec_dot(double * const vector_a, double * const vector_b)
{
	uint64_t length = _VEC_MIN_LENGTH(vector_a, vector_b);

	double result = 0;

	for (uint64_t i = 0; i < length; i++)
		result += vector_a[i] * vector_b[i];

	if (vector_b != vector_a)
		vec_deltemp(vector_b);

	vec_deltemp(vector_a);

	return result;
}

double *vec_cross(double * const vector_a, double * const vector_b)
{
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);

	if (vech_a->length != vech_b->length && vech_a->length != 3)
		return NULL;

	double i, j, k;
	i = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
	j = vector_a[2] * vector_b[0] - vector_a[0] * vector_b[2];
	k = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];

	if (vech_a->temp) {
		vec_deltemp(vector_b);
		vector_a[0] = i;
		vector_a[1] = j;
		vector_a[2] = k;
		return vector_a;
	} else if (vech_b->temp) {
		vector_b[0] = i;
		vector_b[1] = j;
		vector_b[2] = k;
		return vector_b;
	} else {
		return vec_new(3, (double []){i, j, k});
	}
}

double vec_len(double * const vector)
{
	return sqrtf(vec_dot(vector, vector));
}

double *vec_addm(double * const vector_a, double * const vector_b, const double multiplier)
{
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);

	uint64_t length = _VEC_MIN_LENGTH(vector_a, vector_b);

	double *output;

	if (vech_a->temp)
		output = vector_a;
	else if (vech_b->temp)
		output = vector_b;
	else
		output = vec_blank(length);

	for (uint64_t i = 0; i < length; i++)
		output[i] = vector_a[i] + vector_b[i] * multiplier;

	if (vech_a->temp && vech_b->temp)
		vec_deltemp(vector_b);

	return output;
}

double *vec_norm(double * const vector)
{
	struct vec_header *vech = VEC_HEADER(vector);

	double *output;

#ifdef FAST
	double ilength;

	if (vech->temp) {
		ilength = 1 / vec_len(vec_perm(vector));
		vec_temp(vector);
		output = vector;
	} else {
		ilength = 1 / vec_len(vector);
		output = vec_blank(vech->length);
	}

	for (uint64_t i = 0; i < vech->length; i++)
		output[i] = vector[i] * ilength;
#else
	double length;

	if (vech->temp) {
		length = vec_len(vec_perm(vector));
		vec_temp(vector);
		output = vector;
	} else {
		length = vec_len(vector);
		output = vec_blank(vech->length);
	}

	for (uint64_t i = 0; i < vech->length; i++)
		output[i] = vector[i] / length; /* SLOW */
#endif

	return output;
}
