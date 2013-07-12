#pragma once

#include <algorithm>
#include <vector>

typedef unsigned char byte;

template<class T>
class kstatistics {
public:
	typedef T link_type;

	kstatistics(std::vector<byte> alphabet, int _k)
		: m_alphabet(alphabet), m_k(_k)
	{
		size_t power = 1;
		for (size_t i = 0; i != m_k; ++i)
		{
			m_pow_cache.push_back(power);
			power *= alphabet.size();
		}
		power *= alphabet.size();
		m_data = std::vector<std::vector<link_type>>(power, std::vector<link_type>());
	}

	template <class Iterator>
	bool add(Iterator begin, Iterator end, link_type link)
	{
		if (begin > end - m_k)
		{
			return false;
		}
		size_t hash_val = hash(begin, begin + m_k);
		if (hash_val == -1)
		{
			return false;
		}
		m_data[hash_val].push_back(link);
		return true;
	}

	template <class Iterator>
	std::vector<link_type>* get(Iterator begin, Iterator end)
	{
		if (begin > end - m_k)
		{
			return nullptr;
		}
		size_t hash_val = hash(begin, begin + m_k);
		if (hash_val == -1)
		{
			return nullptr;
		}
		std::vector<link_type>* result = &m_data[hash_val];
		return result;
	}

	template <class Iterator>
	size_t hash(Iterator begin, Iterator end)
	{
		size_t hash_val = 0;
		if (begin > end - m_k)
		{
			return -1;
		}
		Iterator iter = begin;
		size_t power = 0;
		while (iter != end) {
			hash_val += m_pow_cache[power] *
						(std::find(m_alphabet.begin(), m_alphabet.end(), (byte)*iter) - m_alphabet.begin());
			power++;
			++iter;
		}
		return hash_val;
	}

private:
	std::vector<byte> m_alphabet;
	std::vector<std::vector<link_type>> m_data;
	std::vector<size_t> m_pow_cache;
	int m_k;
};
