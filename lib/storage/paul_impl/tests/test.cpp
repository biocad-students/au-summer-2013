#include <iostream>
#include <string>
#include <cstdlib>

#include "../annotation/annotation.hpp"
#include "../trie/trie.hpp"
#include "../kstat/kstat.hpp"
#include "../contig.hpp"

#include "../algorithm.hpp"

#include "../unittest.h"

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
    std::string s4 = "ACCCATCGATCTGCGACTACGTTC";
    std::string s5 = "ACTCATCGATCTGCGACTACGAAA";

    my_contig.push_unannotated(s1.begin(), s1.end(), "s1");
    my_contig.push_unannotated(s2.begin(), s2.end(), "s2");
    my_contig.push_unannotated(s3.begin(), s3.end(), "s3");
    my_contig.push_unannotated(s4.begin(), s4.end(), "s4");
    my_contig.push_unannotated(s5.begin(), s5.end(), "s5");

    std::string sp = "GATCTGCGACTACG";
    auto result = contig_search(sp.begin(), sp.end(), my_contig);
    for(auto i : result)
    {
        contig<Alphabet, RegionProp>::const_iterator iter = my_contig.iter(i);
        for (auto c = sp.begin(); c != sp.end() - 1; )
        {
            std::cout << *c << ": ";
            for (auto j : my_contig.getLabels(iter))
            {
                std::cout << j << " ";
            }
            std::cout << std::endl;
            ++c;
            iter = iter.next(*c);
        }
    }
    std::cout << std::endl;
}

void test_contig2()
{
    contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    std::string s1 = "ACCCGTCGTGCAGCATGCATGCGACTACGCGCA";
    std::string s2 = "ACCCATCGATCTGCGACTACGCGCA";
    std::string s3 = "CGAACGCTCAGCATGCATGCGACTACGCGCA";

    my_contig.push_unannotated(s1.begin(), s1.end(), "s1");
    my_contig.push_unannotated(s2.begin(), s2.end(), "s2");
    my_contig.push_unannotated(s3.begin(), s3.end(), "s3");

    std::cout << "Labels of nodes in DFS:" << std::endl;
    contig<Alphabet, RegionProp>::iterator iter = my_contig.begin();
    while (iter != my_contig.end())
    {
        auto labels = my_contig.getLabels(iter++);
        std::cout << "( ";
        for(auto i : labels)
        {
            std::cout << i << " ";
        }
        std::cout << ")   ";
    }
    std::cout << std::endl;
}

void test_contig()
{
    contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    std::string s1 = "ACCCGTCGTGCAGCATGCATGCGACTACGCGCA";

    contig<Alphabet, RegionProp>::const_iterator iter = my_contig.push_unannotated(s1.begin(), s1.end(), "s1");

    std::cout << "get labels" << std::endl;
    auto labels = my_contig.getLabels(iter);
    std::cout << "( ";
    for(auto i : labels)
    {
        std::cout << i << " ";
    }
    std::cout << ")   " << std::endl;

    while (iter != my_contig.end())
    {
        auto annotations = my_contig.getAnnotations(iter++);
        for (auto i : annotations)
        {
            std::cout << i.region_id << " ";
        }
    }
    std::cout << std::endl;

    std::cout << "with root" << std::endl;
    for (auto i = my_contig.begin(); i != my_contig.end(); ++i)
    {
        auto annotations = my_contig.getAnnotations(i);
        for (auto i : annotations)
        {
            std::cout << i.region_id << " ";
        }
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
