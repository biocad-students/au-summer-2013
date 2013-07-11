#include "trie.h"
//#include "FASTA_reader.h"
#include <list>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>

#if 0
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

void find_substring_unittest()
{
  struct Struct
  {
    int k;
  };
  cheese<Struct, int/*std::string*/> cheese_;

  std::string str_gene1 = "ACGTACGT";
  cheese_.add(str_gene1.begin(), str_gene1.end(), 1);
  std::string str_gene2 = "ACGCACGT";
  cheese_.add(str_gene2.begin(), str_gene2.end(), 2);
  std::string str_gene3 = "ACGCCCGT";
  cheese_.add(str_gene3.begin(), str_gene3.end(), 3);

  std::string needle = "ACG";

  //fill(cheese_, ... );
  cheese<Struct, int>::iterator iter = cheese_.start_BFS();
  while(iter != cheese_.end())
  {
//    iter->_value = 0;
  }

//  iter = cheese_.start_BFS();
//  iter->_value = 0;
  ++iter;
  while(iter != cheese_.end())
  {
//    int k = (iter-1)->k;
//    if(needle[k] == iter.key())
//    {
//      iter->k = k + 1;
//    }
    ++iter;
  }


}
#endif



#if 0
template<class Iterator>
void find(Iterator begin, Iterator end)
{
    trie<bool> color_trie(m_trie);
	std::fill(color_trie/*.DFS*/.begin(), color_trie/*.DFS*/.end(), false);
	size_t count = std::distance(begin, end) - 7;
	for(size_t i = 0; i != count; ++i)
	{
	    k_stat::value_type anno_cont = m_kstat.find(begin + i);
	    k_stat::value_type::const_iterator anno_iter = anno_cont.begin();
		for(; anno_iter != anno_cont.end(); ++anno_iter)
		{
		    color_trie[*anno_iter] = true;
		}
	}


	k_stat::value_type anno_cont = m_kstat.find(begin + count);
	k_stat::value_type::const_iterator anno_iter = anno_cont.begin();
	for(; anno_iter != anno_cont.end(); ++anno_iter)
	{
		bool found = true;
	    color_trie::itertor iter(*anno_iter);
		do
		{
			--iter;
			if(!*iter)
			{
				found = false;
				break;
			}
		}
		while(count != 0);

		if(found)
		{
			//visitor(*iter);
		}
	}
}
#endif


void general_test()
{
  struct Struct
  {
    int A, B, C;
    std::string Str;
  };

  trie<Struct, char> my_trie;
  my_trie.reserve(1000);

  std::string str_gene1 = "ACGTACGT";
  my_trie.add(str_gene1.begin(), str_gene1.end());
  std::string str_gene2 = "ACGCACGT";
  my_trie.add(str_gene2.begin(), str_gene2.end());
  std::string str_gene3 = "ACGCCCGT";
  my_trie.add(str_gene3.begin(), str_gene3.end());

  char chr[] = {'A', 'C', 'T'};
  my_trie.add(chr, chr + 3);
  char chr1[] = {'A', 'C', 'T'};
  std::vector<char> vec(chr1, chr1 + 3);
  my_trie.add(vec.begin(), vec.end());
  std::list<char> lst;
  lst.push_back('C');
  lst.push_back('G');
  lst.push_back('T');
  trie<Struct, char>::iterator iter = my_trie.add(lst.begin(), lst.end());
  while(iter != my_trie.end())
  {
    std::cout << iter.key();
    --iter;
  }
  std::cout << std::endl;

  std::string str_gene4 = "ACGCATC";
  iter = my_trie.add(str_gene4.begin(), str_gene4.end());
  while(iter != my_trie.end())
  {
    static int counter = 0;
    std::cout << iter.key();
    iter->A = ++counter;
    iter--;
  }
  std::cout << std::endl;


  /*iter = cheese_[7];
  while(iter != cheese_.end())
  {
    std::cout << iter.key() << " " << iter->A << std::endl;
    iter++;
  }
  std::cout << std::endl; */


  trie<int, char> my_color_trie(my_trie);
  trie<int, char>::iterator iter1 = my_color_trie.begin();
  while(iter1 != my_color_trie.end())
  {
    std::cout << iter1.key() << " " << *iter1 << std::endl;
    ++iter1;
  }
  std::cout << std::endl;

  std::fill(my_color_trie.begin(), my_color_trie.end(), 2);
  for(iter1 = my_color_trie.begin(); iter1 != my_color_trie.end(); ++iter1)
  {
    std::cout << iter1.key() << " " << *iter1 << std::endl;
  }
  std::cout << std::endl;

  *my_color_trie.find(3) = 1;
  *my_color_trie.find(5) = 1;
  *my_color_trie.find(10) = 1;
  *my_color_trie.find(20) = 1;

  iter1 = my_color_trie.begin();
  while(iter1 != my_color_trie.end())
  {
    std::cout << iter1.key() << " " << *iter1 << std::endl;
    ++iter1;
  }
  std::cout << std::endl;
}

void find_test()
{
  struct Struct
  {
    int A, B, C;
    std::string Str;
  };

  trie<Struct, char> my_trie;
  my_trie.reserve(1000);

  std::string str_gene1 = "ACGTGCAT";
  my_trie.add(str_gene1.begin(), str_gene1.end());
  std::string str_gene2 = "ACGCACGT";
  my_trie.add(str_gene2.begin(), str_gene2.end());
  std::string str_gene3 = "ACGCCCGT";
  my_trie.add(str_gene3.begin(), str_gene3.end());

  std::multimap<char, int> k1_mers;
  for(trie<Struct, char>::iterator iter = my_trie.begin(); iter != my_trie.end(); ++iter)
  {
    k1_mers.insert(std::pair<char, int>(iter.key(), iter.index() ) );
  }

  // looking for "GCA"
  std::string needle("GCA");

  trie<std::set<int>, char> my_color_trie(my_trie);
  std::fill(my_color_trie.begin(), my_color_trie.end(), std::set<int>());

  std::multimap<char,int>::iterator it;
  for(it = k1_mers.begin(); it != k1_mers.end(); ++it)
      std::cout << (*it).first << " => " << (*it).second << std::endl;


  for(int i=0; i<needle.size() - 1; ++i)
  {
    std::pair<std::multimap<char,int>::iterator, std::multimap<char,int>::iterator> range = k1_mers.equal_range(needle[i]);
    for(it = range.first; it != range.second; ++it)
    {
      (*my_color_trie.find((*it).second)).insert(i);
    }
  }

  std::pair<std::multimap<char,int>::iterator, std::multimap<char,int>::iterator> range =
        k1_mers.equal_range(needle[needle.length()-1]);
  for(it = range.first; it != range.second; ++it)
  {
    int length = needle.length()-2;
    trie<std::set<int>, char>::iterator iter = my_color_trie.find((*it).second);
    bool found = true;
    --iter;
    while(length >= 0)
    {
      if(iter == my_color_trie.end() || (*iter).count(length) == 0)
      {
        found = false;
        break;
      }
      --length;
      --iter;
    }
    

    if(found)
    {
      std::cout << "found at: " << my_color_trie.find((*it).second).index() << std::endl;
    }
  }



}

int main()
{
  //add_sequence_unittest();
  //readFASTA_unittest();
  //find_substring_unittest();
  general_test();
  find_test();
}