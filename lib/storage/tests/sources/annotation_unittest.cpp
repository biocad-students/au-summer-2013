#include "../include/unittest.h"

void annotation_unittest(void ) {
	PERFOMANCE_INIT();
	PERFOMANCE_TIME("Begin annotation_unittest");
	igc::annotation<char, Prop, Lab> annot_;
	std::string path = "../../../data/germline/human/VH.fasta";

	FastaReader FR(path);
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