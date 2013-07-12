#pragma once

#include <vector>

#include "trie/trie.hpp"
#include "contig.hpp"


template <class Iterator, class T, template <class> class Property, class LabelType>
std::vector<typename contig<T, Property, LabelType>::iterator> search(Iterator begin, Iterator end,
															 contig<T, Property, LabelType> const & c)
{
	std::vector<typename contig<T, Property, LabelType>::iterator> result;
	trie<bool>* tmp = nullptr;
	c.copyTrie(&tmp);

	//TODO: Search implementation here (Pavel)

	delete tmp;
	return result;
}
