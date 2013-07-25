#include <vector>
#include "tests/include/unittest.h"

clock_t g_begin;

void fill_alphabet(std::vector<unsigned char>* _alphabet) {
	_alphabet->push_back('A');
	_alphabet->push_back('C');
	_alphabet->push_back('G');
	_alphabet->push_back('T');
}

int main(void)
{
	matrix_unittest();
	annotation_unittest();
	kstat_unittest();
	storage_unittest();
	find_unittest();
	align_unittest();
}