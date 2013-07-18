#include <set>
#include "Storage\storage.hpp"

namespace igc {

template<class Iterator, class T, template <class> class Property, class Label, class Link>
std::set<Link> find (Iterator _begin, Iterator _end, storage<T, Property, Label, Link> & storage) {	
	typedef typename std::set<Link> result_t;
	typedef typename trie<char, char, Link> trie_t;
	typedef typename trie_t::iterator iterator;
	typedef typename trie_t::const_iterator const_iterator;
	typedef typename Kstat<Link> kstat_t;
	result_t result;

	trie_t my_color_trie = storage.copyTrie<char, char, Link>();
	
	kstat_t my_kstat = storage.get_kstat();
	std::fill(my_color_trie.begin(), my_color_trie.end(), false);

	size_t dist = std::distance(_begin, _end);
	// окраска дерева
	for(size_t i = 0; i < dist - 7; ++i, ++_begin) {
		result_t *similar = my_kstat.get(_begin, _begin+7);
		result_t::iterator similar_iter = similar->begin();
		while(similar_iter != similar->end()) {
			iterator iter = my_color_trie.find(*similar_iter);
			*iter = true;
			++similar_iter;
		}
	}

	// поиск по окрашенному дереву
	result_t *similar = my_kstat.get(_begin, _begin+7);
	result_t::iterator similar_iter = similar->begin();
	while(similar_iter != similar->end()) {
		const_iterator iter = my_color_trie.find(*similar_iter);
		bool found = true;
		for(int c = dist - 7; c > 0; --c) {
			--iter;
			if(!*iter) {
				found = false;
				break;
			}
		}
		if(found) {
			result.insert(iter.index());
		}
		++similar_iter;
	}
	return result;
}
}