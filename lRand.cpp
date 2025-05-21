#include "pch.h"
#include "lRand.h"

lRand::lRand()
{
}

lRand::~lRand()
{
}

float lRand::random()
{
	return (float)(rand()/(double)RAND_MAX);
}

float lRand::Gaussian()
{
	float u, v, s;
	while (1) {
		u = random() * 2.0 - 1.0;
		v = random() * 2.0 - 1.0;
		s = u * u + v * v;
		if (s > 1.0 || s == 0) continue;
		break;
	}
	s = sqrt(-2.0 * log(s) / s);

	return s*v;
}
