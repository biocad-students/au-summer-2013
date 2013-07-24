#include "../include/unittest.h"

void find_unittest (void ) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin find_unittest");
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
	std::string needle("AAGTGCAGCTGGTGCAGTCTGGGGGAGGCTTGGTGCAGC");
	igc::storage<char, Prop, Lab>::iterator it = my_storage.find(needle.begin(), needle.end());
	for(; it.valid(); ++it)
	{
		std::vector<Lab> labels = it.get_labels();
	}
	PERFOMANCE_TIME("End find_unittest");
}