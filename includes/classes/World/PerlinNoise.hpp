#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

class PerlinNoise
{
	
private :


public:

		/*****	1 - constructors 		*****/

	~PerlinNoise();
	PerlinNoise();
	PerlinNoise(const 				PerlinNoise &copy);
	
		/*****	2 - noise makers	 	*****/

	
	void							noise1d(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput);
	void							noise2d(int nWidth, int nheight, float *fSeed, int nOctaves, float fBias, float *fOutput);


		/*****	3 - data getters 		*****/


		/*****	4 - operators 			*****/

	PerlinNoise&					operator=(const PerlinNoise &copy);
};

	




#endif