#include "../include/unittest.h"

void kstat_unittest(void) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin kstat_unittest");

	igc::annotation<char, Prop, Lab> annot_;
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::Kstat kstat_(alphabet_, K_);
	std::string path = "../../../data/germline/human/VH.fasta";

	FastaReader FR(path);
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