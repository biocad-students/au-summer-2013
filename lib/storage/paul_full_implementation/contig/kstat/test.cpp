#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "kstat.hpp"
#include "../unit_tests.h"


void test_kstat()
{
	std::vector<byte> alpha {'A', 'C', 'G', 'T', 'N'};
	size_t k = 7;

	kstatistics<size_t> my_kstat(alpha, k);

	std::string s1 = "ACGTCGATCGTCAGACTGACTGACTAGCTAGCATCTAGCTACGATCGATCGACT";
	for (std::string::iterator i = s1.begin(); i != s1.end(); ++i)
	{
		my_kstat.add(i, s1.end(), rand() % 100);
	}

	std::string s2 = "TCGTCAGACTGACTGA";
	for (std::string::iterator i = s2.begin(); i != s2.end(); ++i)
	{
		std::vector<size_t>* v = my_kstat.get(i, s2.end());
		if (v != nullptr)
		{
			for (auto c : *v)
			{
				std::cout << c << " ";
			}
			std::cout << std::endl;
		}
	}
}