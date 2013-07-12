#pragma once

#include <vector>
#include <map>

#include "annotation_node.hpp"

template <class T, template <class> class Property>
class annotation_record
{
public:
	typedef annotation_node<T, Property> value_type;

	annotation_record(size_t length = 0)
	{
		m_data.reserve(length);
	}

	Property<T> & push(size_t index, byte symbol)
	{
		m_data.push_back(value_type(index, symbol));
		return *m_data[size() - 1];
	}

	Property<T> & operator[](size_t i)
	{
		return *m_data[i];
	}

	byte symbolOf(size_t i)
	{
		return m_data[i].symbol();
	}

	size_t indexOf(size_t i)
	{
		return m_data[i].index();
	}

	size_t size()
	{
		return m_data.size();
	}

private:
	std::vector<value_type> m_data;
};
