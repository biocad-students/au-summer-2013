#pragma once

#include <vector>
#include <stack>

#include "alicont_line.hpp"


template <class T>
struct matrix2d
{
    typedef std::vector<std::vector<T>> type;
};

// For T - alphabet != returns 0             : for a == b
//                             mismatch(a,b) : for a != b
template <class T>
class alicont
{
public:
    typedef typename std::pair<matrix2d<int>::type*, path_value> trace_value;


    template <class Iterator>
    alicont(Iterator begin, Iterator end, int gap_open, int gap_ext)
        : m_data(), m_gap_open(gap_open), m_gap_ext(gap_ext)
    {
        while(begin != end)
        {
            m_line.push_back(*begin++);
        }
    }

    template <class Iterator>
    alicont_line* add(Iterator begin, Iterator end)
    {
        return alignment(begin, end);
    }

    void pop()
    {
        m_data.pop();
    }

private:
    template <class Iterator>
    alicont_line* alignment(Iterator begin, Iterator end)
    {
        size_t m = m_line.size();
        size_t n = std::distance(begin, end);

        matrix2d<int>::type e(n + 1, std::vector<int>(m + 1));
        matrix2d<int>::type f(n + 1, std::vector<int>(m + 1));
        matrix2d<int>::type v(n + 1, std::vector<int>(m + 1));

        matrix2d<trace_value>::type eb(n + 1, std::vector<trace_value>(m + 1));
        matrix2d<trace_value>::type fb(n + 1, std::vector<trace_value>(m + 1));
        matrix2d<trace_value>::type vb(n + 1, std::vector<trace_value>(m + 1));

        return nullptr;
    }

private:
    std::stack<alicont_line> m_data;
    std::vector<T>           m_line;
    int                      m_gap_open;
    int                      m_gap_ext;
};
