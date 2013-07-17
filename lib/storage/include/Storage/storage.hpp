#pragma once
// IGC - ImmunoGlobulin Container

#include <map>
#include <vector>
#include "Annotation\annotation.hpp"
#include "Kstat\kstat.hpp"
#include "Trie\trie.hpp"

namespace IGC {
// typedef T - property param
// typedef Property - annotation stored type
// typedef Label - record label type
// typedef Link - index 1D offset type
template <class T, template <class> class Property, class Label = std::string, class Link = size_t>
class Storage
{
public:
	typedef std::pair<Link, Link> offset_t;
	typedef typename trie<std::vector<offset_t>, char, Link> trie_t;
	typedef typename annotation<T, Property, Label, Link> annotation_t;
	typedef typename kstat<Link> kstat_t;
	typedef std::vector<unsigned char> alphabet_t;
	typedef typename trie_t::iterator iterator;
	typedef typename trie_t::const_iterator const_iterator;
public:

	iterator begin() {
		return m_trie.begin();
	}

	const_iterator begin () const {
		return m_trie.begin();
	}

	iterator end() {
		return m_trie.end();
	}

	const_iterator end () const {
		return m_trie.end();
	}

	iterator find(size_t _idx) {
		return m_trie.find(_idx);
	}

	const_iterator find(size_t _idx) const {
		return m_trie.find(_idx);
	}

	Storage(alphabet_t _alphabet, size_t _K) : m_kstat(_alphabet, _K) {
	}

	~Storage() {
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
			offset_t tmp = std::make_pair(record, node);
			lastTrie->push_back(tmp);

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