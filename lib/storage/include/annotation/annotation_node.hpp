#pragma once

#include <vector>

namespace igc {

template <class T, template <class> class Property>
class Annotation_node {
public:
	typedef Property<T> property_t;
	
	Annotation_node (size_t _trie_node_idx) : m_trie_node_idx(_trie_node_idx) {
	}

	~Annotation_node () {
	}

	property_t & operator*()
	{
		return m_internal;
	}

private:
	property_t m_internal;
	size_t m_trie_node_idx;
};
}