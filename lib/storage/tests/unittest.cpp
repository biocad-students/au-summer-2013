#include "contig.hpp"
#include "fasta_reader.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#define cheese trie

template <class T>
struct Prop {
	typedef T alphabet;
	std::string name;
	Prop(std::string name_) : name(name_) {}
	Prop() : name("") {}
};
template<class T>
void dump_s(std::vector<trie_node<T>*> const & vct) {
	for(auto iter = vct.begin(); iter != vct.end(); ++iter)
	  {
		std::cout << (*iter)->getParentVal();
	  }
	  std::cout << std::endl;
}

void add_sequence_unittest()
{
  contig<char, Prop> cheese_;
  std::string str_gene1 = "ACGTACGT";
  std::string str_gene2 = "ACGTAAA";
  std::string str_gene3 = "GCT";
  int id;
  id = cheese_.pushSequence(str_gene1.begin(), str_gene1.end(), Prop<char>());
  id = cheese_.pushSequence(str_gene2.begin(), str_gene2.end(), Prop<char>());
  id = cheese_.pushSequence(str_gene3.begin(), str_gene3.end(), Prop<char>());

  char chr[] = {'A', 'C', 'T'};
  id = cheese_.pushSequence(chr, chr + 3, Prop<char>());
  char chr1[] = {'A', 'C', 'T'};
  std::vector<char> vec(chr1, chr1 + 3);
  id = cheese_.pushSequence(vec.begin(), vec.end(), Prop<char>());

  std::list<char> lst;
  lst.push_back('C');
  lst.push_back('G');
  lst.push_back('T');
  id = cheese_.pushSequence(lst.begin(), lst.end(), Prop<char>());

  std::string str_gene4 = "ACGCATC";
  id = cheese_.pushSequence(str_gene4.begin(), str_gene4.end(), Prop<char>());

  std::vector<trie_node<char>*> vct = cheese_.getPath(id);
  dump_s(vct);


}

void read_fasta_unittest() {
	contig<char, Prop> my_contig;
	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
	Read tmp;
	while(!FR.is_eof()) {
		FR >> tmp;
		int id = my_contig.pushSequence(tmp.seq.begin(), tmp.seq.end(), Prop<char>(tmp.name));
		auto vct = my_contig.getPath(id);
		dump_s(vct);
	}
	std::cout << my_contig[158].second.name;
}

int main()
{
  add_sequence_unittest();
  read_fasta_unittest();
}