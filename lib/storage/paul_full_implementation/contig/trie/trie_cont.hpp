#pragma once

#include <vector>
#include <map>

#include "trie_node.hpp"

template <class T>
class trie_cont
{
public:
	trie_cont() {}

	template <class S>
	trie_cont(trie_cont<S> const & cont)
	{
		m_data.reserve(cont.size());
		for (auto i = cont.m_data.begin(); i != cont.m_data.end(); ++i)
		{
			trie_node* node = i->first;
			push(node);
		}
	}

	T & operator[](size_t i)
	{
		return m_data[i].second;
	}

	trie_node* nodeOf(size_t i)
	{
		return m_data[i].first;
	}

	size_t push(trie_node* node)
	{
		m_data.push_back(std::make_pair(node, T()));
		return m_data.size() - 1;
	}

	size_t size()
	{
		return m_data.size();
	}

private:
	std::vector<std::pair<trie_node*, T>> m_data;
};
