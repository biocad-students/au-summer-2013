#include <vector>
#include "annotation.hpp"

template<class T, template<class T> class Property, class Link>
class kstat_t {
public:
	typedef Link link_type;

	kstat_t(std::vector<unsigned char> alphabet, int _k = 7): m_alphabet(alphabet), m_k(_k) {
		m_data = std::vector<std::vector<Link>>(pow(alphabet.size(),_k), std::vector<Link>());
		int len = m_alphabet.size();
		for (int i = 0; i<m_k; i++) {
			m_pow_cache.push_back(pow(len, i));
		}
	}

	template <class Iterator, class Link>
	bool add(Iterator begin, Iterator end, Link link) {
		if(begin <= end - m_k) {
			m_data[hash(begin, begin+m_k)].push_back(link);
			return true;
		}
		return false;
	}
	
	template <class Iterator>

	std::vector<link_type>* get(Iterator begin, Iterator end) {
		std::vector<link_type>* result = &m_data[hash(begin, end)];
		// TODO: add filtering results (Roman);
		return result;
	}

	template <class Iterator>
	size_t hash(Iterator begin, Iterator end) {
		Iterator iter = begin;
		int power = 0;
		size_t hash_val = 0;
		while(iter != end) {
			hash_val += m_pow_cache[power] * (std::find(m_alphabet.begin(), m_alphabet.end(), *iter) - m_alphabet.begin());
			power++;
			++iter;
		}
		return hash_val;
	}
	
private:
	std::vector<unsigned char> m_alphabet;
	std::vector<std::vector<Link>> m_data;
	std::vector<size_t> m_pow_cache;
	int m_k;
};	
