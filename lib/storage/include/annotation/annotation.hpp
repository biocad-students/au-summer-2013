#pragma once

#include <vector>
#include "annotation_record.hpp"

namespace igc {
// Class annotation:
// typename T - template parameter of stored type
// typename Propery<T> - the type used to be stored in each node
// typename Label - the type used to be stored in each record
// typename Link - TODO: add description
template <class T, template <class> class Property, class Label = std::string>
class annotation {
public:
	typedef annotation_record<T, Property, Label> annotation_record_t;
	typedef typename annotation_record_t::property_t property_t;
	typedef std::pair<size_t, size_t> offset_t;
	typedef std::vector<annotation_record_t> data_t;
	
	annotation () {
	}

	~annotation () {
	}

	void reserve(size_t _size) {
		m_data.reserve(_size);
	}

	// add node
	void insert(size_t _link, property_t _prop, size_t _idx) {
		m_data[_idx].push_back(_link, _prop);
	}

		// add node
	void insert_back(size_t _link, property_t _prop) {
		insert(_link, _prop, m_data.size() - 1);
	}

	// add new annotation
	void push_back(Label _label, size_t _size = 0) {
		annotation_record_t tmp = annotation_record_t(_label);
		if(_size) {
			tmp.reserve(_size);
		}
		m_data.push_back(tmp);
	}

	Label getRecordData(size_t _top) {
		return *m_data[_top];
	}

	property_t getNodeData(size_t _top, size_t _left) {
		return *(((m_data[_top])[_left]));
	}

	size_t size() {
		return m_data.size();
	}

	size_t record_size(size_t _idx) {
		return m_data[_idx].size();
	}

private:
	data_t m_data;
};
}
