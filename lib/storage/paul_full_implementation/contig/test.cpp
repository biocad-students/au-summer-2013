#include <iostream>
#include <string>
#include <cstdlib>

#include "annotation/annotation.hpp"
#include "trie/trie.hpp"
#include "kstat/kstat.hpp"
#include "contig.hpp"

#include "unit_tests.h"

typedef unsigned char byte;

struct Alphabet
{
	static std::vector<byte> getAlphabet()
	{
		return std::vector<byte> {'A', 'C', 'G', 'T'};
	}
};

template <class T>
struct RegionProp
{
	size_t region_id;

	RegionProp(size_t id = 0) : region_id(id)
	{
	}
};

void test_contig()
{
	contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
	std::string s1 = "ACCCGTCGTGCAGCATGCATGCGACTACGCGCA";
	std::string s2 = "ACCCATCGATCTGCGACTACGCGCA";
	std::string s3 = "CGAACGCTCAGCATGCATGCGACTACGCGCA";

	contig<Alphabet, RegionProp>::iterator iter = my_contig.push_unannotated(s1.begin(), s1.end(), "s1");
	std::cout << my_contig.getLabel(iter) << std::endl;
	while(iter != my_contig.end())
	{
		std::cout << my_contig.getAnnotation(iter++).region_id << " ";
	}
	std::cout << std::endl;
}
