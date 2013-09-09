#pragma once
#if 1

#include <assert.h>
#include <deque>
#include <vector>
#include <string>
#include <map>
#include "../traits.hpp"

template <typename _Property, typename _Label = std::string, class _Traits = _default_traits_N>
class annotation_sequence_registry
{
private:

public:
        typedef _Property property_type;
        typedef _Label label_type;
        typedef size_t index_type;
        //typedef typename _Traits::link_type first_index_type;
        //typedef typename _Traits::link_type second_index_type;
        //typedef std::pair<first_index_type, second_index_type> link_type;
        class annotation;
        //typedef std::vector<annotation> annotation_sequence;
        class annotation_sequence;
        typedef std::vector<annotation_sequence> container_type;

public:
        struct annotation
        {
                annotation(const property_type &_prop/*, const link_type &_link*/) : prop(_prop)/*, link(_link)*/ {}
                property_type prop;
                //link_type link;
        };

        class annotation_sequence
        {
        public:
                annotation_sequence(size_t id) : m_id(id) {}
                size_t id() {return m_id;}
                //annotation& operator [] (size_t index) { return m_data[index]; }
                //std::vector<annotation>& operator * () { return m_data; }
                std::vector<annotation>* operator -> () { return &m_data; }
        private:
                size_t m_id;
                std::vector<annotation> m_data;
        };

        annotation_sequence_registry()
        {
        }

        ~annotation_sequence_registry()
        {
        }

        size_t sequence_count()
        {
                return m_data.size();
        }

        void reserve(size_t _size)
        {
                m_data.reserve(_size);
        }

        //void insert(size_t _link, property_t _prop, size_t _idx)
        //{
        //        m_data[_idx].push_back(_link, _prop);
        //}

	//	// add node
	//void insert_back(size_t _link, property_t _prop) {
	//	insert(_link, _prop, m_data.size() - 1);
	//}

        // add new annotation
        void push_back(label_type _label)
        {
                assert(m_indexer.find(_label) == m_indexer.end());
                m_data.push_back(annotation_sequence(m_indexer.size()));
                m_indexer[_label] = m_data.size() - 1;
        }

        // Пока заменю оператор на метод
        annotation_sequence& operator [] (label_type _label)
        {
                std::map<label_type, index_type>::const_iterator indexer_iter = m_indexer.find(_label);
                if(indexer_iter != m_indexer.end())
                {
                        return get_by_index(indexer_iter->second);
                }
                else
                {
                        push_back(_label);
                        return get_by_index(m_data.size() - 1);
                }
        }

        annotation_sequence& add(label_type _label)
        {
            push_back(_label);
            return get_by_index(m_data.size() - 1);
        }

        annotation_sequence& get_by_index(index_type _index)
        {
                return m_data[_index];
        }

	//property_t getNodeData(size_t _top, size_t _left) {
	//	return *(((m_data[_top])[_left]));
	//}


	//size_t record_size(size_t _idx) {
	//	return m_data[_idx].size();
	//}

private:
        container_type m_data;
        std::map<label_type, index_type> m_indexer;
};








#else
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
#endif
