#include "../include/unittest.h"

#include "../../include/storage/storage.hpp"
#include "../../common/include/fasta_reader.h"

static void default_traits_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin default_traits_instatiation_unittest");

        storage<int, Prop<int> > storage_instance;

        PERFOMANCE_TIME("End");
}

static void push_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin push_unittest");

        storage<int> storage_instance;

        std::string str1("ACGTACGT");
        storage_instance.pushSequence(str1.begin(), str1.end(), str1);
        std::string str2("ACGTAAAA");
        storage_instance.pushSequence(str2.begin(), str2.end(), str2);
        std::string str3("ACGTCCCC");
        storage_instance.pushSequence(str3.begin(), str3.end(), str3);

        PERFOMANCE_TIME("End kstat_unittest");
}

static void load_from_fasta_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin load_from_fasta_unittest");

        storage<int> storage_instance;

        std::string path = "../../../data/germline/human/VH.fasta";
        FastaReader FR(path);
        Read tmp;
        std::string::iterator begin;
        std::string::iterator end;
        while(!FR.is_eof())
        {
                FR >> tmp;
                begin = tmp.seq.begin();
                end = tmp.seq.end();
                storage_instance.pushSequence(begin, end, tmp.name);
        }

        PERFOMANCE_TIME("End");
}

static void find_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin find_unittest");

        storage<int> storage_instance;

        std::string str1("ACGTACGTAAAA");
        storage_instance.pushSequence(str1.begin(), str1.end(), str1);
        std::string str2("ACGTAAAACCCCC");
        storage_instance.pushSequence(str2.begin(), str2.end(), str2);
        std::string str3("ACGTCCCCTTTTT");
        storage_instance.pushSequence(str3.begin(), str3.end(), str3);

        std::string needle("ACGTAAACG");
        storage_instance.find(needle.begin(), needle.end());

        PERFOMANCE_TIME("End");
}

static void align_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin align_unittest");

        storage<int> storage_instance;

        std::string str1("AAAAAAAAAAAAA");
        storage_instance.pushSequence(str1.begin(), str1.end(), str1);
        std::string str2("AAAAAAAACCCCC");
        storage_instance.pushSequence(str2.begin(), str2.end(), str2);
        std::string str3("AAAGGGGGGGGGG");
        storage_instance.pushSequence(str3.begin(), str3.end(), str3);

        std::string align_str = "AAAAAACCC";
        storage_instance.align(align_str.begin(), align_str.end());

        PERFOMANCE_TIME("End");
}

static void align1_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin align1_unittest");

        storage<int> storage_instance;

        std::string path = "../../../data/germline/human/VH.fasta";
        FastaReader FR(path);
        Read tmp;
        std::string::iterator begin;
        std::string::iterator end;
        while(!FR.is_eof())
        {
                FR >> tmp;
                begin = tmp.seq.begin();
                end = tmp.seq.end();
                storage_instance.pushSequence(begin, end, tmp.name);
        }

        std::string align_str = "TCGCGCCCAGCCGGCCATGGCGAGGTGCAGCTGGATGCAGTCTGGGGGGGTACTCTGAGACGCTCTTGTGCAGCATCTGGATTCACCTTCCGTAGCTCTGTTATGAGCTGAGTCCGACAGGCTCCAGGGAAGGGTTTCTGGAGTGGGTGACACTCCAGTATTTGGCGTGAACGAGTCTCACATACTATGCAGAGTCCGTGAAGAGATTCTCCATCTCCACAGACAAAACATTCTGTATCTGCAAATGAACAGCCTGAAGCCTGAAGACACGGCCGTGTATTACTGTGCTGCAGATCAATGCGGTAGTAGCTGCCGCCGGTATCTCGCAGTTTGGGGCCAGGGCACCCTGGTCACCGTCTCTTCAGGCTCGAGTGC";
        storage_instance.align(align_str.begin(), align_str.end());

        PERFOMANCE_TIME("End");
}

static void align2_unittest(void)
{
        storage<int> storage_instance;

        std::string path = "../../../data/germline/human/VH_corrected.fasta";
        FastaReader FR(path);
        Read tmp;
        std::string::iterator begin;
        std::string::iterator end;
        while(!FR.is_eof())
        {
                FR >> tmp;
                begin = tmp.seq.begin();
                end = tmp.seq.end();
                storage_instance.pushSequence(begin, end, tmp.name);
        }

        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin align2_unittest");

        std::string align_str = "TCGCGCCCAGCCGGCCATGGCGAGGTGCAGCTGGATGCAGTCTGGGGGGGTACTCTGAGACGCTCTTGTGCAGCATCTGGATTCACCTTCCGTAGCTCTGTTATGAGCTGAGTCCGACAGGCTCCAGGGAAGGGTTTCTGGAGTGGGTGACACTCCAGTATTTGGCGTGAACGAGTCTCACATACTATGCAGAGTCCGTGAAGAGATTCTCCATCTCCACAGACAAAACATTCTGTATCTGCAAATGAACAGCCTGAAGCCTGAAGACACGGCCGTGTATTACTGTGCTGCAGATCAATGCGGTAGTAGCTGCCGCCGGTATCTCGCAGTTTGGGGCCAGGGCACCCTGGTCACCGTCTCTTCAGGCTCGAGTGC";
        storage_instance.align(align_str.begin(), align_str.end());

        PERFOMANCE_TIME("End");
}

void run_storage_unittests(void)
{
        default_traits_instatiation_unittest();
        push_unittest();
        load_from_fasta_unittest();
        find_unittest();
        align_unittest();
        align1_unittest();
        align2_unittest();
}
