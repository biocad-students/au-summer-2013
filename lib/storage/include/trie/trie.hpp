#pragma once
#include "trie_topology.hpp"

#if 1

//// _trie_default_traits
//// alphabet_size: размер алфавита хранимых строк. Предполагается для нуклеотидов 4, для аминокислот 20.
//// key_type: тип символа в алфавите. Например, unsigned char, uint_16. По умолчанию char.
//// index_type: тип индексации вершин в боре. Например, size_t, unsigned int, uint_16.
//// get_key_mapping(): функция для быстрого определения индекса каждого из символов.
////                    Предполагается порлучение за O(1) из массива.
////                    Кстати, такой подход позволяет быть нечувствительным к регистру.
//struct _trie_default_traits
//{
//        enum { alphabet_size = 4 };
//        typedef char key_type;
//        typedef unsigned int index_type;
//        template<typename T> struct container
//        {
//                typedef std::deque<T> container_type;
//        };
//
//        inline static size_t get_key_mapping(key_type _key) { return standard_ACGT_key_mapping(_key); }
//};

template<typename T, class _Traits = _default_traits_N>
class trie
{
public:
        enum { alphabet_size = _Traits::alphabet_size };
        typedef typename _Traits::symbol_type symbol_type;
        typedef typename _Traits::link_type link_type;
        typedef typename _Traits::container<T>::container_type container_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        class const_iterator;
        class iterator;
        class permutation;

private:
        typedef trie<T, _Traits> _My_type;
        typedef trie_topology<_Traits> _My_topology_type;
        typedef typename trie_topology<_Traits>::iterator _My_topology_iter_type;
        typedef typename trie_topology<_Traits>::const_iterator _My_topology_const_iter_type;

public:
// The iterators of trie actually are wrappers of trie_topology::iterators
// Most of the operations just forward the requests to trie_topology::iterator
// The exceptions are: operator *() and operator->(),
// these operatios provide access to real data stored in the trie.
        class const_iterator
                : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
        {
        private:
                //typedef typename std::forward_iterator_tag iterator_category;
                typedef const_iterator _My_type_iter;
                typedef typename _My_type::const_pointer const_pointer;
                typedef typename _My_type::const_reference const_reference;
                friend _My_type;
        public:
                const_iterator()
                {
                }

                const_iterator(const _My_topology_const_iter_type &_topology_iter, const container_type *_data)
                        : m_topology_iter(_topology_iter), m_data(_data)
                {
                }

                symbol_type key()
                {
                        return m_topology_iter.key();
                }

                link_type index()
                {
                        return m_topology_iter.index();
                }

                link_type depth()
                {
                        return m_topology_iter.depth();
                }

                const_iterator& operator=(const const_iterator& _right)
                {
                        m_topology_iter = _right.m_topology_iter;
                        m_data = _right.m_data;
                        return *this;
                }

                const_reference operator*() const
                {
                        return (*m_data)[m_topology_iter.index()];
                }

                const_pointer operator->() const
                {
                        return (&**this);
                }

                bool operator==(const const_iterator& _right) const
                {
                        return m_topology_iter == _right.m_topology_iter;
                }

                bool operator!=(const const_iterator& _right) const
                {
                        return (!(*this == _right));
                }

                const_iterator& operator++()
                {
                        ++m_topology_iter;
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
                        --m_topology_iter;
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
                        return _My_type_iter(m_topology_iter.parent(), m_data);
                }

                bool is_leaf() const
                {
                      return m_topology_iter.is_leaf();
                }

        private:
                _My_topology_const_iter_type m_topology_iter;
// TODO: replace the type with _My_data_type&
                const container_type *m_data;
        };

        class iterator : public const_iterator
        {
        private:
                typedef iterator _My_type_iter;
        public:
                typedef typename _My_type::pointer pointer;
                typedef typename _My_type::reference reference;

                iterator()
                {
                }

                iterator(const _My_topology_const_iter_type &_topology_iter, const container_type *_data)
                        : const_iterator(_topology_iter, _data)
                {
                }

                reference operator*() const
                {       // return designated value
                        return ((reference)**(const_iterator *)this);
                }

                pointer operator->() const
                {       // return pointer to class object
                        return (&**this);
                }

                iterator& operator++()
                {       // preincrement
                        ++(*(const_iterator *)this);
                        return *this;
                }

                iterator operator++(int)
                {       // postincrement
                        _My_type_iter _Tmp = *this;
                        ++*this;
                        return _Tmp;
                }

                iterator& operator--()
                {       // preincrement
                        --(*(const_iterator *)this);
                        return *this;
                }

                iterator operator--(int)
                {       // postincrement
                        _My_type_iter _Tmp = *this;
                        --*this;
                        return _Tmp;
                }

                iterator parent() const
                {
                        return _My_type_iter(m_topology_iter.parent(), m_data);
                }
        };

        class permutation
        {
        public:
                permutation(typename _My_topology_type::permutation &_topology_perm, container_type *_data) : m_topology_permutation(&_topology_perm), m_data(_data) {}
                permutation(const permutation& _perm) : m_topology_permutation(_perm.m_topology_permutation), m_data(m_topology_permutation.m_data)
                {
                }

                const_iterator begin() const { return const_iterator(m_topology_permutation->begin(), m_data); }
                iterator begin()
                {
                        return iterator(m_topology_permutation->begin(), m_data);
                }
                const_iterator end() const { return const_iterator(m_topology_permutation->end(), m_data); }
                iterator end() { return iterator(m_topology_permutation->end(), m_data); }
        private:
                //_My_topology_type *m_topology;
                typename _My_topology_type::permutation *m_topology_permutation;
                container_type *m_data;
        };

        trie()
        {
                m_topology_instance = new _My_topology_type();
                m_topology = m_topology_instance;
                m_data.resize(1); // for root
        }

        template<typename Y>
        explicit trie(const trie<Y, _Traits> &_prototype)
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

        const permutation& DFS() const
        {
                return permutation(m_topology->DFS(), &m_data);
        }

        permutation& DFS()
        {
//                return permutation(m_topology->DFS(), &m_data);
                static permutation aaa(m_topology->DFS(), &m_data);
                aaa = permutation(m_topology->DFS(), &m_data);
                return aaa;
        }

        const_iterator find(link_type index) const
        {
                return const_iterator(m_topology->find(index), &m_data);
        }

        iterator find(link_type index)
        {
                return iterator(m_topology->find(index), &m_data);
        }

// Note, this operation returns an iterator on the LAST added item!
        template<typename _Iter>
        iterator add(_Iter from, _Iter to)
        {
                iterator iter = root();
                for(; from != to; ++from)
                {
                        iter = insert(iter, *from);
                }

                return iter;
        }

        iterator insert(iterator _where, symbol_type _key)
        {
                assert(m_topology != NULL);

                _My_topology_iter_type topology_iter(m_topology->find(_where.index()));

                topology_iter = m_topology->insert(topology_iter, _key);
                if(topology_iter.index() == m_data.size())
                {
                        m_data.resize(m_data.size() + 1);
                }
                return iterator(topology_iter, &m_data);
        }

        void swap(_My_type& _right)
        {
                assert(m_topology_instance == _right.m_topology_instance);
                m_data.swap(_right.m_data);
        }

// NOTE: These data members are public due to the bug in my compiler. Should be protected later
//protected:
// TODO: try to make trie_topology const.
        /*const*/ _My_topology_type *m_topology;
        container_type m_data;

private:
        _My_topology_type *m_topology_instance;
};


#else
// Class trie:
// typename T - the type we use as a parameter of the container, so the trie stores the elements of type T.
// typename Key_type - the type we use to branch the tree. In general we could use char, but sometimes
//	other types may be more convenient (i.e. unsigned char or int).
// typename Index_type - the type we use to access the internal representation of data
//	(the actual data is stored in vector, so we use an integer type to access the elements).
//	Could be reduced to smaller integer types, such as short int.
template<typename T, typename Key_type = char, typename Index_type = size_t>
class trie
{
public:
	class const_iterator;
	class iterator;

private:
// Sorry, here I use the code style from VS 2008 STL. Probably this would be refined later.
	typedef trie<T, Key_type, Index_type> _My_type;
	typedef std::vector<T> _My_data_type;
	typedef trie_topology<Key_type, Index_type> _My_topology_type;
	typedef typename trie_topology<Key_type, Index_type>::iterator _My_topology_iter_type;
	typedef typename trie_topology<Key_type, Index_type>::const_iterator _My_topology_const_iter_type;
	typedef T* _Tptr;
	typedef const T* _Ctptr;
	typedef T& _Reft;
	typedef const T& const_reference;

public:
// The iterators of trie actually are wrappers of trie_topology::iterators
// Most tof the operations just forward the requests to trie_topology::iterator
// The exceptions are: operator *() and operator->(),
// these operatios provide access to real data stored in the trie.
	class const_iterator
		: public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
	{
	private:
		//typedef typename std::forward_iterator_tag iterator_category;
		typedef const_iterator _My_type_iter;
		friend _My_type;
	public:

		const_iterator()
		{
		}

		const_iterator(const _My_topology_const_iter_type &_topology_iter, const _My_data_type *_data)
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

		Index_type depth()
		{
			return m_topology_iter.depth();
		}

		_My_type_iter& operator=(const _My_type_iter& _right)
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

		bool operator==(const _My_type_iter& _right) const
		{
			return m_topology_iter == _right.m_topology_iter;
		}

		bool operator!=(const _My_type_iter& _right) const
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

		_My_type_iter parent() const
		{
			return _My_type_iter(m_topology_iter.prev(), m_data);
		}

		bool is_leaf() const
		{
			return m_topology_iter.is_leaf();
		}

	private:
		_My_topology_const_iter_type m_topology_iter;
// TODO: replace the type with _My_data_type&
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
		iterator()
		{
		}

		//iterator(const _My_topology_iter_type &_topology_iter, _My_data_type *_data)
		//	: const_iterator(_topology_iter, _data)
		//{
		//}

		iterator(const _My_topology_const_iter_type &_topology_iter, const _My_data_type *_data)
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

		_My_type_iter parent() const
		{
			return _My_type_iter(m_topology_iter.parent(), m_data);
		}
	};

	trie()
	{
		m_topology_instance = new trie_topology<Key_type, Index_type>();
		m_topology = m_topology_instance;
		m_data.resize(1); // for root
	}

	template<typename Y>
	explicit trie(const trie<Y, Key_type, Index_type> &_prototype)
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

// Note, this operation returns an iterator on the LAST added item!
	template<typename _Iter>
	iterator add(_Iter from, _Iter to)
	{
		iterator iter = root();
		for(; from != to; ++from)
		{
			iter = insert(iter, *from);
		}

//		return begin();
		return iter;
	}

	iterator insert(const_iterator _where, Key_type _key)
	{
		assert(m_topology != NULL);
		_My_topology_iter_type topology_iter = m_topology->insert(_where.m_topology_iter, _key);
		if(topology_iter.index() == m_data.size())
		{
			m_data.resize(m_data.size() + 1);
		}
		return iterator(topology_iter, &m_data);
	}

	void swap(_My_type& _right)
	{
		assert(m_topology_instance == _right.m_topology_instance);
		m_data.swap(_right.m_data);
	}

// NOTE: These data members are public due to the bug in my compiler. Should be protected later
//protected:
// TODO: try to make trie_topology const.
	/*const*/ trie_topology<Key_type, Index_type> *m_topology;
	std::vector<T> m_data;

private:
	trie_topology<Key_type, Index_type> *m_topology_instance;
};
#endif
