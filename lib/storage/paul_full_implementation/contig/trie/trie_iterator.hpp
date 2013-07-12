#pragma once

#include <iterator>

#include "trie.hpp"

template <class> class trie;

template <class T>
class trie_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
public:
	friend class trie<T>;

	trie_iterator() : m_trie(nullptr), m_current(-1)
	{
	}

	trie_iterator(trie<T>* t, size_t current = 0) : m_trie(t), m_current(current)
	{
	}

	byte symbol()
	{
		trie_node* node = m_trie->get(m_current);
		return node ? node->symbol() : 0;
	}

	trie_iterator next(byte symbol)
	{
		return trie_iterator(m_trie, m_trie->next(symbol));
	}

	trie_iterator prev()
	{
		return trie_iterator(m_trie, m_trie->prev());
	}

	trie_iterator & operator++()
	{
		m_trie->createDfsCache();
		m_current = m_trie->dfsNext(m_current);
		return *this;
	}

	trie_iterator operator++(int)
	{
		m_trie->createDfsCache();
		trie_iterator tmp = *this;
		m_current = m_trie->dfsNext(m_current);
		return tmp;
	}

	trie_iterator & operator--()
	{
		m_current = m_trie->prev(m_current);
		return *this;
	}

	trie_iterator operator--( int)
	{
		trie_iterator tmp = *this;
		m_current = m_trie->prev(m_current);
		return tmp;
	}

	trie_iterator operator+(size_t i)
	{
		size_t new_current = m_current;
		for (size_t k = 0; k < i; ++k)
		{
			new_current = m_trie->dfsNext(new_current);
		}
		return trie_iterator(m_trie, new_current);
	}

	trie_iterator operator-(size_t i)
	{
		size_t new_current = m_current;
		for (size_t k = 0; k < i; ++k)
		{
			new_current = m_trie->prev(new_current);
		}
		return trie_iterator(m_trie, new_current);
	}

	T & operator*()
	{
		return (*m_trie)[m_current];
	}

	T* operator->()
	{
		return &(**this);
	}

	bool operator==(trie_iterator<T> iter)
	{
		if (m_current < m_trie->size())
			return m_current == iter.m_current;
		return iter.m_current > m_trie->size();
	}

	bool operator!=(trie_iterator<T> iter)
	{
		return !(*this == iter);
	}

	size_t index()
	{
		return m_current;
	}

private:
	trie<T>*  m_trie;
	size_t    m_current;
};
