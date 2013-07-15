#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "fasta_reader.h"
#include "contig.hpp"
#include "kstat.hpp"
#include "annotation.hpp"
#include "annotation_record.hpp"
#include "annotation_node.hpp"

template <class T>
struct Prop {
	typedef T alphabet;
	std::string name;
	Prop(std::string name_) : name(name_) {}
	Prop() : name("") {}
};

struct Lab {
	std::string m_name;
	Lab(std::string _name) : m_name(_name) {}
};
//
//void add_sequence_unittest()
//{
//  trie<std::vector<offset_t>, char, size_t> my_trie;
//  std::string str_gene1 = "ACGTACGT";
//  std::string str_gene2 = "ACGTAAA";
//  std::string str_gene3 = "GCT";
//  int id;
//  id = my_trie.pushSequence(str_gene1.begin(), str_gene1.end(), Prop<char>());
//  id = my_trie.pushSequence(str_gene2.begin(), str_gene2.end(), Prop<char>());
//  id = my_trie.pushSequence(str_gene3.begin(), str_gene3.end(), Prop<char>());
//
//  char chr[] = {'A', 'C', 'T'};
//  id = cheese_.pushSequence(chr, chr + 3, Prop<char>());
//  char chr1[] = {'A', 'C', 'T'};
//  std::vector<char> vec(chr1, chr1 + 3);
//  id = cheese_.pushSequence(vec.begin(), vec.end(), Prop<char>());
//
//  std::list<char> lst;
//  lst.push_back('C');
//  lst.push_back('G');
//  lst.push_back('T');
//  id = cheese_.pushSequence(lst.begin(), lst.end(), Prop<char>());
//
//  std::string str_gene4 = "ACGCATC";
//  id = cheese_.pushSequence(str_gene4.begin(), str_gene4.end(), Prop<char>());
//
//  std::vector<trie_node<char>*> vct = cheese_.getPath(id);
//}
//
//void read_fasta_unittest() {
//	clock_t t0 = clock();
//	contig<char, Prop> my_contig;
//	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
//	Read tmp;
//	while(!FR.is_eof()) {
//		FR >> tmp;
//		int id = my_contig.pushSequence(tmp.seq.begin(), tmp.seq.end(), Prop<char>(tmp.name));
//		auto vct = my_contig.getPath(id);
//	}
//	std::cout << my_contig[158].second.name << std::endl;
//	clock_t t1 = clock();
//	std::cout << "time_read_fasta_unittest: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
//}

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
	int seq_count = 20;
	int seq_size = 200;
	IG::annotation<char, Prop, Lab, size_t> annot_;
	for(int s = 0; s<seq_count; s++) {
		Lab label("Bar");
		annot_.push_back(label, seq_size);
		for(int i = 0; i<seq_size; i++) {
			annot_.insert_back(i, (Prop<char>("Foo")));
		}
	}
	Lab record_data = annot_.getRecordData(5);
	Prop<char> node_data = annot_.getNodeData(5,1);
}

void kstat_unittest(void) {
	clock_t t0 = clock();
	IG::annotation<char, Prop, Lab, size_t> annot_;
	std::vector<unsigned char> alphabet_;
	alphabet_.push_back('A');
	alphabet_.push_back('C');
	alphabet_.push_back('G');
	alphabet_.push_back('T');
	alphabet_.push_back('N');
	int K_ = 7;
	IG::kstat<size_t> kstat_(alphabet_, K_);

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
			if(!kstat_.add(iter, end, 0))
				break;
		}
	}
	clock_t t1 = clock();
	std::cout << "time_kstat_unittest: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
	std::string tst = "AGCCTGG";
	std::vector<size_t>* result = kstat_.get(tst.begin(), tst.end());
	std::cout << &result[150] << std::endl;
}

void contig_unittest (void) {
	std::vector<unsigned char> alphabet_;
	alphabet_.push_back('A');
	alphabet_.push_back('C');
	alphabet_.push_back('G');
	alphabet_.push_back('T');
	alphabet_.push_back('N');
	int K_ = 7;
	IG::contig<char, Prop, Lab> my_contig(alphabet_, K_);
	std::string str1 = "ACGCTAGCTG";
	my_contig.pushSequence(str1.begin(), str1.end(), Lab("New label 1"));
	std::string str2 = "ACGCTAGCTC";
	my_contig.pushSequence(str2.begin(), str2.end(), Lab("New label 2"));
	std::string str3 = "AAGCTAGCTC";
	my_contig.pushSequence(str3.begin(), str3.end(), Lab("New label 3"));
}

int main()
{
	contig_unittest();
	annotation_unittest();
	kstat_unittest();
	//add_sequence_unittest();
	//read_fasta_unittest();
}