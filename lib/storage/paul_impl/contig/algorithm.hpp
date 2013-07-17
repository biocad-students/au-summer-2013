#pragma once

#include <vector>

#include "trie/trie.hpp"
#include "contig.hpp"


template <class Iterator, class T, template <class> class Property, class LabelType>
std::vector<typename contig<T, Property, LabelType>::index_type> contig_search(Iterator begin, Iterator end,
                                                             contig<T, Property, LabelType> const & c)
{
    typedef typename trie<bool>::iterator 						tb_iterator;
    typedef typename contig<T, Property, LabelType>::index_type index_type;

    std::vector<index_type> result;
    trie<bool>* tmp = nullptr;
    c.copyTrie(&tmp);

    size_t deep = 0;
    const std::set<index_type>* nodes = nullptr;
    for (Iterator iter = begin; iter != end; ++iter)
    {
        nodes = c.getNodes(iter, end);
        if (nodes == nullptr)
        {
            nodes = c.getNodes(iter-1, end);
            break;
        }
        for (auto niter = nodes->begin(); niter != nodes->end(); ++niter)
        {
            tb_iterator t(tmp, *niter);
            *t = true;
        }
        deep++;
    }

    if (nodes == nullptr)
    {
        return result;
    }

    for (auto niter = nodes->begin(); niter != nodes->end(); ++niter)
    {
        bool breakflag = false;
        tb_iterator t(tmp, *niter);
        for (size_t i = deep-1; i != 0; --i)
        {
            --t;
            if (t == tmp->end() || !*t)
            {
                breakflag = true;
                break;
            }
        }
        if (!breakflag)
        {
            result.push_back(t.index());
        }
    }

    delete tmp;
    return result;
}
