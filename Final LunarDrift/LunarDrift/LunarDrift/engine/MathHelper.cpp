#include "MathHelper.h"
#include <cmath>
#include <cstdlib>


double rand(double min, double max)
{
	double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return min + (r * (max - min));
}

float randf(float min, float max)
{
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return min + (r * (max - min));
}
