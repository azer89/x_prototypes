
#include <iostream>
#include <cassert>

// http://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats

float RandomFloat(float min, float max)
{
	// this  function assumes max > min, you may want 
	// more robust error checking for a non-debug build
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random*range) + min;
}


int _tmain(int argc, _TCHAR* argv[])
{
	float aNum = 0.01f;
	float bNum = 0.20f;
	for (int a = 0; a < 100; a++)
	{
		std::cout << RandomFloat(aNum, bNum) << "\n";
	}

	return 0;
}

