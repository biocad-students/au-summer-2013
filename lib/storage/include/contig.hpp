#pragma once
// ContIG - Container ImmunoGlobulin

#include <map>
#include <vector>
#include "annotation.hpp"
#include "kstat.hpp"
#include "trie.hpp"

namespace IG {
// typedef T - property param
// typedef Property - annotation stored type
// typedef Label - record label type
// typedef Link - index 1D offset type
template <class T, template <class> class Property, class Label = std::string, class Link = size_t>
class contig
{
public:
	typedef std::pair<Link, Link> offset_t;
	typedef typename trie<std::vector<offset_t>, char, Link> trie_t;
	typedef typename annotation<T, Property, Label, Link> annotation_t;
	typedef typename kstat<Link> kstat_t;
	typedef std::vector<unsigned char> alphabet_t;

public:
	contig(alphabet_t _alphabet, size_t _K) : m_kstat(_alphabet, _K) {
	}

	~contig() {
	}

	template <class Iterator>
	void pushSequence(Iterator _begin, Iterator _end, Label _label) {
		Iterator iter = _begin;
		trie_t::iterator lastTrie = m_trie.root();
		m_annotation.push_back(_label, std::distance(_begin, _end));	
			
		while(iter != _end) {
			// trie add
			lastTrie = m_trie.insert(lastTrie, *iter);
			// kstat add
			if(_end-7 >= iter) {
				m_kstat.add(iter, iter+7, lastTrie.index());
			}
			// annotation add
			m_annotation.insert_back(lastTrie.index(), (Prop<char>("")));
			Link record = m_annotation.size()-1;
			Link node = m_annotation.record_size(record);

			// TODO: add link to annotation
			// *lastTrie = offset_t(record, node);

			++iter;
		}
	}

	template <class Iterator>
	std::vector<Label> find(Iterator _begin, Iterator _end) {
	}

private:
	annotation_t m_annotation;
	trie_t m_trie;
	kstat_t m_kstat;
};
}