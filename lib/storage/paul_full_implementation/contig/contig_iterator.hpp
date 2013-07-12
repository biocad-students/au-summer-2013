#pragma once

#include <iterator>
#include <string>

#include "trie/trie_iterator.hpp"
#include "contig.hpp"

template <class T, template <class> class Property, class LabelType> class contig;

template <class T, template <class> class Property, class LabelType = std::string>
class contig_iterator : public std::iterator<std::bidirectional_iterator_tag,
										     Property<T>, std::ptrdiff_t,
										     const Property<T>*,
										     const Property<T>&>
{
public:
	friend class contig<T, Property, LabelType>;
	typedef contig<T, Property, LabelType> contig_t;

	contig_iterator() : m_contig(nullptr), m_iter()
	{
	}

	contig_iterator(contig_t* c, typename contig_t::iterator iter)
		: m_contig(c), m_iter(iter)
	{
	}

	byte symbol() const
	{
		return m_iter.symbol();
	}

	contig_iterator next(byte symbol) const
	{
		return contig_iterator(m_contig, m_iter.next());
	}

	contig_iterator prev() const
	{
		return contig_iterator(m_contig, m_iter.prev());
	}

	contig_iterator & operator++()
	{
		++m_iter;
		return *this;
	}

	contig_iterator operator++(int)
	{
		contig_iterator tmp = *this;
		++m_iter;
		return tmp;
	}

	contig_iterator & operator--()
	{
		--m_iter;
		return *this;
	}

	contig_iterator operator--( int)
	{
		contig_iterator tmp = *this;
		--m_iter;
		return tmp;
	}

	contig_iterator operator+(size_t i) const
	{
		return contig_iterator(m_contig, m_iter + i);
	}

	contig_iterator operator-(size_t i) const
	{
		return contig_iterator(m_contig, m_iter - i);
	}

	Property<T> & operator*()
	{
		return m_contig->getAnnotation(m_iter);
	}

	Property<T>* operator->()
	{
		return &(**this);
	}

	bool operator==(contig_iterator iter) const
	{
		return m_iter == iter.m_iter;
	}

	bool operator!=(contig_iterator iter) const
	{
		return !(*this == iter);
	}

	size_t index() const
	{
		return m_iter.index();
	}

private:
	contig_t*                   m_contig;
	typename contig_t::iterator m_iter;
};
