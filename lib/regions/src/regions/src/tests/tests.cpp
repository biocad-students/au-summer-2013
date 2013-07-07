#include <iostream>
#include <gtest/gtest.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "amino_acid_converter.h"
#include "reverse_complement.h"

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

TEST(ReverseComplement, SimpleTest) {
	const std::string nucleotid = "ATGAAT";
	const std::string res = revc(nucleotid);
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
