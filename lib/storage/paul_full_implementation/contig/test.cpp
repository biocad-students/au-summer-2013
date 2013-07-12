#include <iostream>
#include <string>
#include <cstdlib>

#include "annotation/annotation.hpp"
#include "trie/trie.hpp"
#include "kstat/kstat.hpp"
#include "contig.hpp"

#include "algorithm.hpp"

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

void test_search()
{
	contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
	std::string s1 = "ACCCGTCGTGCAGCATGCATGCGACTACGCGCA";
	std::string s2 = "ACCCATCGATCTGCGACTACGCGCA";
	std::string s3 = "CGAACGCTCAGCATGCATGCGACTACGCGCA";

	std::string sp = "GATCTGCGACTACG";
	std::vector<contig<Alphabet, RegionProp>::iterator> result = search(sp.begin(), sp.end(), my_contig);
	for(auto i : result)
	{
		std::cout << i.symbol() << " ";
	}
	std::cout << std::endl;
}

void test_contig()
{
	contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
	std::string s1 = "ACCCGTCGTGCAGCATGCATGCGACTACGCGCA";
	std::string s2 = "ACCCATCGATCTGCGACTACGCGCA";
	std::string s3 = "CGAACGCTCAGCATGCATGCGACTACGCGCA";

	contig<Alphabet, RegionProp>::iterator iter = my_contig.push_unannotated(s1.begin(), s1.end(), "s1");
	std::cout << my_contig.getLabel(iter) << std::endl;
	while (iter != my_contig.end())
	{
		std::cout << my_contig.getAnnotation(iter++).region_id << " ";
	}
	std::cout << std::endl;

	std::cout << "with root" << std::endl;
	for (auto ai = my_contig.abegin(); ai != my_contig.aend(); ++ai)
	{
		std::cout << ai->region_id << " ";
	}
	std::cout << std::endl;

	std::cout << "isomorphic tree creation" << std::endl;
	trie<bool>* my_trie = nullptr;
	my_contig.copyTrie(&my_trie);
	for (auto i : *my_trie)
	{
		std::cout << i << " ";
	}
	delete my_trie;
	std::cout << std::endl;
}
