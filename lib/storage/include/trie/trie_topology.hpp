#pragma once

#include <assert.h>
#include <vector>
#include <map>

#include "trie/trie_node.hpp"

namespace igc {

template<typename T, typename Link>
class trie_topology
{
public:
	typedef typename Node<T, Link> node_t;
	trie_topology()
		: m_counter(-1)
	{
		m_root = create_node_t(NULL, ' ', true, false);
	}

	~trie_topology()
	{
		delete m_root;
	}

	size_t AddRef() const { return ++m_counter; }
	size_t Release() const { return --m_counter; }

	class const_iterator
	{
		friend trie_topology;
	public:
		typedef typename std::vector<node_t*> dfs_t;

		const_iterator(node_t* _node_t) : m_node(_node_t), m_DFS(NULL), m_dfs_index(0) {}
		const_iterator(std::vector<node_t*>* _dfs)
			: m_node(NULL), m_DFS(_dfs), m_dfs_index(0)
		{
			if(!m_DFS->empty())
			{
				m_node = m_DFS->front();
			}
		}
		const_iterator(const const_iterator &_right) 
			: m_node(_right.m_node), m_DFS(_right.m_DFS), m_dfs_index(_right.m_dfs_index)
		{
		}

		const_iterator& operator = (const const_iterator &_right)
		{
			m_node = _right.m_node;
			m_DFS = _right.m_DFS;
			m_dfs_index = _right.m_dfs_index;
			return *this;
		}

		const node_t& operator*() const
		{
			return *m_node;
		}

		const node_t* operator->() const
		{
			return (&**this);
		}

		bool operator == (const const_iterator& _right) const
		{
			return m_node == _right.m_node;
		}

		bool operator!=(const const_iterator& _right) const
		{
			return (!(*this == _right));
		}

		const_iterator& operator++()
		{
			++m_dfs_index;
			if(m_dfs_index == m_DFS->size())
			{
				m_node = NULL;
			}
			else
			{
				m_node = (*m_DFS)[m_dfs_index];
			}
			return *this;
		}

		const_iterator operator++(int)
		{
			_My_type_iter _Tmp = *this;
			++*this;
			return _Tmp;
		}

		const_iterator& operator--()
		{
			if(m_DFS != NULL)
			{
				do
				{
					if(m_dfs_index == 0)
					{
						m_node = (*m_DFS)[0]->m_parent;
						return *this;
					}
					--m_dfs_index;
				}
				while((*m_DFS)[m_dfs_index] != m_node->m_parent);

				m_node = (*m_DFS)[m_dfs_index];
			}
			else
			{
				m_node = m_node->m_parent;
			}
			return *this;
		}

		const_iterator operator--(int)
		{
			_My_type_iter _Tmp = *this;
			--*this;
			return _Tmp;
		}

		T key() const
		{
			return m_node->m_cached_key;
		}

		Link index() const
		{
			return m_node->m_index;
		}

	private:
		node_t* m_node;
		dfs_t *m_DFS;
		size_t m_dfs_index;
	};

	class iterator : public const_iterator
	{
	public:
		iterator(node_t* _node_t) : const_iterator(_node_t) {}
		iterator(std::vector<node_t*>* _dfs) : const_iterator(_dfs) {}

		node_t& operator*() const
		{	// return designated value
			return ((node_t&)**(const_iterator *)this);
		}

		node_t* operator->() const
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

		iterator& operator--()
		{	// preincrement
			--(*(const_iterator *)this);
			return (*this);
		}

		iterator operator--(int)
		{	// postincrement
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};

	const_iterator root() const
	{
		return const_iterator(m_root);
	}

	iterator root()
	{
		return iterator(m_root);
	}

	const_iterator begin() const
	{
		if(m_DFS.empty())
		{
			make_DFS();
		}
		return const_iterator(&m_DFS);
	}

	iterator begin()
	{
		if(m_DFS.empty())
		{
			make_DFS();
		}
		return iterator(&m_DFS);
	}

	const_iterator end() const
	{
		return const_iterator((node_t*)NULL);
	}

	iterator end()
	{
		return iterator((node_t*)NULL);
	}

	const_iterator find(Link index) const
	{
		if(m_DFS.empty())
		{
			make_DFS();
		}
		for(const_iterator i=begin(); i != end(); ++i)
		{
			if(i->index == index)
			{
				return i;
			}
		}
		return end();
	}

	iterator find(Link index)
	{
		if(m_DFS.empty())
		{
			make_DFS();
		}
		if(index < m_indexed.size())
		{
			return iterator(m_indexed[index]);
		}
		return end();
	}

	iterator insert(const_iterator _where, T _key)
	{
		m_DFS.clear();
		if(_where.m_node->m_child.count(_key) == 0)
		{
			_where.m_node->m_child[_key] = create_node_t(/*&*_where*/_where.m_node, _key, false, false);
		}
		return iterator(_where.m_node->m_child[_key]);
	}

protected:
	node_t* create_node_t(node_t *_parent, T _cached_key, bool _is_root, bool _is_end)
	{
		m_indexed.push_back(new node_t(m_counter++, _parent, _cached_key, _is_root, _is_end));
		return m_indexed.back();
	}

	void make_DFS()
	{
		dfs(m_root);
	}

	void dfs(node_t *_node_t)
	{
		if(_node_t != m_root)
		{
			m_DFS.push_back(_node_t);
		}

		std::map<T, node_t*>::iterator i = _node_t->m_child.begin();
		for(; i != _node_t->m_child.end(); ++i)
		{
			dfs(i->second);
		}
	}

private:
	mutable size_t m_counter;
	node_t *m_root;
	Link m_nodeCounter;
	std::vector<node_t*> m_indexed;
	std::vector<node_t*> m_DFS;
};

}