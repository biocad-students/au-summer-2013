#include <iostream>
#include <string>
#include <vector>

#include "contig/contig.hpp"
#include "contig/unit_tests.h"



void tests()
{
	std::cout << "**Annotation test" << std::endl;
	test_anno();

	std::cout << std::endl << "***Trie test" << std::endl;
	test_trie();

	std::cout << std::endl << "***KStat test" << std::endl;
	test_kstat();

	std::cout << std::endl << "***ContIG test" << std::endl;
	test_contig();

	std::cout << std::endl << "***Search test" << std::endl;
	test_search();
}

int main(int argc, char *argv[])
{
	tests();

	return 0;
}
