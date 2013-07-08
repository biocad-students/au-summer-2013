#pragma once

#include <vector>
#include <map>

template<typename T, typename ID_TYPE> class trie
{
public:
  typedef trie<T, ID_TYPE> _Myt;
  class const_iterator;
  class iterator;
  //friend class const_iterator;
protected:
  template<int> struct _Node;
  //friend struct _Node;
  typedef typename T value_type;
  typedef typename T* _Tptr;
	typedef typename const T* _Ctptr;
  typedef typename T& _Reft;
	typedef typename const T& const_reference;

  template<int n> struct _Node
  {
    _Node<n>(_Node<n> *_parent,
      char _key,
      const value_type& _Val)
      : _Parent(_parent),
        _Key(_key),
        _value(_Val)
      { // construct a node with value
      }

    char _Key;
    _Node<n> *_Parent;
    _Node<n> *_Children[n];
    value_type _value;	// the stored value
  };


  class const_iterator
  {
  private:
    typedef const_iterator _Myt_iter;
    friend _Myt;
  public:
//    const_iterator() : _Ptr(NULL) {}
//    const_iterator(_Node<256> *_pNode, const _Myt *_pTrie) : _Ptr(_pNode) {}
    const_iterator() {}
    const_iterator(_Node<256> *_pNode, const _Myt *_pTrie) : _Ptr(_pNode) {}

    char key()
    {
      if(_Queue.size() == 0)
          return ' ';
      return _Queue.back()->_Key;
    }

		const_reference operator*() const
	  {
			return _Queue.back()->_value;
		}

		_Ctptr operator->() const
		{
			return (&**this);
		}

    bool operator == (const _Myt_iter& _Right) const
    {
      if(_Queue.size() == 0 && _Queue.size() == _Right._Queue.size())
          return true;
      if(_Queue.size() == 0 || _Right._Queue.size() == 0)
          return false;
      return (_Queue.back() == _Right._Queue.back());
    }

    bool operator!=(const _Myt_iter& _Right) const
    {
      return (!(*this == _Right));
    }

    _Myt_iter& operator++()
    {
      _Queue.pop_back();
      return (*this);
    }

    _Myt_iter operator++(int)
    {
      _Myt_iter _Tmp = *this;
      ++*this;
      return (_Tmp);
    }


  private:
    //shared_ptr<vector<direction> >
    std::vector<_Node<256>*> _Queue;
  };

  class iterator : public const_iterator
  {
  public:
		typedef const_iterator _Mybase_iter;
    typedef _Tptr pointer;
		typedef _Reft reference;

    iterator() {}
    iterator(_Node<256> *_pNode, const _Myt *_pTrie) : _Mybase_iter(_pNode, _pTrie) {}

    reference operator*() const
			{	// return designated value
			return ((reference)**(_Mybase_iter *)this);
			}

		_Tptr operator->() const
			{	// return pointer to class object
			return (&**this);
			}

		_Myt_iter& operator++()
			{	// preincrement
			++(*(_Mybase_iter *)this);
			return (*this);
			}

		_Myt_iter operator++(int)
			{	// postincrement
			_Myt_iter _Tmp = *this;
			++*this;
			return (_Tmp);
			}
  };

public:
  trie() : _Root(new _Node<256>(NULL, ' ', T())) {}

  template<class _Iter>
    iterator add(_Iter from, _Iter to, ID_TYPE id)
    {
      iterator iter;
      _Node<256> *node = _Root;
      for(; from != to; ++from)
      {
        node = _Insert(node, *from);
      }
      _Id_Map[id] = node;

      for(; node != _Root; node = node->_Parent)
      {
        iter._Queue.push_back(node);
      }
      return iter;
    }

  iterator end()
  {
    return iterator();
  }

  const_iterator end() const
  {
    return const_iterator();
  }

  iterator operator[](ID_TYPE id) const
  {
    iterator iter;
    std::map<ID_TYPE, _Node<256>*>::const_iterator i = _Id_Map.find(id);
    if(i == _Id_Map.end())
        return iter;
    _Node<256> *node = i->second;
    for(; node != _Root; node = node->_Parent)
    {
      iter._Queue.push_back(node);
    }
    return iter;
  }
protected:
  _Node<256> *_Insert(_Node<256>* node, char direction)
  {
    if(node->_Children[direction] != NULL)
    {
      node->_Children[direction] = new _Node<256>(node, direction, T());
    }
    return node->_Children[direction];
  }

protected:
  _Node<256> *_Root;
  std::map<ID_TYPE, _Node<256>*> _Id_Map;
};
