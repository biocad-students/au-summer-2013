#include <vector>

template <class T>
class annotation {
public:
	typedef typename std::vector<T>::iterator iterator;

	iterator begin() {
		return m_data.begin();
	}
public:
	std::vector<T> m_data;
};