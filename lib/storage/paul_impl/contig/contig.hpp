#pragma once

#include <deque>
#include <vector>
#include <string>

#include "trie/trie.hpp"
#include "annotation/annotation.hpp"
#include "kstat/kstat.hpp"


template <class T, template <class> class Property, class LabelType=std::string>
class contig
{
public:
    typedef Property<T>                        			  data_type;
    typedef std::vector<byte>				   			  alphabet_type;
    typedef annotation<T, Property, LabelType> 			  anno_type;
    typedef typename anno_type::index          			  link_type;
    typedef typename anno_type::record_type    			  record_type;
    typedef trie<std::vector<link_type>>				  trie_type;
    typedef typename trie_type::index_type				  index_type;
    typedef kstatistics<index_type>		      			  kstat_type;

    typedef typename trie_type::const_iterator      	  const_iterator;
    typedef typename trie_type::iterator       			  iterator;

    contig(std::string const & name, alphabet_type const & alphabet, size_t k = 7)
        : m_name(name), m_stat(alphabet, k)
    {
    }

    template <class Iterator1, class Iterator2>
    iterator push(Iterator1 begin, Iterator1 end, Iterator2 anno_begin,
                  LabelType label)
    {
        size_t record_length = std::distance(begin, end);

        record_type & new_record = m_anno.add(label, record_length);
        iterator trie_iter = m_trie.begin();

        link_type link;

        for (Iterator1 iter = begin; iter != end; ++iter, ++anno_begin)
        {
            byte symbol = *iter;
            data_type new_data = *anno_begin;

            // Add trie node
            trie_iter = m_trie.insert(trie_iter, symbol);

            // Add annotation node and link annotation to trie node
            data_type & data = new_record.push(trie_iter.index(), symbol);
            // Add annotation data
            data = new_data;

            // Link trie node and annotation data
            trie_iter->push_back(m_anno.last());

            // Add statistics data and link it with the node
            m_stat.add(iter, end, trie_iter.index());
        }

        return trie_iter - (record_length - 1);
    }

    template <class Iterator>
    iterator push(Iterator begin, Iterator end, LabelType label)
    {
        size_t record_length = std::distance(begin, end);

        record_type & new_record = m_anno.add(label, record_length);
        typename trie_type::iterator trie_iter = m_trie.begin();

        link_type link;
        for (Iterator iter = begin; iter != end; ++iter)
        {
            byte symbol = *iter;
            data_type new_data = data_type();

            // Add trie node
            trie_iter = m_trie.insert(trie_iter, symbol);

            // Add annotation node and link annotation to trie node
            data_type & data = new_record.push(trie_iter.index(), symbol);
            // Add annotation data
            data = new_data;

            // Link trie node and annotation data
            trie_iter->push_back(m_anno.last());

            // Add statistics data and link it with the node
            m_stat.add(iter, end, trie_iter.index());
        }

        return trie_iter - (record_length - 1);
    }

    std::vector<data_type> getAnnotations(const_iterator iter)
    {
        std::vector<data_type> result;
        for (auto i = iter->begin(); i != iter->end(); ++i)
        {
            result.push_back(m_anno[*i]);
        }
        return result;
    }


    data_type & getAnnotationByLabel(const_iterator iter, LabelType const & label)
    {
        for (auto i = iter->begin(); i != iter->end(); ++i)
        {
            if (m_anno.getLabel(*i) == label)
            {
                return m_anno[*i];
            }
        }

        throw std::range_error("Shit happend!");
    }

    std::vector<LabelType> getLabels(const_iterator iter) const
    {
        std::vector<LabelType> result;
        for (auto i = iter->begin(); i != iter->end(); ++i)
        {
            result.push_back(m_anno.labelOf(*i));
        }
        return result;
    }

    iterator begin()
    {
        return m_trie.begin();
    }

    iterator end()
    {
        return m_trie.end();
    }

    const_iterator begin() const
    {
        return m_trie.begin();
    }

    const_iterator end() const
    {
        return m_trie.end();
    }

    iterator iter(index_type i)
    {
        return iterator(&m_trie, i);
    }

    const_iterator iter(index_type i) const
    {
        return const_iterator(&m_trie, i);
    }

    template <class S>
    void copyTrie(trie<S> ** t) const
    {
        *t = new trie<S>(m_trie);
    }

    template <class Iterator>
    const std::set<index_type>* getNodes(Iterator begin, Iterator end) const
    {
        return m_stat.get(begin, end);
    }

    record_type & getRecord(LabelType const & label)
    {
        return m_anno.getRecordByLabel(label);
    }

private:
    std::string m_name;
    trie_type   m_trie;
    anno_type   m_anno;
    kstat_type  m_stat;
};
