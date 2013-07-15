#pragma once

#include <vector>
#include "annotation_record.hpp"

template <class T, template <class> class Property, class Label>
class annotation_t {
public:
	typedef annotation_record_t<T, Property, Label> annotation_record;

	annotation_t () {
	}

	~annotation_t () {
	}

	void reserve(size_t _size) {
		m_data.reserve(_size);
	}

	// add node
	void insert(size_t _idx, size_t _link, Property<T> _prop) {
		m_data[_idx].push_back(_link, _prop);
	}

	// add new annotation
	void push_back(size_t _size, Label _label) {
		annotation_record tmp = annotation_record(_label);
		tmp.reserve(_size);
		m_data.push_back(tmp);
	}

	Label getRecordData(size_t top) {
		return *m_data[top];
	}

	Property<T> getNodeData(size_t _top, size_t _left) {
		return *(((m_data[_top])[_left]));
	}

	size_t size() {
		return m_data.size();
	}

private:
	std::vector<annotation_record> m_data;
};