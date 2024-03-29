#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include "../contig/contig.hpp"
#include "../contig/aligner/container.hpp"

#include "../unittest.h"

void test_alicont()
{
    score_matrix m("/home/mactep/BLOSUM62.txt");
    alicont a("MEANLY", -5, std::move(m));
    simple_matrix2i m1 = std::move(a.score("PLEA"));
    a.push("PLEA", &m1);
    simple_matrix2i m2 = std::move(a.score("SANT"));
    a.push("SANT", &m2);
    simple_matrix2i m3 = std::move(a.score("LY"));
    a.push("LY", &m3);

    std::cout << a.target() << std::endl;
    a.print();
    alignment_result res = a.alignment();
    std::cout << res.first << std::endl << res.second << std::endl;

    a.pop();
    m3 = std::move(a.score("PYR"));
    a.push("PYR", &m3);

    std::cout << a.target() << std::endl;
    a.print();
    res = a.alignment();
    std::cout << res.first << std::endl << res.second << std::endl;
}

void test_contig_alicont()
{
    igc::contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    std::string s1 = "ACGTAG";
    std::string s2 = "ACGTCGT";
    std::string s3 = "ACAAT";
    std::string s4 = "ACAAC";

    my_contig.push(s1.begin(), s1.end(), "s1");
    my_contig.push(s2.begin(), s2.end(), "s2");
    my_contig.push(s3.begin(), s3.end(), "s3");
    my_contig.push(s4.begin(), s4.end(), "s4");

    std::string query = "GCGTTG";
    score_matrix m("/home/mactep/BLOSUM62.txt");
    auto r = my_contig.align(query.begin(), query.end(), -5, m, 2);
    for (auto i : r)
    {
        std::cout << i.score << std::endl << i.first
                             << std::endl << i.second
                             << std::endl << std::endl;
    }
    std::cout << std::endl;
}

void test_contig_alicont2()
{
    igc::contig<Alphabet, RegionProp> my_contig("CONTIG-TEST", Alphabet::getAlphabet());
    //import_data("/home/mactep/DEV/au-summer-2013/data/germline/human/VH.fasta", my_contig);
    import_data("/home/mactep/Data/NGS-llama/VH_corrected.fasta", my_contig);
    std::string query = "CAGGTTCAGCTGGTGCAGTCTGGGGCTGAGGTGAAGAAGCCTGGGGCCTCAGTGAAGGTTTCCTGCAAGGCTTCTGGATACACCTTCACTAGCTATGCTATGCATTGGGTGCGCCAGGCCCCCGGACAAAGGCTTGAGTGGATGGGATGGAGCAACGCTGGCAATGGTAACACAAAATATTCACAGGAGTTCCAGGGCAGAGTCACCATTACCAGGGACACATCCGCGAGCACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAGGACATGGCTGTGTATTACTGTGCGAGAGA";

    score_matrix m("/home/mactep/BLOSUM62.txt");
    auto r = my_contig.align(query.begin(), query.end(), -5, m, 10);
    for (auto i : r)
    {
        std::cout << i.score << std::endl << i.first
                             << std::endl << i.second
                             << std::endl << std::endl;
    }
    std::cout << std::endl;
}
