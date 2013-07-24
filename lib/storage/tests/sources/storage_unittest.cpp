#include "../include/unittest.h"

void storage_unittest (void ) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin storage_unittest");
	std::string path = "../../../data/germline/human/VH.fasta";
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::storage<char, Prop, Lab> my_storage(alphabet_, K_);

	FastaReader FR(path);
	Read tmp;
	std::string::iterator iter;
	std::string::iterator end;
	while(!FR.is_eof()) {
		FR >> tmp;
		iter = tmp.seq.begin();
		end = tmp.seq.end();
		my_storage.pushSequence(iter, end, Lab(tmp.name));
	}
	PERFOMANCE_TIME("End storage_unittest");
}