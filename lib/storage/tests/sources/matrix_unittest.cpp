#include "../include/unittest.h"

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