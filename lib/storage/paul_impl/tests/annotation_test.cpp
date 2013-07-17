#include <iostream>
#include <string>
#include <cstdlib>

#include "../annotation/annotation.hpp"
#include "../unittest.h"

typedef unsigned char byte;

struct Alphabet
{
    static std::vector<byte> getAlphabet()
    {
        return std::vector<byte> {'A', 'C', 'G', 'T', 'N'};
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


void test_anno()
{
    annotation<Alphabet, RegionProp> my_anno;
    annotation<Alphabet, RegionProp>::record_type & record = my_anno.add("Seq1");

    std::string s1 = "ACGCGACAGCACGAGAGAGGAGAGCA";
    size_t i = 0;
    for (auto c : s1)
    {
        RegionProp<Alphabet> & data = record.push(i++, c);
        data = RegionProp<Alphabet>(rand() % 7);
    }

    for (size_t i = 0; i != my_anno.size(); ++i)
    {
        for (size_t j = 0; j != my_anno[i].size(); ++j)
        {
            std::cout << my_anno[i][j].region_id << " ";
        }
        std::cout << std::endl;
    }
}
