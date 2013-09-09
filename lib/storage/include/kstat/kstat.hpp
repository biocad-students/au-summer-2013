#pragma once

#if 1

#include <assert.h>
#include <vector>
#include <set>
#include "../traits.hpp"


template<int _k, class _Traits = _default_traits_N>
class kstat
{
public:
        enum { alphabet_size = _Traits::alphabet_size };
        typedef typename _Traits::link_type link_type;
        typedef typename std::set<link_type>::const_iterator const_iterator;
        typedef std::pair<const_iterator, const_iterator> find_result_type;

private:
        typedef kstat<_k, _Traits> _My_type;
        typedef std::set<link_type> raw_data_type;
        typedef typename _Traits::container<raw_data_type>::container_type container_type;
        typedef std::vector<size_t> cache_type;

public:
        kstat()
        {
                m_container_size = (size_t) pow(double(alphabet_size), double(_k));
                m_data = container_type(m_container_size);
                for (size_t i = 0; i < _k; ++i)
                {
                        m_pow_cache.push_back((size_t) pow(double(alphabet_size), double(i)));
                }
        }

        ~kstat()
        {
        }

        template <typename _Iter>
        bool add(_Iter _begin, _Iter _end, link_type _link)
        // Позднее все заменить на link_type
        {
                if(std::distance(_begin, _end) == _k)
                {
                        size_t hashval = hash(_begin);
                        m_data[hashval].insert(_link);
                        return true;
                }
                return false;
        }

        // Вообще говоря, второй параметр тут не нужен. Позднее можно сделать две функции: безопасную и быструю.
        template <typename _Iter>
        find_result_type find(_Iter _begin, _Iter _end)
        {
                if(std::distance(_begin, _end) == _k)
                {
                        size_t hashval = hash(_begin);
                        return std::make_pair(m_data[hashval].begin(), m_data[hashval].end());
                }
                return std::make_pair(m_data[0].end(), m_data[0].end());
        }

protected:
        template <typename _Iter>
        size_t hash(_Iter _begin)
        {
                _Iter iter = _begin;
                size_t hash_val = 0;
                while(iter != _begin + _k)
                {
                        hash_val *= alphabet_size;
                        hash_val += _Traits::get_key_mapping(*iter);
                        ++iter;
                }
                return hash_val % m_container_size;
        }

private:
        size_t m_container_size;
        container_type m_data;
        cache_type m_pow_cache;
};


#else
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include "../annotation/annotation.hpp"

namespace igc {

class Kstat {
public:
	typedef std::vector<unsigned char> alphabet_t;
	typedef std::vector<size_t> cache_t;
	typedef std::set<size_t> data_raw_t;
	typedef std::vector<data_raw_t> data_t;

	Kstat() {
	}

	~Kstat() {
	}

	Kstat(alphabet_t _alphabet, int _k = 7): m_alphabet(_alphabet), m_k(_k) {
		m_size = (size_t)pow(double(_alphabet.size()),_k);
		if(!m_size) {
			return;
		}
		m_data = data_t(m_size, data_raw_t());
		int len = m_alphabet.size();
		for (int i = 0; i<m_k; i++) {
			m_pow_cache.push_back((size_t)pow(double(len), i));
		}
	}

	template <class Iterator>
	bool add(Iterator _begin, Iterator _end, size_t _link) {
		if(_begin <= _end - m_k) {
			size_t hashval = hash(_begin, _begin+m_k);
			if(hashval < m_size) {
				m_data[hashval].insert(_link);
				return true;
			}
		}
		return false;
	}
	
	template <class Iterator>
	data_raw_t* get(Iterator _begin, Iterator _end) {
		if(_end-m_k >= _begin) {
			size_t hashval = hash(_begin, _begin+m_k);
			if(hashval < m_size) {
				return &m_data[hashval];
			}
		}
		return nullptr;
	}

	template <class Iterator>
	size_t hash(Iterator _begin, Iterator _end) {
		Iterator iter = _begin;
		size_t hash_val = 0;
		size_t PRIME_BASE = m_alphabet.size();
		alphabet_t::iterator idx;
		alphabet_t::iterator begin = m_alphabet.begin();
		alphabet_t::iterator end = m_alphabet.end();
		while(iter != _end) {
			idx = std::find(begin, end, (*iter));
    		hash_val *= PRIME_BASE; //shift over by one
    		hash_val += idx - begin; //add the current char
			++iter;
		}
		return hash_val;
	}
	
	size_t getK(void) {
		return m_k;
	}

private:
	alphabet_t m_alphabet;
	data_t m_data;
	cache_t m_pow_cache;
	size_t m_size;
	int m_k;
};	
}
#endif