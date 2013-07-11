#include "contig.hpp"
#include "fasta_reader.h"
#include "kstat.hpp"
#include "annotation.hpp"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

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
}

void read_fasta_unittest() {
	clock_t t0 = clock();
	contig<char, Prop> my_contig;
	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
	Read tmp;
	while(!FR.is_eof()) {
		FR >> tmp;
		int id = my_contig.pushSequence(tmp.seq.begin(), tmp.seq.end(), Prop<char>(tmp.name));
		auto vct = my_contig.getPath(id);
	}
	std::cout << my_contig[158].second.name << std::endl;
	clock_t t1 = clock();
	std::cout << "time_read_fasta_unittest: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
}

//void search_seq_unittest() {
//	contig<char, Prop> my_contig;
//	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
//	Read tmp;
//	while(!FR.is_eof()) {
//		FR >> tmp;
//		int id = my_contig.pushSequence(tmp.seq.begin(), tmp.seq.end(), Prop<char>(tmp.name));
//		auto vct = my_contig.getPath(id);
//	}
//	std::string str_gene1 = "ACGTACGT";
//	std::vector<std::vector<trie_node<char>*>> results = my_contig.find(str_gene1.begin(), str_gene1.end());
//	std::vector<std::vector<trie_node<char>*>> autoresults = my_contig.find(str_gene1.begin(), str_gene1.end());
//}

//void align_seq_unittest() {
//	contig<char, Prop> my_contig;
//	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
//	Read tmp;
//	while(!FR.is_eof()) {
//		FR >> tmp;
//		int id = my_contig.pushSequence(tmp.seq.begin(), tmp.seq.end(), Prop<char>(tmp.name));
//		auto vct = my_contig.getPath(id);
//	}
//	std::string str_gene1 = "ACGTACGT";
//	// TODO: create FIGA class holder
//}

void annotation_unittest(void ) {
	annotation_vector_t<char, Prop> annot_;

	annotation_vector_t<char, Prop>::iterator aniter = annot_.begin();
}

void kstat_unittest(void) {
	clock_t t0 = clock();
	annotation_vector_t<char, Prop> annot_;
	annotation_vector_t<char, Prop>::iterator aniter = annot_.begin();
	std::vector<unsigned char> alphabet_;
	alphabet_.push_back('A');
	alphabet_.push_back('C');
	alphabet_.push_back('G');
	alphabet_.push_back('T');
	alphabet_.push_back('N');
	int K_ = 7;
	kstat_t<char, Prop> kstat_(alphabet_, K_);

	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		iter = tmp.seq.begin();
		end = tmp.seq.end();
		for(;iter != end; ++iter)
		{
			if(!kstat_.add(iter, end, aniter))
				break;
		}
	}
	clock_t t1 = clock();
	std::cout << "time_kstat_unittest: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
	std::string tst = "AGCCTGG";
	std::vector<typename annotation_vector_t<char, Prop>::iterator>* result = kstat_.get(tst.begin(), tst.end());
	std::cout << &result[150] << std::endl;
}

int main()
{
	kstat_unittest();
	add_sequence_unittest();
	read_fasta_unittest();
}