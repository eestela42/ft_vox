#include <classes/World/PerlinNoise.hpp>


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

		/*****	2 - noise makers	 	*****/

void	PerlinNoise::noise1d(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
	for (int x = 0; x < nCount; x++)
	{
		float fNoise = 0.0f;
		float fScale = 1.0f;
		float fScaleAcc = 0.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = nCount >> o;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;

			float fBlend = (float)(x - nSample1) / (float)nPitch;

			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / fBias;
		}
		fOutput[x] = fNoise / fScaleAcc;
	}
}

void	PerlinNoise::noise2d(int nWidth, int nheight, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nheight; y++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(x - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleX1 + nSampleY1 * nWidth] + fBlendX * fSeed[nSampleX2 + nSampleY1 * nWidth];
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleX1 + nSampleY2 * nWidth] + fBlendX * fSeed[nSampleX2 + nSampleY2 * nWidth];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / fBias;
			}
			fOutput[x + y * nWidth] = fNoise / fScaleAcc;
			}
	}
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