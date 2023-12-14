#include <classes/World/PerlinNoise.hpp>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

double PerlinNoise::fade(double t) { 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) { 
	return a + t * (b - a); 
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

		/*****	1 - constructors 		*****/

PerlinNoise::~PerlinNoise()
{}

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::PerlinNoise(const PerlinNoise &copy)
{
	*this = copy;
}
PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(size*size);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

		/*****	2 - noise makers	 	*****/

// void	PerlinNoise::noise1d(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput)
// {
// 	for (int x = 0; x < nCount; x++)
// 	{
// 		float fNoise = 0.0f;
// 		float fScale = 1.0f;
// 		float fScaleAcc = 0.0f;

// 		for (int o = 0; o < nOctaves; o++)
// 		{
// 			int nPitch = nCount >> o;
// 			int nSample1 = (x / nPitch) * nPitch;
// 			int nSample2 = (nSample1 + nPitch) % nCount;

// 			float fBlend = (float)(x - nSample1) / (float)nPitch;

// 			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

// 			fScaleAcc += fScale;
// 			fNoise += fSample * fScale;
// 			fScale = fScale / fBias;
// 		}
// 		fOutput[x] = fNoise / fScaleAcc;
// 	}
// }

// void	PerlinNoise::noise2d(int nWidth, int nheight, float *fSeed, int nOctaves, float fBias, float *fOutput)
// {
// 	for (int y = 0; y < nheight; y++)
// 	{
// 		for (int x = 0; x < nWidth; x++)
// 		{
// 			float fNoise = 0.0f;
// 			float fScale = 1.0f;
// 			float fScaleAcc = 0.0f;

// 			for (int o = 0; o < nOctaves; o++)
// 			{
// 				int nPitch = nWidth >> o;
// 				int nSampleX1 = (x / nPitch) * nPitch;
// 				int nSampleY1 = (y / nPitch) * nPitch;

// 				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
// 				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

// 				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
// 				float fBlendY = (float)(x - nSampleY1) / (float)nPitch;

// 				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleX1 + nSampleY1 * nWidth] + fBlendX * fSeed[nSampleX2 + nSampleY1 * nWidth];
// 				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleX1 + nSampleY2 * nWidth] + fBlendX * fSeed[nSampleX2 + nSampleY2 * nWidth];

// 				fScaleAcc += fScale;
// 				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
// 				fScale = fScale / fBias;
// 			}
// 			fOutput[x + y * nWidth] = std::abs(fNoise / fScaleAcc);
// 			}
// 	}
// }
double PerlinNoise::Octave2D(double x, double y, const std::int32_t octaves, const double persistence) 
{
	double result = 0;
	double amplitude = 1;
	double fScale = 0;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += (newNoise2d(x, y, 0) * amplitude);
		x *= 2;
		y *= 2;
		fScale += amplitude;
		amplitude *= persistence;
	}

	return result / fScale;
}

double	PerlinNoise::newNoise2d(double x, double y, double z)
{
	// Find the unit cube that contains the point
	int X = (int) floor(x) & ((size*size)-1);
	int Y = (int) floor(y) & ((size*size)-1);
	int Z = (int) floor(z) & ((size*size)-1);

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
	return (res + 1.0)/2.0;
}

		/*****	3 - data getters 		*****/


		/*****	4 - operators 		*****/

PerlinNoise &PerlinNoise::operator=(const PerlinNoise &copy)
{
	if (this != &copy)
	{
		*this = copy;
	}
	return (*this);
}