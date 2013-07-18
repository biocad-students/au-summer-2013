#pragma once
// IGC - ImmunoGlobulin Container

#include <map>
#include <vector>
#include "Annotation\annotation.hpp"
#include "Kstat\kstat.hpp"
#include "Trie\trie.hpp"

namespace igc {
// typedef T - property param
// typedef Property - annotation stored type
// typedef Label - record label type
// typedef Link - index 1D offset type
template <class T, template <class> class Property, class Label = std::string, class Link = size_t>
class storage
{
public:
	typedef std::pair<Link, Link> offset_t;
	typedef typename trie<std::vector<offset_t>, char, Link> trie_t;
	typedef typename annotation<T, Property, Label, Link> annotation_t;
	typedef typename Kstat<Link> kstat_t;
	typedef std::map<unsigned char, size_t> alphabet_t;
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

	storage(alphabet_t _alphabet, size_t _K) : m_kstat(_alphabet, _K) {
	}

	~storage() {
	}

	template <class Iterator>
	void pushSequence(Iterator _begin, Iterator _end, Label _label) {
		Iterator iter = _begin;
		trie_t::iterator lastTrie = m_trie.root();
		m_annotation.push_back(_label, std::distance(_begin, _end));	
		while(iter != _end) {
			lastTrie = m_trie.insert(lastTrie, *iter);
			if(_end-7 >= iter) {
				m_kstat.add(iter, iter+7, lastTrie.index());
			}
			m_annotation.insert_back(lastTrie.index(), (Prop<char>("")));
			lastTrie->push_back(std::make_pair(m_annotation.size()-1, m_annotation.record_size(m_annotation.size()-1)));
			++iter;
		}
	}

	template<typename T, typename Key_type, typename Link>
	trie<T, Key_type, Link> copyTrie () {
		trie<T, Key_type, Link> tmp = m_trie;
		return tmp;
	}

	kstat_t get_kstat() {
		return m_kstat;
	}

	size_t getK(void) {
		return m_kstat.getK();
	}

private:
	annotation_t m_annotation;
	trie_t m_trie;
	kstat_t m_kstat;
};
}