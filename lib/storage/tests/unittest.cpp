#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

clock_t g_begin;
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000
#endif
#define PERFOMANCE_INIT() g_begin = clock();
#define PERFOMANCE_TIME(label) std::cout << label << " " << (double)(clock() - g_begin)/CLOCKS_PER_SEC << " at line " << __LINE__ << std::endl;

// memory leaks detection
#ifdef WIN32
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif
// end
#include "fasta_reader.h"
#include "storage/storage.hpp"
#include "kstat/kstat.hpp"
#include "annotation/annotation.hpp"
#include "algorithm/algorithm.hpp"
#include "scorematrix.h"

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

void fill_alphabet(std::vector<unsigned char>* _alphabet) {
	_alphabet->push_back('A');
	_alphabet->push_back('C');
	_alphabet->push_back('G');
	_alphabet->push_back('T');
}

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
	igc::annotation<char, Prop, Lab, size_t> annot_;
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
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
	std::string tst = "AGCCTGG";
	std::set<size_t>* result = kstat_.get(tst.begin(), tst.end());
	std::cout << &result[150] << std::endl;
}

void contig_unittest (void) {
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::storage<char, Prop, Lab> my_contig(alphabet_, K_);
	std::string str1 = "NNNACGCTAGCTG";
	my_contig.pushSequence(str1.begin(), str1.end(), Lab("New label 1"));
	std::string str2 = "ACGCTAGCTCNNN";
	my_contig.pushSequence(str2.begin(), str2.end(), Lab("New label 2"));
	std::string str3 = "AAGCTAGCTC";
	my_contig.pushSequence(str3.begin(), str3.end(), Lab("New label 3"));
}

//void find_unittest (void) {
//	std::cout << "start find unittest" << std::endl;
//	clock_t t1 = clock();
//	typedef igc::storage<char, Prop, Lab, size_t> my_storage_t;
//	typedef my_storage_t::iterator iterator;
//	std::map<unsigned char, size_t> alphabet_;
//
//	int K_ = 7;
//	my_storage_t my_storage(alphabet_, K_);
//	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH_corrected.fasta");
//	Read tmp;
//	std::string::iterator iter;
//	std::string::iterator end;
//	while(!FR.is_eof()) {
//		FR >> tmp;
//		iter = tmp.seq.begin();
//		end = tmp.seq.end();
//		my_storage.pushSequence(iter, end, Lab(tmp.name));
//	}
//	std::cout << "readed VH.fasta" << std::endl;
//	std::string needle("TTCCAGGGCAGAGTCA");
//	std::set<size_t> find_result = igc::find(needle.begin(), needle.end(), my_storage);
//	std::set<size_t>::iterator find_iter = find_result.begin();
//	while(find_iter != find_result.end()) {
//		std::cout << *find_iter << " ";
//		++find_iter;
//	}
//	clock_t t2 = clock();
//	std::cout << (double) (t2-t1) / CLOCKS_PER_SEC <<"end find unittest" << std::endl;
//}

void find_unittest2 (void) {
	PERFOMANCE_TIME("Init find unittest");
	typedef igc::storage<char, Prop, Lab, size_t> my_storage_t;
	//typedef my_storage_t::iterator iterator;

	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	my_storage_t my_storage(alphabet_, K_);
	PERFOMANCE_TIME("Reading file");
	FastaReader FR("..\\..\\..\\data\\germline\\human\\VH_corrected.fasta");
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		iter = tmp.seq.begin();
		end = tmp.seq.end();
		my_storage.pushSequence(iter, end, Lab(tmp.name));
	}
	PERFOMANCE_TIME("End reading file");
	PERFOMANCE_TIME("Find processing");
	std::string needle("AAGTGCAGCTGGTGCAGTCTGGGGGAGGCTTGGTGCAGC");
	my_storage_t::iterator it = my_storage.find(needle.begin(), needle.end());
	PERFOMANCE_TIME("End finding");
	for(; it.valid(); ++it)
	{
		std::vector<Lab> labels = it.get_labels();
		//for(int i=0; i < labels.size(); ++i)
		//	std::cout << labels[i].m_name << std::endl;
		std::cout << labels.size() << std::endl;
		std::cout << std::endl;
	}
	PERFOMANCE_TIME("End test");
}

int main()
{
	PERFOMANCE_INIT();
	find_unittest2();
	//find_unittest();
	//contig_unittest();
	//annotation_unittest();
	//kstat_unittest();
	std::string BLOSUM62 = "../../../data/BLOSUM62.txt";
	matrix m(BLOSUM62);
	int a = m('A', 'A');
	int b = m('A', 'C');
	int c = m('C', 'G');
	int d = m('A', 'T');
}
