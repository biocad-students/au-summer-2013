#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "fasta_reader.h"
#include "read.h"
#include "amino_acid_converter.h"
#include "reverse_complement.h"
#include "kmer_generator.h"
#include "output_formatter.h"
#include "database.h"

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

#define ITERS 10000
#define DATA_FILE "./test_data/sample.fasta"

TEST(Database, ReadDatabaseTest) {
	Database db(DATA_FILE, 51);
	{
		ASSERT_EQ(db.get_sequences_amount(), 2);
		std::string tmp;
		db.get_name_by_sequence("GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG", tmp);
		ASSERT_EQ(tmp, "IGHJ1*01");

		db.get_sequence_by_name("IGHJ1*01", tmp);
		ASSERT_EQ(tmp, "GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG");

		db.get_name_by_sequence("AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA", tmp);
		ASSERT_EQ(tmp, "IGHJ1P*01");

		db.get_sequence_by_name("IGHJ1P*01", tmp);
		ASSERT_EQ(tmp, "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
	}
	{
		ASSERT_EQ(db.get_kmers_amount(), 6);
		auto it = db.get_kmer_iterator();

		//kmer
		ASSERT_EQ(*(it->first), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACA");
		//parent sequence
		ASSERT_EQ(**(it++->second.begin()), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
		ASSERT_EQ(*(it->first), "AAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACAT");
		ASSERT_EQ(**(it++->second.begin()), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
		ASSERT_EQ(*(it->first), "AGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATC");
		ASSERT_EQ(**(it++->second.begin()), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
		ASSERT_EQ(*(it->first), "CTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG");
		ASSERT_EQ(**(it++->second.begin()), "GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG");
		ASSERT_EQ(*(it->first), "GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCA");
		ASSERT_EQ(**(it++->second.begin()), "GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG");
		ASSERT_EQ(*(it->first), "GGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
		ASSERT_EQ(**(it++->second.begin()), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
	}
}

TEST(Alignment, OutputTest) {
	//TODO why do we have 3 softclipped nucleotides on both edges?
	const std::string query = "PROTEINPROTEIN";
	const std::string ref = "PROTEINPROTEIN";

	StripedSmithWaterman::Aligner aligner(10, -10, 11, 11); //match, mismatch, gap open, gap extend
	StripedSmithWaterman::Filter filter;
	StripedSmithWaterman::Alignment alignment;

	aligner.SetReferenceSequence(ref.c_str(), ref.length());
	aligner.Align(query.c_str(), filter, &alignment);

	OutputFormatter::print_alignment(std::cout, alignment, ref, query, "Ref", "Query");
}

TEST(Alignment, SpeedTest) {
	const std::string seq = "ATCTACTGACGCTGCTGCTGGTCTGCTGCTCCTCGCTGCCCAGCCGGCGATGGCTAGCTCCTATGAGCTGACTCAGCCGCCCTCCCTCTCTGCATCTCCGGGATCATCTGTCAGACTCGCCTGCA";
	const std::string ref = "CAGGTTCAGCTGGTGCAGTCTGGAGCTGAGGTGAAGAAGCCTGGGGCCTCAGTGAAGGTCTCCTGCAAGGCTTCTGGTTACACCTTTACCAGCTATGGTATCAGCTGGGTGCGACAGGCCCCTGGACAAGGGCTTGAGTGGATGGGATGGATCAGCGCTTACAATGGTAACACAAACTATGCACAGAAGCTCCAGGGCAGAGTCACCATGACCACAGACACATCCACGAGCACAGCCTACATGGAGCTGAGGAGCCTGAGATCTGACGACACGGCCGTGTATTACTGTGCGAGAGA";

	StripedSmithWaterman::Aligner aligner;
	StripedSmithWaterman::Filter filter;
	StripedSmithWaterman::Alignment alignment;

	aligner.SetReferenceSequence(ref.c_str(), ref.length());

	clock_t start = clock();
	for (int i = 0; i < ITERS; ++i) {
		aligner.Align(seq.c_str(), filter, &alignment);
	}
	clock_t ends = clock();
	LOG4CXX_INFO(logger, "Processor Time Spent for " << ITERS << " alignments: " << (double) (ends - start) / CLOCKS_PER_SEC << " seconds.");
}

TEST(Kmers, SimpleTest) {
	std::vector<std::string> result = KmerGenerator::getKmers("ATA", 2);

	ASSERT_EQ(result.size(), 2);
	ASSERT_EQ(result[0], "AT");
	ASSERT_EQ(result[1], "TA");
}

TEST(IO, SimpleTest) {
	std::vector<Read> result;
	FastaReader reader(DATA_FILE);
	if(reader.is_opened()) {
		while (!reader.is_eof()) {
			Read r;
			reader >> r;
			result.push_back(r);
		}
	}

	ASSERT_EQ(result.size(), 2);
	ASSERT_EQ(result[0].getSeq(), "GCTGAATACTTCCAGCACTGGGGCCAGGGCACCCTGGTCACCGTCTCCTCAG");
	ASSERT_EQ(result[1].getSeq(), "AAAGGTGCTGGGGGTCCCCTGAACCCGACCCGCCCTGAGACCGCAGCCACATCA");
}

TEST(ReverseComplement, SimpleTest) {
	const std::string nucleotid = "ATGAAT";
	const std::string res = ReverseComplement::getRevc(nucleotid);
	ASSERT_EQ(res, "ATTCAT");
}

TEST(NucleotidConvertion, SimpleConvertion) {
	const std::string nucleotid = "ATGAAT";
	std::vector<std::string> nucleotides = AminoAcidConverter::convertFromNucleotide(nucleotid);
	ASSERT_EQ(nucleotides.size(), 3);
	EXPECT_EQ(nucleotides[0], "MN");
	EXPECT_EQ(nucleotides[1], "z");
	EXPECT_EQ(nucleotides[2], "E");
}

TEST(AminoAcidConvertion, SimpleConvertion) {
	const std::string aminoAcid = "MN";
	std::vector<std::string> nucleotides = AminoAcidConverter::convertFromAminoAcid(aminoAcid);
	EXPECT_EQ(nucleotides.size(), 2);
	EXPECT_EQ(nucleotides[0], "ATGAAT");
	EXPECT_EQ(nucleotides[1], "ATGAAC");
}

TEST(AminoAcidConvertion, LongConvertion) {
	clock_t start = clock();
	const std::string aminoAcid = "QSVLTQPPSVS";

	LOG4CXX_INFO(logger, "Start converting " << aminoAcid);
	std::vector<std::string> nucleotides = AminoAcidConverter::convertFromAminoAcid(aminoAcid);
	EXPECT_GT(nucleotides.size(), 2);

    clock_t ends = clock();
    LOG4CXX_INFO(logger, "Processor Time Spent: " << (double) (ends - start) / CLOCKS_PER_SEC << " seconds.");
}

int main(int argc, char **argv) {
	log4cxx::PropertyConfigurator::configure("./config/log4cxx.properties");
	LOG4CXX_INFO(logger, "Start tests");

	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
