#pragma once

#include <vector>
#include "annotation/annotation_node.hpp"

namespace igc {

template <class T, template <class> class Property, class Label = std::string, class Link = size_t>
class annotation_record {
public:
	typedef Annotation_node<T, Property, Link> annotation_node_t;
	typedef std::vector<annotation_node_t> data_t;
	typedef typename annotation_node_t::property_t property_t;

	annotation_record(Label _label) : m_label(_label) {
	}

	~annotation_record() {
	}

	void reserve(size_t _size) {
		m_data.reserve(_size);
	}

	void push_back(size_t link, property_t _prop) {
		annotation_node_t _node(link);
		*_node = _prop;
		return m_data.push_back(_node);
	}

	size_t size() {
		return m_data.size();
	}

	annotation_node_t operator[](size_t _idx) {
		return m_data[_idx];
	}

	Label operator* () {
		return m_label;
	}

private:
	Label m_label;
	data_t m_data;
};
}
