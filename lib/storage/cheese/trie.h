
#include "trie_topology.h"


template<typename T, typename Key_type = char, typename Index_type = int>
class trie
{
public:
  typedef trie<T, Key_type, Index_type> _My_type;
  class const_iterator;
  class iterator;

private:
  typedef std::vector<T> _My_data_type;
  typedef trie_topology<Key_type, Index_type> _My_topology_type;
  typedef typename trie_topology<Key_type, Index_type>::iterator _My_topology_iter_type;
  typedef typename T* _Tptr;
	typedef typename const T* _Ctptr;
  typedef typename T& _Reft;
	typedef typename const T& const_reference;

public:
  class const_iterator
  {
  private:
    typedef const_iterator _My_type_iter;
    friend _My_type;
  public:
    typedef typename T value_type;
    typedef typename std::forward_iterator_tag iterator_category;
    typedef typename ptrdiff_t difference_type;

    const_iterator() {}
    const_iterator(_My_topology_iter_type &_topology_iter, const _My_data_type *_data)
      : m_topology_iter(_topology_iter), m_data(_data)
    {
    }

    Key_type key()
    {
      return m_topology_iter.key();
    }

    Index_type index()
    {
      return m_topology_iter.index();
    }

    _My_type_iter& operator = (_My_type_iter& _right)
    {
      m_topology_iter = _right.m_topology_iter;
      m_data = _right.m_data;
      return *this;
    }

    const_reference operator*() const
	  {
      return (*m_data)[m_topology_iter.index()];
		}

		_Ctptr operator->() const
		{
			return (&**this);
		}

    bool operator == (const _My_type_iter& _right) const
    {
      return m_topology_iter == _right.m_topology_iter;
    }

    bool operator != (const _My_type_iter& _right) const
    {
      return (!(*this == _right));
    }

    _My_type_iter& operator++()
    {
      ++m_topology_iter;
      return *this;
    }

    _My_type_iter operator++(int)
    {
      _My_type_iter _Tmp = *this;
      ++*this;
      return _Tmp;
    }

    _My_type_iter& operator--()
    {
      --m_topology_iter;
      return *this;
    }

    _My_type_iter operator--(int)
    {
      _My_type_iter _Tmp = *this;
      --*this;
      return _Tmp;
    }

  private:
    _My_topology_iter_type m_topology_iter;
    const _My_data_type *m_data;
  };

  class iterator : public const_iterator
  {
  public:
    typedef iterator _My_type_iter;
		typedef const_iterator _Mybase_iter;
    typedef _Tptr pointer;
		typedef _Reft reference;

  public:
    iterator() {}
    iterator(_My_topology_iter_type &_topology_iter, _My_data_type *_data)
      : const_iterator(_topology_iter, _data)
    {
    }

    reference operator*() const
		{	// return designated value
			return ((reference)**(_Mybase_iter *)this);
		}

		_Tptr operator->() const
	  {	// return pointer to class object
			return (&**this);
		}

		_My_type_iter& operator++()
		{	// preincrement
			++(*(const_iterator *)this);
			return *this;
		}

		_My_type_iter operator++(int)
		{	// postincrement
			_My_type_iter _Tmp = *this;
			++*this;
			return _Tmp;
		}

		_My_type_iter& operator--()
		{	// preincrement
			--(*(const_iterator *)this);
			return *this;
		}

		_My_type_iter operator--(int)
		{	// postincrement
			_My_type_iter _Tmp = *this;
			--*this;
			return _Tmp;
		}
  };

  trie()
  {
    m_topology_instance = new trie_topology<Key_type, Index_type>();
    m_topology = m_topology_instance;
  }

  template<typename Y>
  trie(const trie<Y, Key_type, Index_type> &_prototype)
    : m_topology(_prototype.m_topology)
  {
    m_data.resize(_prototype.size());
  }

  ~trie()
  {
    size_t counter = m_topology->Release();
    if(counter == 0 && m_topology_instance != NULL)
    {
      delete m_topology_instance;
    }
  }

  size_t size() const
  {
    return m_data.size();
  }

  size_t capacity() const
  {
    return m_data.capacity();
  }

  void reserve(size_t size)
  {
    m_data.reserve(size);
  }

  const_iterator root() const
  {
    return const_iterator(m_topology->root(), &m_data);
  }

  iterator root()
  {
    return iterator(m_topology->root(), &m_data);
  }

  const_iterator begin() const
  {
    return const_iterator(m_topology->begin(), &m_data);
  }

  iterator begin()
  {
    return iterator(m_topology->begin(), &m_data);
  }

  const_iterator end() const
  {
    return const_iterator(m_topology->end(), &m_data);
  }

  iterator end()
  {
    return iterator(m_topology->end(), &m_data);
  }

  const_iterator find(Index_type index) const
  {
    return const_iterator(m_topology->find(index), &m_data);
  }

  iterator find(Index_type index)
  {
    return iterator(m_topology->find(index), &m_data);
  }

  template<typename _Iter>
  iterator add(_Iter from, _Iter to)
  {
    iterator iter = root();
    for(; from != to; ++from)
    {
      iter = insert(iter, *from);
    }

//    return begin();
    return iter;
  }

  iterator insert(const_iterator _where, Key_type _key)
  {
    assert(m_topology != NULL);

    m_data.resize(m_data.size()+1);
    return iterator(m_topology->insert(_where.m_topology_iter, _key), &m_data);
  }


//protected:
  /*const */trie_topology<Key_type, Index_type> *m_topology;
  std::vector<T> m_data;

private:
  trie_topology<Key_type, Index_type> *m_topology_instance;
};
