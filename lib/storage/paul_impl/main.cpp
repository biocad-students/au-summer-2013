#include <iostream>
#include "unittest.h"

using namespace std;

void test()
{
    test_trie();
    test_anno();
    test_kstat();
    test_contig();
    test_contig2();
    test_search();
}

int main()
{
    test();
    return 0;
}
