#include "cheese-trie.h"
#include "FASTA_reader.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#define cheese trie



void add_sequence_unittest()
{
  struct Struct
  {
    int A, B, C;
  };
  cheese<Struct, int/*std::string*/> cheese_;
  std::string str_gene1 = "ACGTACGT";
  std::string str_gene2 = "ACGTAAA";
  std::string str_gene3 = "GCT";
  cheese_.add(str_gene1.begin(), str_gene1.end(), 1/*"gene from string 1"*/);
  cheese_.add(str_gene2.begin(), str_gene2.end(), 2/*"gene from string 2"*/);
  cheese_.add(str_gene3.begin(), str_gene3.end(), 3/*"gene from string 3"*/);
  char chr[] = {'A', 'C', 'T'};
  cheese_.add(chr, chr + 3, 4/*"gene from char[]"*/);
  char chr1[] = {'A', 'C', 'T'};
  std::vector<char> vec(chr1, chr1 + 3);
  cheese_.add(vec.begin(), vec.end(), 5/*"gene from vector"*/);
  std::list<char> lst;
  lst.push_back('C');
  lst.push_back('G');
  lst.push_back('T');
  cheese<Struct, int/*std::string*/>::iterator iter = cheese_.add(lst.begin(), lst.end(), 6/*"gene from list"*/);
  while(iter != cheese_.end())
  {
    std::cout << iter.key();
    ++iter;
  }
  std::cout << std::endl;

  std::string str_gene4 = "ACGCATC";
  iter = cheese_.add(str_gene4.begin(), str_gene4.end(), 7/*"gene from string 4"*/);
  while(iter != cheese_.end())
  {
    static int counter = 0;
    std::cout << iter.key();
    iter->A = ++counter;
    iter++;
  }
  std::cout << std::endl;

  iter = cheese_[7/*"gene from string 4"*/];
  while(iter != cheese_.end())
  {
    std::cout << iter.key() << " " << iter->A << std::endl;
    iter++;
  }
  std::cout << std::endl;
}

void readFASTA_unittest()
{
  struct Struct
  {
    int A, B, C;
  };
  cheese<Struct, int/*std::string*/> cheese_;

  std::ifstream fsIn("input.fasta");
	if(fsIn.is_open())
		readFASTA(&fsIn, &cheese_);
}

int main()
{
  add_sequence_unittest();
  readFASTA_unittest();
}