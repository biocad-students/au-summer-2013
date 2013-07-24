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

void annotation_unittest(std::string _path) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin annotation_unittest");
	igc::annotation<char, Prop, Lab> annot_;

	FastaReader FR(_path);
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		Lab label(tmp.name);
		annot_.push_back(label, tmp.seq.size());
		for(int i = 0; i<tmp.seq.size(); i++) {
			annot_.insert_back(i, (Prop<char>("Foo")));
		}
	}
	Lab record_data = annot_.getRecordData(5);
	Prop<char> node_data = annot_.getNodeData(5,1);
	PERFOMANCE_TIME("End annotation_unittest");
}

void kstat_unittest(std::string _path) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin kstat_unittest");
	igc::annotation<char, Prop, Lab> annot_;
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::Kstat kstat_(alphabet_, K_);

	FastaReader FR(_path);
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
	PERFOMANCE_TIME("End kstat_unittest");
}

void storage_load_unittest (std::string _path, igc::storage<char, Prop, Lab>& _storage) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin storage_load_unittest");

	FastaReader FR(_path);
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		iter = tmp.seq.begin();
		end = tmp.seq.end();
		_storage.pushSequence(iter, end, Lab(tmp.name));
	}
	PERFOMANCE_TIME("End storage_load_unittest");
}

void find_unittest (std::string _path, igc::storage<char, Prop, Lab>& _storage) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin find_unittest");
	typedef igc::storage<char, Prop, Lab> my_storage_t;

	std::string needle("AAGTGCAGCTGGTGCAGTCTGGGGGAGGCTTGGTGCAGC");
	my_storage_t::iterator it = _storage.find(needle.begin(), needle.end());
	for(; it.valid(); ++it)
	{
		std::vector<Lab> labels = it.get_labels();
	}
	PERFOMANCE_TIME("End find_unittest");
}

void align_unittest(std::string _path, igc::storage<char, Prop, Lab>& _storage)
{
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin align_unittest");
	typedef igc::storage<char, Prop, Lab> my_storage_t;
	std::string alstr = "ACGTATAC";
	_storage.align(alstr.begin(), alstr.end());
	PERFOMANCE_TIME("End align_unittest");
}

void matrix_unittest()
{
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin matrix_unittest");
	std::string BLOSUM62 = "../../../data/BLOSUM62.txt";
	matrix m(BLOSUM62);
	int a = m('A', 'A');
	int r = m('A', 'R');
	int n = m('A', 'N');
	int d = m('A', 'D');
	PERFOMANCE_TIME("End matrix_unittest");
}

int main()
{
	std::string path = "../../../data/germline/human/VH_corrected.fasta";
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::storage<char, Prop, Lab> my_storage(alphabet_, K_);

	matrix_unittest();
	kstat_unittest(path);
	annotation_unittest(path);
	storage_load_unittest(path, my_storage);
	find_unittest(path, my_storage);
	align_unittest(path, my_storage);
}
