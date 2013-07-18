#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include "Annotation/annotation.hpp"

namespace igc {

template<class Link = size_t>
class Kstat {
public:
	typedef std::map<unsigned char, size_t> alphabet_t;
	typedef std::vector<size_t> cache_t;
	typedef std::set<Link> data_raw_t;
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
	bool add(Iterator _begin, Iterator _end, Link _link) {
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
	size_t hash(Iterator begin, Iterator end) {
		Iterator iter = begin;
		size_t hash_val = 0;
		size_t PRIME_BASE = m_alphabet.size();
		size_t idx;
		while(iter != end) {
			try {
				idx = m_alphabet.at(*iter);
			}
			catch ( std::out_of_range ) {
				return -1;
			}

    		hash_val *= PRIME_BASE; //shift over by one
    		hash_val += idx; //add the current char
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
