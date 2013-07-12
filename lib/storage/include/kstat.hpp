#include <vector>

template<class T, template<class T> class Property> class annotation_t;
template<class T, template<class T> class Property> class annotation_node_t;
template<class T, template<class T> class Property> class annotation_vector_t;

template<class T, template<class T> class Property>
class kstat_t {
public:
	typedef typename annotation_vector_t<T, Property>::iterator annoIter;
	
	kstat_t(std::vector<unsigned char> alphabet, int _k = 7): m_alphabet(alphabet), m_k(_k) {
		m_data = std::vector<std::vector<annoIter>>(pow(alphabet.size(),_k), std::vector<annoIter>());
		int len = m_alphabet.size();
		for (int i = 0; i<m_k; i++) {
			m_pow_cache.push_back(pow(len, i));
		}
	}

	template <class Iterator>
	bool add(Iterator begin, Iterator end, annoIter link) {
		if(begin <= end - m_k) {
			m_data[hash(begin, begin+m_k)].push_back(link);
			return true;
		}
		return false;
	}
	
	template <class Iterator>
	std::vector<annoIter>* get(Iterator begin, Iterator end) {
		if(begin <= end - m_k) {
			std::vector<annoIter>* result = &m_data[hash(begin, end)];
			// TODO: add filtering results (Roman);
			return result;
		}
		return nullptr;
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
	std::vector<std::vector<annoIter>> m_data;
	std::vector<size_t> m_pow_cache;
	int m_k;
};	
