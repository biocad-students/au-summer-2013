#include <ctime>
#include <iostream>
#include <algorithm>
#include "unittest.h"

typedef unsigned char byte;

void fill_alphabet(std::vector<unsigned char>* _alphabet) {
	_alphabet->push_back('A');
	_alphabet->push_back('C');
	_alphabet->push_back('G');
	_alphabet->push_back('T');
}


void test()
{
    //time_t s = clock();

    //test_trie();
    //test_anno();
    //test_kstat();
    //test_contig();
    //test_contig2();
    //test_search();
    //test_fasta();
    //test_fasta_push();
    //test_alicont();
    //test_contig_alicont();
    //test_contig_alicont2();
    //s = clock() - s;
    //std::cout << "Test time: " << (double) s / CLOCKS_PER_SEC << std::endl;
}

int main()
{
    //test();
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::contig<char, Prop, Lab> my_storage("main", alphabet_, K_);
	std::cout << "Welcome to IG Storage" << std::endl;
	while(1) {
		std::string input;
		std::stringstream ss;
		std::cout << ">> ";
		std::getline(std::cin, input);
		ss << input;
		my_storage.process(ss);
	}
    return 0;
}
