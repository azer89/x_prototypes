// RejectionSampling.cpp : Defines the entry point for the console application.

// http://stackoverflow.com/questions/22975359/how-to-optimize-rejection-sampling (doesn't work in windows)
// http://stackoverflow.com/questions/21959404/initialising-stddiscrete-distribution-in-vs2013 (solution)
// http://ideone.com/351Jhg (solution)

#include "stdafx.h"

#include <iostream>
#include <map>
#include <random>

template <typename T>
class sampler
{
	//std::size_t i;
	std::vector<T> keys;
	std::discrete_distribution<T> distr;

public:
	sampler(const std::vector<T>& keys, const std::vector<float>& prob)
	{
		this->keys = keys;

		std::size_t i = 0;
		this->distr = std::discrete_distribution<>(prob.size(),
			0, // dummy!
			1, // dummy!
			[&prob, &i](float)
		{			
			auto w = prob[i];
			++i;
			return w;
		});

	}
	//sampler(const std::vector<T>& keys, const std::vector<float>& prob) :
	//	keys(keys), distr(prob.begin(), prob.end()) { }

	T operator()()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return keys[distr(gen)];
	}
};

int _tmain(int argc, _TCHAR* argv[])
{	
	using T = int;
	sampler<T> samp({ 19, 54, 192, 732 }, { 0.1f, 0.2f, 0.4f, 0.3f });
	std::map<T, size_t> hist;

	for (size_t n = 0; n < 10000; ++n)
	{
		++hist[samp()];
	}

	for (auto i : hist)
	{
		std::cout << i.first << " generated " <<
			i.second << " times" << std::endl;
	}

	std::getchar();
	return 0;
}

