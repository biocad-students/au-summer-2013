#pragma once

#include <string>
#include <vector>
#include <map>

#include "annotation_node.hpp"
#include "annotation_record.hpp"

template <class T, template <class> class Property, class LabelType = std::string>
class annotation
{
public:
	typedef annotation_record<T, Property>    record_type;
	typedef std::pair<LabelType, record_type> value_type;

	struct index
	{
		size_t record;
		size_t letter;

		index(size_t r = 0, size_t l = 0) : record(r), letter(l)
		{
		}
	};

	annotation(size_t length = 0)
	{
		m_data.reserve(length);
	}

	record_type & add(LabelType label, size_t length = 0)
	{
		m_data.push_back(std::make_pair(label, record_type(length)));
		return (*this)[size() - 1];
	}

	record_type & operator[](size_t i)
	{
		return m_data[i].second;
	}

	Property<T> & operator[](index i)
	{
		return m_data[i.record].second[i.letter];
	}

	LabelType labelOf(size_t i)
	{
		return m_data[i].first;
	}

	LabelType labelOf(index i)
	{
		return m_data[i.record].first;
	}

	index last()
	{
		return index(size() - 1, (*this)[size() - 1].size() - 1);
	}

	size_t size()
	{
		return m_data.size();
	}

private:
	std::vector<value_type> m_data;
};
