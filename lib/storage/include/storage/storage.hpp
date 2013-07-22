#pragma once
// IGC - ImmunoGlobulin Container

#include <map>
#include <vector>
#include "Annotation/annotation.hpp"
#include "Kstat/kstat.hpp"
#include "Trie/trie.hpp"

namespace igc {
// typedef T - property param
// typedef Property - annotation stored type
// typedef Label - record label type
// typedef Link - index 1D offset type
template <class T, template <class> class Property, class Label = std::string, class Link = size_t>
class storage
{
public:
	typedef storage<T, Property, Label, Link> _My_type;
	typedef std::pair<Link, Link> offset_t;
	typedef typename trie<std::vector<offset_t>, char, Link> trie_t;
	typedef typename annotation<T, Property, Label, Link> annotation_t;
	typedef typename Kstat<Link> kstat_t;
	typedef std::map<unsigned char, size_t> alphabet_t;

	class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
	{
	public:
		const_iterator(_My_type &_storage, std::vector<int>& _data)
			: m_storage(_storage), m_data(_data), m_index(0)
		{
		}

		const_iterator(const const_iterator &_right)
			: m_storage(_right.m_storage), m_data(_right.m_data), m_index(_right.m_index)
		{
		}

		const_iterator& operator = (const const_iterator &_right)
		{
			m_storage = _right.m_storage;
			m_data = _right.m_data;
			m_index = _right.m_index;
			return *this;
		}

		bool valid() const
		{
			return m_index >= 0 && m_index < m_data.size();
		}

		std::vector<Label> get_labels()
		{
			std::vector<Label> result;
			trie_t::iterator it = m_storage.m_trie.find(m_data[m_index]);
			for(int i=0; i<(*it).size(); ++i)
			{
				int top = (*it)[i].first;
				result.push_back(m_storage.m_annotation.getRecordData(top));
			}
			return result;
		}

		const T& operator*() const
		{
		}

		const T* operator->() const
		{
			return (&**this);
		}

		//bool operator == (const const_iterator& _right) const
		//{
		//	//return m_node == _right.m_node;
		//}

		//bool operator!=(const const_iterator& _right) const
		//{
		//	return (!(*this == _right));
		//}

		const_iterator& operator++()
		{
			++m_index;
			return *this;
		}

		//const_iterator operator++(int)
		//{
		//	_My_type_iter _Tmp = *this;
		//	++*this;
		//	return _Tmp;
		//}

		//const_iterator& operator--()
		//{
		//	--m_index;
		//	return *this;
		//}

		//const_iterator operator--(int)
		//{
		//	_My_type_iter _Tmp = *this;
		//	--*this;
		//	return _Tmp;
		//}

    //Key_type key() const
    //{
    //  return m_node->cached_key;
    //}

    //Index_type index() const
    //{
    //  return m_node->index;
    //}

	private:
		_My_type m_storage;
		std::vector<int> m_data;
		int m_index;
	};





	class iterator : public const_iterator
	{
	public:
		iterator(_My_type& _storage, std::vector<int>& _data)
			: const_iterator(_storage, _data)
		{
		}

		T& operator*() const
		{	// return designated value
			return ((Node&)**(const_iterator *)this);
		}

		T* operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		iterator& operator++()
		{	// preincrement
			++(*(const_iterator *)this);
			return (*this);
		}

		iterator operator++(int)
		{	// postincrement
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		//iterator& operator--()
		//{	// preincrement
		//	--(*(const_iterator *)this);
		//	return (*this);
		//}

		iterator operator--(int)
		{	// postincrement
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};

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





	template <class Iterator>
	iterator find(Iterator _begin, Iterator _end) {
		std::vector<int> result;

		trie<char, char, Link> my_color_trie(m_trie);
		std::fill(my_color_trie.begin(), my_color_trie.end(), false);

		size_t dist = std::distance(_begin, _end);
		for(size_t i = 0; i < dist - 7; ++i, ++_begin)
		{
			std::set<Link> *similar = m_kstat.get(_begin, _begin + 7);
			if(similar == NULL)
				continue;
			std::set<Link>::iterator similar_iter = (*similar).begin();
			while(similar_iter != similar->end()) {
				trie<char, char, Link>::iterator iter = my_color_trie.find(*similar_iter);
				*iter = true;
				++similar_iter;
			}
		}
		std::set<Link> *similar = m_kstat.get(_begin, _begin + 7);
		typename std::set<Link>::iterator similar_iter = similar->begin();
		for(;similar_iter != similar->end(); ++similar_iter)
		{
			trie<char, char, Link>::const_iterator iter = my_color_trie.find(*similar_iter);
			trie<char, char, Link>::const_iterator last_iter = iter;
			bool found = true;
			for(int c = dist - 7; c > 0; --c)
			{
				--iter;
				if(iter == my_color_trie.root() || !*iter)
				{
					found = false;
					break;
				}
			}
			if(found)
			{
				result.push_back( last_iter.index() );
			}
		}

		return iterator(*this, result);
	}



private:
	annotation_t m_annotation;
	trie_t m_trie;
	kstat_t m_kstat;
};
}
