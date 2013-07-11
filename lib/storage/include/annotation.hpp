#include <vector>

// annotationIterator lastAnno = m_annotation.begin();
// lastAnno = m_annotation.insert(lastAnno, iter->first);
// lastAnno->addLink(lastTrie);
// lastAnno->setAnno(iter->second);

template <class T, template <class> class Property>
class annotation_node_t {
public:
	typedef void trie_node;
	annotation_node_t () {
	}

	~annotation_node_t () {
	}
private:
	Property<T> m_internal;
	trie_node* m_trie_node;
};

template <class T, template <class> class Property>
class annotation_t {
public:
	typedef typename std::vector<annotation_node_t<T, Property>>::iterator iterator;

	annotation_t() {
	}

	~annotation_t() {
	}

	iterator push_back(annotation_node_t<T, Property> _annotation_node) {
		return m_data.push_back(_annotation_node);
	}

	iterator begin() {
		return m_data.begin();
	}

	iterator end() {
		return m_data.end();
	}

private:
	std::vector<annotation_node_t<T, Property>> m_data;
};

template <class T, template <class> class Property>
class annotation_vector_t {
public:
	typedef typename std::vector<annotation_t<T, Property>>::iterator iterator;

	annotation_vector_t () {
	}

	~annotation_vector_t () {
	}

	iterator begin() {
		return m_data.begin();
	}

	iterator push_back(annotation_t<T, Property> _annotation) {
		return m_data.push_back(_annotation);
	}

	iterator getAnnotationByOffset(int top) {
		return m_data[top].begin();
	}

	iterator getNodeByOffset(int top, int right) {
		return m_data[top][right].begin();
	}

	iterator end() {
		return m_data.end();
	}

private:
	std::vector<annotation_t<T, Property>> m_data;
};