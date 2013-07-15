#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "tokenizer.h"
#include "alignment.h"
#include "nomenclature.h"


TEST(Utils, TokenizerTest) {
	const std::string line = "Hello World";
	std::vector<std::string> tokens = Tokenizer::tokenize(line);
	ASSERT_EQ(tokens.size(), 2);
	ASSERT_EQ(tokens[0], "Hello");
	ASSERT_EQ(tokens[1], "World");
}

TEST(Model, AlignmentTest) {
	const std::string line = "H52AO1A01ALABY	IGLV1-40*01	18	120	152	270	41	48";
	Alignment a(line);
	ASSERT_EQ(a.getReadName(), "H52AO1A01ALABY");
	ASSERT_EQ(a.getRefName(), "IGLV1-40*01");
	ASSERT_EQ(a.getReadBegin(), 18);
	ASSERT_EQ(a.getReadEnd(), 120);
	ASSERT_EQ(a.getRefBegin(), 152);
	ASSERT_EQ(a.getRefEnd(), 270);
	ASSERT_EQ(a.getMismatches(), 41);
	ASSERT_EQ(a.getScore(), 48);
}

TEST(Model, NomenclatureTest) {
	const std::string line = "A1	1	69	70	117	118	162	163	183	184	279	VK	0";
	Nomenclature a(line);
	ASSERT_EQ(a.getRefName(), "A1");
	ASSERT_EQ(a.getFR1begin(), 1);
	ASSERT_EQ(a.getCDR1begin(), 70);
	ASSERT_EQ(a.getFR2begin(), 118);
	ASSERT_EQ(a.getCDR2begin(), 163);
	ASSERT_EQ(a.getFR3begin(), 184);
	ASSERT_EQ(a.getFR3end(), 279);
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
