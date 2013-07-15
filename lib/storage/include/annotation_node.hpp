#pragma once

#include <vector>

namespace IG {

	template <class T, template <class> class Property, class Link = size_t>
class annotation_node {
public:
	typedef Property<T> property_t;
	
	annotation_node (size_t _trie_node_idx) : m_trie_node_idx(_trie_node_idx) {
	}

	~annotation_node () {
	}

	property_t & operator*()
	{
		return m_internal;
	}

private:
	property_t m_internal;
	Link m_trie_node_idx;
};
}