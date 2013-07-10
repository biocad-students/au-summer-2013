#include <vector>
template<class T> class annotation;

template<class T>
class kstat_t {
public:
	typedef typename annotation<T>::iterator annoIter;

	kstat_t(std::vector<unsigned char> alphabet, int _k = 7): m_alphabet(alphabet), k(_k) {
		m_data = std::vector<std::vector<annoIter>>(pow(alphabet.size(),k), std::vector<annoIter>());
	}

	template <class Iterator>
	void add(Iterator begin, Iterator end, annoIter link) {
		m_data[hash(begin, end)].push_back(link);
	}
	
	template <class Iterator>
	std::vector<annoIter>* get(Iterator begin, Iterator end) {
		std::vector<annoIter>* result = &m_data[hash(begin, end)];
		// TODO: add filtering results (Roman);
		return result;
	}

	template <class Iterator>
	size_t hash(Iterator begin, Iterator end) {
		Iterator iter = begin;
		int power = 0;
		int len = m_alphabet.size();
		size_t hash_val = 0;
		while(iter != end) {
			hash_val += pow(len, power) * (std::find(m_alphabet.begin(), m_alphabet.end(), *iter) - m_alphabet.begin());
			power++;
			++iter;
		}
		return hash_val;
	}
	
private:
	std::vector<unsigned char> m_alphabet;
	std::vector<std::vector<annoIter>> m_data;
	int k;
};	