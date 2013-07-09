#pragma once
// ContIG - Container ImmunoGlobulin

#include <map>
#include <vector>
#include "trie.hpp"

template <class T, template <class> class Property>
class contig
{
public:
	typedef std::pair<trie_node<T>*, Property<T>> table_value_t;

	contig() : m_trie(new trie_impl<T>)
	{
	}

	~contig()
	{
		delete m_trie;
	}

	std::vector<trie_node<T>*> getReversedPath(size_t handle)
	{
			if (handle >= m_annotations.size())
					throw std::out_of_range("Wrong handle!");
			trie_node<T>* last = m_annotations[handle].first;
			std::vector<trie_node<T>*> result;
			while (last != nullptr)
			{
					result.push_back(last);
					last = last->parent();
			}
			return result;
	}

	std::vector<trie_node<T>*> getPath(size_t handle)
	{
			std::vector<trie_node<T>*> result = getReversedPath(handle);
			reverse(result.begin(), result.end());
			return result;
	}	
	
	template <class Iterator>
	int pushSequence(Iterator start, Iterator end, Property<T> const & annotation)
	{
		auto push_result = m_trie->pushSequence(start, end);
		// protect from adding similar seq
		if (push_result.first == m_annotations.size() && push_result.first == push_result.second->getOut())
		{
			m_annotations.push_back(std::make_pair(push_result.second, annotation));
		}
		return push_result.first;
	}

	template <template <class> class Container>
	int pushSequence(Container<T> const & sequence, Property<T> const & annotation)
	{
		return pushSequence(sequence.begin(), sequence.end(), annotation);
	}

	table_value_t & operator[](size_t handle)
	{
		return m_annotations[handle];
	}

private:
	trie_impl<T>*				m_trie;
	std::vector<table_value_t>	m_annotations;
};
