#pragma once

#include <deque>
#include <vector>
#include <string>

#include "trie/trie.hpp"
#include "annotation/annotation.hpp"
#include "kstat/kstat.hpp"

#include "contig_iterator.hpp"

template <class T, template <class> class Property, class LabelType=std::string>
class contig
{
public:
	typedef Property<T>                        		data_type;
	typedef std::vector<byte>				   		alphabet_type;
	typedef size_t							   		kstart_link_type;
	typedef annotation<T, Property, LabelType> 		anno_type;
	typedef typename anno_type::index          		link_type;
	typedef typename anno_type::record_type    		record_type;
	typedef kstatistics<kstart_link_type>      		kstat_type;
	typedef trie<link_type>                    		trie_type;
	typedef typename trie_type::iterator       		iterator;
	typedef contig_iterator<T, Property, LabelType> aiterator;

	contig(std::string const & name, alphabet_type const & alphabet, size_t k = 7)
		: m_name(name), m_stat(alphabet, k)
	{
	}

	template <class Iterator>
	iterator push(Iterator begin, Iterator end, LabelType label = LabelType())
	{
		size_t record_length = std::distance(begin, end);

		record_type new_record = m_anno->add(label, record_length);
		iterator trie_iter = m_trie.begin();

		link_type link;

		std::deque<byte> kmer_cache;
		for (Iterator iter = begin; iter != end; ++iter)
		{
			kmer_cache.push_back(iter->first);
		}

		for (Iterator iter = begin; iter != end; ++iter)
		{
			byte symbol = iter->first;
			data_type new_data = iter->second;

			// Add trie node
			trie_iter = m_trie.insert(trie_iter, symbol);

			// Add annotation node and link annotation to trie node
			data_type & data = new_record.push(trie_iter.index(), symbol);
			// Add annotation data
			data = new_data;

			// Link trie node and annotation data
			*trie_iter = m_anno.last();

			// Add statistics data and link it with the node
			m_stat.add(kmer_cache.begin(), kmer_cache.end(), trie_iter.index());
			kmer_cache.pop_front();
		}

		return trie_iter - (record_length - 1);
	}

	template <class Iterator>
	iterator push_unannotated(Iterator begin, Iterator end, LabelType label = LabelType())
	{
		size_t record_length = std::distance(begin, end);

		record_type new_record = m_anno.add(label, record_length);
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
			*trie_iter = m_anno.last();

			// Add statistics data and link it with the node
			m_stat.add(iter, end, trie_iter.index());
		}

		return trie_iter - (record_length - 1);
	}

	Property<T> & getAnnotation(iterator iter)
	{
		return m_anno[*iter];
	}

	LabelType getLabel(iterator iter) const
	{
		return m_anno.labelOf(*iter);
	}

	iterator begin()
	{
		return m_trie.begin();
	}

	iterator end()
	{
		return m_trie.end();
	}

	aiterator abegin()
	{
		return aiterator(this, begin());
	}

	aiterator aend()
	{
		return aiterator(this, end());
	}

	template <class S>
	void copyTrie(trie<S> ** t) const
	{
		*t = new trie<S>(m_trie);
	}

private:
	std::string m_name;
	trie_type   m_trie;
	anno_type   m_anno;
	kstat_type  m_stat;
};
