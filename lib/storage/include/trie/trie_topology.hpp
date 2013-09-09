#pragma once

#if 1

#include <assert.h>
#include <deque>
#include "../traits.hpp"

template<class _Traits = _default_traits_N>
class trie_topology
{
public:
        enum { alphabet_size = _Traits::alphabet_size };
        typedef typename _Traits::symbol_type symbol_type;
        typedef typename _Traits::link_type link_type;
        class const_iterator;
        class iterator;
        class permutation;

private:
        typedef trie_topology<_Traits> _My_type;
        struct _Node;
        typedef typename _Traits::container<_Node*>::container_type container_type;

protected:
        struct _Node
        {
                _Node(link_type _index, link_type _depth, _Node *_parent, symbol_type _cached_symbol, bool _is_root, bool _is_end)
                        : index(_index), depth(_depth), parent(_parent), cached_symbol(_cached_symbol)/*, is_root(_is_root), is_end(_is_end)*/
                {
                        for(_Node** p = children; p < children + alphabet_size; ++p)
                        {
                                *p = NULL;
                        }
                }

                _Node* get_child_record(symbol_type _key) const
                {
                        assert(_Traits::get_key_mapping(_key) != -1 && (_Traits::get_key_mapping(_key) < sizeof(children)/sizeof(children[0])));
                        return children[_Traits::get_key_mapping(_key)];
                }

                void set_child_record(symbol_type _key, _Node* _node)
                {
                        assert(_Traits::get_key_mapping(_key) != -1 && (_Traits::get_key_mapping(_key) < sizeof(children)/sizeof(children[0])));
                        children[_Traits::get_key_mapping(_key)] = _node;
                }

                // Можно это закешировать
                bool is_leaf() const
                {
                        for(size_t i = 0; i < alphabet_size; ++i)
                        {
                                if(children[i] != NULL)
                                {
                                        return false;
                                }
                        }
                        return true;
                }

                link_type index;
                link_type depth;
                _Node* children[alphabet_size];
                _Node *parent;
                symbol_type cached_symbol;
        };

public:
        class const_iterator
        {
        private:
                typedef const_iterator _My_type_iter;
                //friend _My_type;
                //friend trie_topology;
        public:
                const_iterator(container_type* _roadmap, link_type _roadmap_index = 0)
                        : m_roadmap(_roadmap), m_roadmap_index(_roadmap_index)
                {
                        if(m_roadmap_index < (*m_roadmap).size())
                                m_node = (*m_roadmap)[m_roadmap_index];
                        else
                                m_node = NULL;
                }

                const_iterator(const _Node* _node)
                        : m_roadmap(NULL), m_roadmap_index(0)
                {
                        m_node = const_cast<_Node*>(_node);
                }


                const_iterator(const const_iterator &_right) 
                        : m_roadmap(_right.m_roadmap), m_roadmap_index(_right.m_roadmap_index), m_node(_right.m_node)
                {
                }

                const_iterator& operator = (const const_iterator &_right)
                {
                        m_roadmap = _right.m_roadmap;
                        m_roadmap_index = _right.m_roadmap_index;
                        m_node = _right.m_node;
                        return *this;
                }

                const _Node& operator*() const
                {
                        //return *(*m_roadmap)[m_roadmap_index];
                        return *m_node;
                }

                const _Node* operator->() const
                {
                        return (&**this);
                }

                bool operator == (const const_iterator& _right) const
                {
                        return m_node == _right.m_node;
                }

                bool operator != (const const_iterator& _right) const
                {
                        return (!(*this == _right));
                }

                const_iterator& operator++()
                {
                        ++m_roadmap_index;
                        m_node = m_roadmap_index < (*m_roadmap).size()? (*m_roadmap)[m_roadmap_index]: NULL;
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
                        --m_roadmap_index;
                        m_node = (*m_roadmap)[m_roadmap_index];
                        return *this;
                }

                const_iterator operator--(int)
                {
                        _My_type_iter _Tmp = *this;
                        --*this;
                        return _Tmp;
                }

                const_iterator parent() const
                {
                        /*link_type parent_ind = (**this).parent->index;
                        size_t ri = m_roadmap_index;
                        while((*m_roadmap)[--ri]->index != parent_ind )
                        {
                        }
                        return const_iterator(m_roadmap, ri);*/
                        return const_iterator((**this).parent);
                }

                symbol_type key() const
                {
                        return (**this).cached_symbol;
                }

                link_type index() const
                {
                        return (**this).index;
                }

                link_type depth() const
                {
                        return (**this).depth;
                }

                // Позднее стоило бы убрать этот метод. Хотя...
                bool is_leaf() const
                {
                        return (**this).is_leaf();
                }

        private:
                container_type *m_roadmap;
                link_type m_roadmap_index;
                _Node* m_node;
        };

        class iterator : public const_iterator
        {
        private:
                typedef iterator _My_type_iter;
                typedef const_iterator _Mybase_iter;

        public:
                iterator(container_type* _roadmap, link_type _roadmap_index = 0)
                        : const_iterator(_roadmap, _roadmap_index)
                {
                }

                iterator(const _Node* _node)
                        : const_iterator(_node)
                {
                }

                _Node& operator*() const
                {       // return designated value
                        return ((_Node&)**(const_iterator *)this);
                }

                _Node* operator->() const
                {       // return pointer to class object
                        return (&**this);
                }

                iterator& operator++()
                {       // preincrement
                        ++(*(const_iterator *)this);
                        return (*this);
                }

                iterator operator++(int)
                {       // postincrement
                        iterator _Tmp = *this;
                        ++*this;
                        return (_Tmp);
                }

                iterator& operator--()
                {       // preincrement
                        --(*(const_iterator *)this);
                        return (*this);
                }

                iterator operator--(int)
                {       // postincrement
                        iterator _Tmp = *this;
                        --*this;
                        return (_Tmp);
                }

                iterator parent() const
                {
                        return iterator(m_node->parent);
                }

        };

        class permutation
        {
        public:
                permutation(size_t _size) : m_order(_size) {}
//позднее убрать:
permutation(container_type _order) : m_order(_order) {}

                size_t size() const { return m_order.size(); }
                const_iterator begin() const { return const_iterator(&m_order, 1); }
                iterator begin() { return iterator(&m_order, 1); }
                const_iterator end() const { return const_iterator(&m_order, m_order.size()); }
                iterator end() { return iterator(&m_order, m_order.size()); }

                void swap(permutation& _rhv) { m_order.swap(_rhv.m_order); }
        //private:
        public:
                container_type m_order;
        };

protected:
        class permutation_DFS : public permutation
        {
        public:
                // Позднее убрать:
                permutation_DFS()
                        : permutation(0)
                {
                }

                permutation_DFS(const permutation& _perm)
                        : permutation(_perm.size())
                {
                        m_temp = &_perm.m_order;
                        m_temp_ind = 0;
                        DFS_go((*m_temp)[0]);
                }
        private:
                void DFS_go(_Node* _node)
                {
                        if(_node == NULL)
                        {
                                return;
                        }

                        //if(_node != m_root)
                        m_order[m_temp_ind++] = _node;

                        for(size_t i = 0; i != alphabet_size; ++i)
                        {
                                DFS_go(_node->children[i]);
                        }
                }
                const container_type *m_temp;
                size_t m_temp_ind;
        };


public:
        trie_topology()
                : m_counter(0)
        {
                m_root = create_node(NULL, ' ', true, false);
        }

        ~trie_topology()
        {
                for(container_type::iterator iter = m_indexed.begin(); iter != m_indexed.end(); ++iter)
                {
                        delete *iter;
                }
        }

        size_t AddRef() const { return ++m_counter; }
        size_t Release() const { return --m_counter; }

        const_iterator root() const
        {
                return const_iterator(&m_indexed, 0);
        }

        iterator root()
        {
                return iterator(&m_indexed, 0);
        }

        const_iterator begin() const
        {
                return const_iterator(&m_indexed, 1);
        }

        iterator begin()
        {
                return iterator(&m_indexed, 1);
        }

        const_iterator end() const
        {
                return const_iterator(&m_indexed, m_indexed.size());
        }

        iterator end()
        {
                return iterator(&m_indexed, m_indexed.size());
        }

        permutation& DFS()
        {
                if(m_DFS.size() != m_indexed.size())
                {
                        m_DFS.swap(permutation_DFS(permutation(m_indexed)));
                }

                return m_DFS;
        }

        const_iterator find(link_type index) const
        {
                if(index < m_indexed.size())
                {
                        return iterator(&m_indexed, m_indexed[index]);
                }
                return end();
        }

        iterator find(link_type index)
        {
                if(index < m_indexed.size())
                {
                        return iterator(&m_indexed, index);
                }
                return end();
        }

        iterator insert(iterator _where, symbol_type _key)
        {
                if(_where->get_child_record(_key) == NULL)
                {
                        _where->set_child_record(_key, create_node(&(*_where), _key, false, false));
                }
                return iterator(_where->get_child_record(_key));
        }

protected:
        _Node* create_node(_Node *_parent, symbol_type _cached_key, bool _is_root, bool _is_end)
        {
                if(_is_root)
                        m_indexed.push_back(new _Node(m_counter++, 0, _parent, _cached_key, _is_root, _is_end));
                        //return new Node(m_counter++, 0, _parent, _cached_key, _is_root, _is_end);
                else
                        m_indexed.push_back(new _Node(m_counter++, _parent->depth + 1, _parent, _cached_key, _is_root, _is_end));
                return m_indexed.back();
        }

private:
        mutable size_t m_counter;
        _Node *m_root;
        container_type m_indexed;
        permutation_DFS m_DFS;
};

#else
#include <assert.h>
#include <vector>
#include <map>

template<typename Key_type, typename Index_type>
class trie_topology
{
private:
	typedef trie_topology<Key_type, Index_type> _My_type;

protected:
  struct Node
  {
    Node(Index_type _index, Index_type _depth, Node *_parent, Key_type _cached_key, bool _is_root, bool _is_end)
      : index(_index), depth(_depth), parent(_parent), cached_key(_cached_key), is_root(_is_root), is_end(_is_end)
    {
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
    Index_type depth;
    std::map<Key_type, Node*> child;
    Node *parent;
    bool is_root;
    bool is_end;
    Key_type cached_key;
  };

public:
  trie_topology()
    : m_counter(0)
  {
    m_root = create_node(NULL, ' ', true, false);
  }

  ~trie_topology()
  {
    for(std::vector<Node*>::iterator iter = m_indexed.begin(); iter != m_indexed.end(); ++iter)
    {
      delete *iter;
    }
  }

  size_t AddRef() const { return ++m_counter; }
  size_t Release() const { return --m_counter; }



  class const_iterator
  {
	private:
		typedef const_iterator _My_type_iter;
		friend _My_type;
    friend trie_topology;
  public:
    const_iterator(Node* _node) : m_node(_node), m_DFS(NULL), m_dfs_index(0) {}
    const_iterator(std::vector<Node*>* _dfs, size_t _dfs_index = 0)
      : m_node(NULL), m_DFS(_dfs), m_dfs_index(_dfs_index)
    {
      if(!m_DFS->empty())
          m_node = (*m_DFS)[m_dfs_index];
    }
    //const_iterator(std::vector<Node*>* _dfs, size_t _dfs_index)
    //  : m_node(NULL), m_DFS(_dfs), m_dfs_index(_dfs_index)
    //{
    //  m_node = (*m_DFS)[_dfs_index];
    //}
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
/*        do
        {
          if(m_dfs_index == 0)
          {
            m_node = NULL;
            return *this;
          }
          --m_dfs_index;
        }
        while((*m_DFS)[m_dfs_index] != m_node->parent);*/
        --m_dfs_index;

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

    const_iterator parent() const
    {
      return const_iterator(m_node->parent);
    }

    Key_type key() const
    {
      return m_node->cached_key;
    }

    Index_type index() const
    {
      return m_node->index;
    }

    Index_type depth() const
    {
      return m_node->depth;
    }

    bool is_leaf() const
    {
      return m_node->child.empty();
    }

  private:
    Node* m_node;
    std::vector<Node*> *m_DFS;
    size_t m_dfs_index;
  };

  class iterator : public const_iterator
  {
	public:
		typedef iterator _My_type_iter;
		typedef const_iterator _Mybase_iter;
  public:
    iterator(Node* _node) : const_iterator(_node) {}
    iterator(std::vector<Node*>* _dfs, size_t _dfs_index = 0) : const_iterator(_dfs, _dfs_index) {}

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

		const_iterator parent() const
		{
			return const_iterator(m_node->parent);
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
        make_DFS();
    return const_iterator(m_DFS, 1);
  }

  iterator begin()
  {
    if(m_DFS.empty())
        make_DFS();
    return iterator(&m_DFS, 1);
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
    if(index < m_indexed.size())
    {
      return iterator(m_indexed[index]);
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
    if(_is_root)
        m_indexed.push_back(new Node(m_counter++, 0, _parent, _cached_key, _is_root, _is_end));
    //    return new Node(m_counter++, 0, _parent, _cached_key, _is_root, _is_end);
    else
        m_indexed.push_back(new Node(m_counter++, _parent->depth + 1, _parent, _cached_key, _is_root, _is_end));
    return m_indexed.back();
  }

  void make_DFS()
  {
    dfs(m_root);
  }

	void dfs(Node *_node)
	{
    //if(_node != m_root)
		    m_DFS.push_back(_node);

    typename std::map<Key_type, Node*>::iterator i = _node->child.begin();
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
#endif
