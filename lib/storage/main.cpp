#if 0
#include <vector>
#include "tests/include/unittest.h"

clock_t g_begin;

void fill_alphabet(std::vector<unsigned char>* _alphabet) {
	_alphabet->push_back('A');
	_alphabet->push_back('C');
	_alphabet->push_back('G');
	_alphabet->push_back('T');
}
#endif

#include "tests/include/unittest.h"

clock_t g_begin;

int main(void)
{
#if 0
	matrix_unittest();
	annotation_unittest();
	kstat_unittest();
	storage_unittest();
	find_unittest();
	align_unittest();
#else
#endif
        run_storage_unittests();
        run_annotation_unittests();
        run_kstat_unittests();
        run_trie_unittests();
        run_trie_topology_unittests();
}
