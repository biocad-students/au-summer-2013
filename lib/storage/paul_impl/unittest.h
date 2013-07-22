#pragma once

typedef unsigned char byte;

struct Alphabet
{
    static std::vector<byte> getAlphabet()
    {
        std::vector<byte> v;
        v.push_back('A');
        v.push_back('C');
        v.push_back('G');
        v.push_back('T');
        v.push_back('N');
        return v;
        //return std::vector<byte> {'A', 'C', 'G', 'T', 'N'};
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

void test_trie();
void test_kstat();
void test_anno();
void test_contig();
void test_contig2();
void test_search();
void test_fasta();
void test_fasta_push();
