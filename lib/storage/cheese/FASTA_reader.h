#pragma once
#include <fstream>  

#include "FASTA_reader.h"
#include "cheese-trie.h"
#include <string>

template<typename T, typename ID_TYPE> 
void push_seq(std::string seq, trie<T, ID_TYPE>* my_trie) {
	if(seq.length()) {
		trie<T,ID_TYPE>::const_iterator iter = my_trie->add(seq.begin(), seq.end(), 0);
		while(iter != my_trie->end())
		{
			std::cout << iter.key();
			++iter;
		}
		std::cout << std::endl;
		
	}
}

template<typename T, typename ID_TYPE> 
void readFASTA(std::ifstream* stream, trie<T, ID_TYPE>* my_trie) {
	std::string seq;
	std::string buf;
	while(std::getline(*stream, buf)) {
		if(buf[0]=='>') {
			push_seq(seq, my_trie);
			seq.clear();
			continue;
		}
		if(buf[0]==';')
			continue;
		seq += buf;
	}
	push_seq(seq, my_trie);
	return;
}