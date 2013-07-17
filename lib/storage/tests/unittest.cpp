#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

// memory leaks detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// end
#include "fasta_reader.h"
#include "Storage\storage.hpp"
#include "Kstat\kstat.hpp"
#include "Annotation\annotation.hpp"
#include "Algorithm\algorithm.hpp"

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

void annotation_unittest(void ) {
	int seq_count = 20;
	int seq_size = 200;
	igc::annotation<char, Prop, Lab, size_t> annot_;
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
	igc::annotation<char, Prop, Lab, size_t> annot_;
	std::vector<unsigned char> alphabet_;
	alphabet_.push_back('A');
	alphabet_.push_back('C');
	alphabet_.push_back('G');
	alphabet_.push_back('T');
	int K_ = 7;
	igc::Kstat<size_t> kstat_(alphabet_, K_);

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
	int K_ = 7;
	igc::storage<char, Prop, Lab> my_contig(alphabet_, K_);
	std::string str1 = "NNNACGCTAGCTG";
	my_contig.pushSequence(str1.begin(), str1.end(), Lab("New label 1"));
	std::string str2 = "ACGCTAGCTCNNN";
	my_contig.pushSequence(str2.begin(), str2.end(), Lab("New label 2"));
	std::string str3 = "AAGCTAGCTC";
	my_contig.pushSequence(str3.begin(), str3.end(), Lab("New label 3"));
}
void find_unittest (void) {
	clock_t t0 = clock();
	typedef igc::storage<char, Prop, Lab, size_t> my_storage_t;
	typedef my_storage_t::iterator iterator;

	std::vector<unsigned char> alphabet_;
	alphabet_.push_back('A');
	alphabet_.push_back('C');
	alphabet_.push_back('G');
	alphabet_.push_back('T');
	int K_ = 7;
	my_storage_t my_storage(alphabet_, K_);

	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH.fasta");
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		iter = tmp.seq.begin();
		end = tmp.seq.end();
		my_storage.pushSequence(iter, end, Lab(tmp.name));
	}

	std::string needle("TTCCAGGGCAGAGTCA");
	std::vector<size_t> find_result = igc::find(needle.begin(), needle.end(), my_storage);

	for(int i = 0; i != find_result.size(); ++i) {
		std::cout << find_result[i] << " ";
	}
	clock_t t1 = clock();
	std::cout << "time_find_unittest: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;

}

int main()
{
	find_unittest();
	contig_unittest();
	annotation_unittest();
	kstat_unittest();
	_CrtDumpMemoryLeaks();
}