#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "../contig/annotation/annotation.hpp"
#include "../contig/trie/trie.hpp"
#include "../contig/kstat/kstat.hpp"
#include "../contig/contig.hpp"

#include "../utils/fasta_reader.h"

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

void test_fasta()
{
    time_t c = clock();
    contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    FastaReader fr("/home/mactep/Data/NGS-llama/out2/VH/VH_corrected.fasta");
    Read tmp_read;
    std::cout << "Import started" << std::endl;
    size_t count = 0;
    size_t real_length = 0;
    while (!fr.is_eof())
    {
        fr >> tmp_read;
        my_contig.push(tmp_read.seq.begin(), tmp_read.seq.end(), tmp_read.name);
        real_length += tmp_read.seq.size();
        count++;
    }
    std::cout << "Import ended (" << count << ")" << std::endl;

    std::cout << "FOUND: "
              << my_contig.find(tmp_read.seq.begin() + 12,
                                tmp_read.seq.begin() + 22).size()
              << std::endl;
    c = clock() - c;
    std::cout << "Compression: " << (double)my_contig.size() / real_length << std::endl;
    std::cout << "Real time: " << (double)c / CLOCKS_PER_SEC << std::endl;
}

void test_fasta_push()
{
    contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    FastaReader fr("/home/mactep/Data/NGS-llama/out2/VH/VH_corrected.fasta");
    Read tmp_read;
    std::vector<Read> tmp_vec;
    time_t c = clock();
    while (!fr.is_eof())
    {
        fr >> tmp_read;
        tmp_vec.push_back(tmp_read);
    }
    std::cout << "KSeq read: " << (double)(clock() - c) / CLOCKS_PER_SEC << std::endl;

    c = clock();
    for (auto r : tmp_vec)
    {
        my_contig.push(r.seq.begin(), r.seq.end(), r.name);
    }
    std::cout << "Container push: " << (double)(clock() - c) / CLOCKS_PER_SEC << std::endl;
}
