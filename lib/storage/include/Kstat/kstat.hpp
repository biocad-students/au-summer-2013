#pragma once

#include <vector>
#include <algorithm>
#include "Annotation\annotation.hpp"

namespace IGC {

template<class Link = size_t>
class kstat {
public:
	typedef std::vector<unsigned char> alphabet_t;
	typedef std::vector<std::vector<Link>> data_t;
	typedef std::vector<Link> data_raw_t;
	typedef std::vector<size_t> cache_t;

	kstat() {
	}

	~kstat() {
	}

	kstat(alphabet_t _alphabet, int _k = 7): m_alphabet(_alphabet), m_k(_k) {
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

	template <class Iterator, class Link>
	bool add(Iterator _begin, Iterator _end, Link _link) {
		if(_begin <= _end - m_k) {
			size_t hashval = hash(_begin, _begin+m_k);
			if(hashval > 0 && hashval < m_size) {
				m_data[hashval].push_back(_link);
				return true;
			}
		}
		return false;
	}
	
	template <class Iterator>
	data_raw_t* get(Iterator begin, Iterator end) {
		data_raw_t* result = &m_data[hash(begin, end)];
		return result;
	}

	template <class Iterator>
	size_t hash(Iterator begin, Iterator end) {
		Iterator iter = begin;
		int power = 0;
		size_t hash_val = 0;
		while(iter != end) {
			alphabet_t::iterator idx = std::find(m_alphabet.begin(), m_alphabet.end(), *iter);
			if(idx == m_alphabet.end()) {
				return m_size + 1;
			}
			hash_val += m_pow_cache[power] * (idx - m_alphabet.begin());
			power++;
			++iter;
		}
		return hash_val;
	}
	
private:
	alphabet_t m_alphabet;
	data_t m_data;
	cache_t m_pow_cache;
	size_t m_size;
	int m_k;
};	
}