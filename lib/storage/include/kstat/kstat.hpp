#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include "annotation/annotation.hpp"

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
