#pragma once

#include <assert.h>
#include <vector>
#include <map>

namespace igc {

template<typename Key_type, typename Index_type>
class trie_topology
{
protected:
	struct Node
	{
		Node(Index_type _index, Node *_parent, Key_type _cached_key, bool _is_root, bool _is_end)
			: index(_index), parent(_parent), cached_key(_cached_key), is_root(_is_root), is_end(_is_end)
		{
		}

		~Node()
		{
			for (auto i = child.begin(); i != child.end(); ++i)
			{
				delete i->second;
			}
		}

		Node* get_child_for(Key_type _key)
		{
			if(child.count(_key) == 1)
			{
				return child[_key];
			}
			return NULL;
		}

		Index_type index;
		std::map<Key_type, Node*> child;
		Node *parent;
		bool is_root;
		bool is_end;
		Key_type cached_key;
	};

public:
	trie_topology()
		: m_counter(-1)
	{
		m_root = create_node(NULL, ' ', true, false);
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
		const_iterator(Node* _node) : m_node(_node), m_DFS(NULL), m_dfs_index(0) {}
		const_iterator(std::vector<Node*>* _dfs)
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

		const Node& operator*() const
		{
			return *m_node;
		}

		const Node* operator->() const
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
						m_node = (*m_DFS)[0]->parent;
						return *this;
					}
					--m_dfs_index;
				}
				while((*m_DFS)[m_dfs_index] != m_node->parent);

				m_node = (*m_DFS)[m_dfs_index];
			}
			else
			{
				m_node = m_node->parent;
			}
			return *this;
		}

		const_iterator operator--(int)
		{
			_My_type_iter _Tmp = *this;
			--*this;
			return _Tmp;
		}

		Key_type key() const
		{
			return m_node->cached_key;
		}

		Index_type index() const
		{
			return m_node->index;
		}

	private:
		Node* m_node;
		std::vector<Node*> *m_DFS;
		size_t m_dfs_index;
	};

	class iterator : public const_iterator
	{
	public:
		iterator(Node* _node) : const_iterator(_node) {}
		iterator(std::vector<Node*>* _dfs) : const_iterator(_dfs) {}

		Node& operator*() const
		{	// return designated value
			return ((Node&)**(const_iterator *)this);
		}

		Node* operator->() const
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
		return const_iterator((Node*)NULL);
	}

	iterator end()
	{
		return iterator((Node*)NULL);
	}

	const_iterator find(Index_type index) const
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

	iterator find(Index_type index)
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

	iterator insert(const_iterator _where, Key_type _key)
	{
		m_DFS.clear();
		if(_where.m_node->child.count(_key) == 0)
		{
			_where.m_node->child[_key] = create_node(/*&*_where*/_where.m_node, _key, false, false);
		}
		return iterator(_where.m_node->child[_key]);
	}

protected:
	Node* create_node(Node *_parent, Key_type _cached_key, bool _is_root, bool _is_end)
	{
		m_indexed.push_back(new Node(m_counter++, _parent, _cached_key, _is_root, _is_end));
		return m_indexed.back();
	}

	void make_DFS()
	{
		dfs(m_root);
	}

	void dfs(Node *_node)
	{
		if(_node != m_root)
		{
			m_DFS.push_back(_node);
		}

		std::map<Key_type, Node*>::iterator i = _node->child.begin();
		for(; i != _node->child.end(); ++i)
		{
			dfs(i->second);
		}
	}

private:
	mutable size_t m_counter;
	Node *m_root;
	Index_type m_nodeCounter;
	std::vector<Node*> m_indexed;
	std::vector<Node*> m_DFS;
};

}