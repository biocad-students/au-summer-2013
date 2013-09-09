#pragma once
// IGC - ImmunoGlobulin Container
#if 1

#include "../annotation/annotation.hpp"
#include "../trie/trie.hpp"
#include "../kstat/kstat.hpp"
#include <algorithm>

template <typename _Property, typename _Label = std::string, class _Traits = _default_traits_N, int _k = 7>
class storage
{
public:
        enum { alphabet_size = _Traits::alphabet_size };
        typedef typename _Traits::symbol_type symbol_type;
        typedef typename _Traits::link_type link_type;
        typedef _Property property_type;
        typedef _Label label_type;
        class const_iterator;
        class iterator;

private:
        typedef storage<property_type, label_type, _Traits> _My_type;
        typedef annotation_sequence_registry<property_type, label_type, _Traits> annotation_registry_type;
        typedef trie<std::vector<typename annotation_registry_type::index_type>, _Traits> trie_type;
        typedef kstat<_k, _Traits> kstat_type;

public:
        class iterator {};

        storage()
        {
        }

        ~storage()
        {
        }

        template <class _Iter>
        void pushSequence(_Iter _begin, _Iter _end, label_type _label)
        {
                //_Iter iter = _begin;
                typename trie_type::iterator trie_iter = m_trie.root();
                //annotation_registry_type::annotation_sequence &seq = m_annotation_registry[_label];
                annotation_registry_type::annotation_sequence &seq = m_annotation_registry.add(_label);

                assert(seq->empty());
                seq->reserve(std::distance(_begin, _end));

                for(; _begin != _end; ++_begin)
                {
                        trie_iter = m_trie.insert(trie_iter, *_begin);
                        annotation_registry_type::annotation curr_annotation(property_type());
                        seq->push_back(property_type());
                        // Далее идет ошибочный код:
                        // Сейчас нет уверенности, что m_annotation_registry.sequence_count() дает то, что нужно
                        // Следует продумать интерфейс annotation_sequence_registry
                        //trie_iter->push_back(m_annotation_registry.sequence_count() - 1);
                        // Исправление:
                        trie_iter->push_back(seq.id());

                        if(_begin <= _end - _k)
                        {
                                m_kstat.add(_begin, _begin + _k, trie_iter.index());
                        }
                }
        }

        template <class _Iter>
        iterator find(_Iter _begin, _Iter _end)
        {
		std::vector<int> result;

                trie<char, _Traits> color_trie(m_trie);
                std::fill(color_trie.begin(), color_trie.end(), false);

                size_t dist = std::distance(_begin, _end);
                for(size_t i = 0; i < dist - _k; ++i, ++_begin)
                {
                        typename kstat_type::find_result_type matches = m_kstat.find(_begin, _begin + _k);
                        typename kstat_type::const_iterator match_iter = matches.first;
                        while(match_iter != matches.second) {
                                typename trie<char, _Traits>::iterator trie_iter = color_trie.find(*match_iter);
                                assert(trie_iter != color_trie.end());
                                *trie_iter = true;
                                ++match_iter;
                        }
                }
                typename kstat_type::find_result_type matches = m_kstat.find(_begin, _begin + _k);
                typename kstat_type::const_iterator match_iter = matches.first;
                for(; match_iter != matches.second; ++match_iter)
                {
                        typename trie<char, _Traits>::iterator trie_iter = color_trie.find(*match_iter);
                        typename trie<char, _Traits>::const_iterator last_iter = trie_iter;
                        bool found = true;
                        for(int c = dist - _k; c > 0; --c)
                        {
                                --trie_iter;
                                if(trie_iter == color_trie.root() || !*trie_iter)
                                {
                                        found = false;
                                        break;
                                }
                        }
                        if(found)
                        {
				result.push_back( last_iter.index() );
                        }
                }

		//return iterator(*this, result);
                return iterator();
        }

        template <class Iterator>
        std::vector<std::string> align(Iterator _begin, Iterator _end, size_t _n = 1)
        {
                std::vector<std::string> result;
                std::vector<std::pair<size_t, std::string> > results_with_scores;

                size_t distance = std::distance(_begin, _end);

                struct Cell
                {
                        size_t score;
                        char backtrace;
                };
                trie<std::vector<Cell>, _Traits> align_trie(m_trie);

                (*align_trie.root()).resize(distance + 1);
                (*align_trie.root())[0].score = 0;
                (*align_trie.root())[0].backtrace = ' ';
                for(size_t i=1; i <= distance; ++i)
                {
                        (*align_trie.root())[i].score = i;
                        (*align_trie.root())[i].backtrace = '|';
                }

                bool need_to_cleanup = false;
                typename trie<std::vector<Cell>, _Traits>::iterator iter = align_trie.DFS().begin();
                for(; iter != align_trie.DFS().end(); ++iter)
                {
                        if(need_to_cleanup)
                        {
                                typename trie<std::vector<Cell>, _Traits>::iterator iter_garbage_collector = iter;
                                typename trie<std::vector<Cell>, _Traits>::iterator parent = iter.parent();
                                while(--iter_garbage_collector != parent)
                                {
                                        (*iter_garbage_collector).swap(std::vector<Cell>());
                                }
                                need_to_cleanup = false;
                        }

                        (*iter).resize(distance + 1);
                        (*iter)[0].score = (*iter.parent())[0].score + 1;
                        (*iter)[0].backtrace = '-';
                        Iterator str_iter = _begin;
                        for(size_t i = 1; str_iter != _end; ++str_iter, ++i)
                        {
                                size_t best_score = (*iter.parent())[i].score + 1;
                                char backtrace = '-';

                                size_t score = (*iter)[i - 1].score + 1;
                                if(score < best_score)
                                {
                                        best_score = score;
                                        backtrace = '|';
                                }

                                score = (*iter.parent())[i - 1].score + (*str_iter == iter.key()? 0: 1);
                                if(score < best_score)
                                {
                                        best_score = score;
                                        backtrace = (*str_iter == iter.key()? *str_iter: '*');
                                }

                                (*iter)[i].score = best_score;
                                (*iter)[i].backtrace = backtrace;
                        }
                        // Тут можно получать эту информацию как-то иначе
                        if(iter.is_leaf())
                        {
                                std::string s;

                                typename trie<std::vector<Cell> >::iterator iter_back = iter;
                                for(size_t i = distance; iter_back != align_trie.root() && i != 0; )
                                {
                                        s.push_back((*iter_back)[i].backtrace);
                                        switch((*iter_back)[i].backtrace)
                                        {
                                        case '-':
                                                iter_back = iter_back.parent();
                                                break;
                                        case '|':
                                                --i;
                                                break;
                                        default:
                                                --i;
                                                iter_back = iter_back.parent();
                                        }
                                }

                                std::reverse(s.begin(), s.end());
                                results_with_scores.push_back(std::make_pair((*iter)[distance].score,s));
                                need_to_cleanup = true;
                        }
                }

                struct LocalNamespace
                {
                        static bool CompareScores (const std::pair<size_t, std::string>& left, const std::pair<size_t, std::string>& right)
                        {
                                return (left.first < right.first);
                        }
                };

                if(results_with_scores.size() > _n)
                {
                        std::partial_sort(results_with_scores.begin(), results_with_scores.begin() + _n, results_with_scores.end(), LocalNamespace::CompareScores);
                }
                else
                {
                        std::sort(results_with_scores.begin(), results_with_scores.end(), LocalNamespace::CompareScores);
                }

                for(size_t i = 0; i < _n; ++i)
                {
                        result.push_back(results_with_scores[i].second);
                }

                return result;
        }


private:
        annotation_registry_type m_annotation_registry;
        trie_type m_trie;
        kstat_type m_kstat;
};

#else
#include <vector>
#include "../../common/include/scorematrix.h"
#include "../annotation/annotation.hpp"
#include "../kstat/kstat.hpp"
#include "../trie/trie.hpp"

namespace igc {
// typedef T - property param
// typedef Property - annotation stored type
// typedef Label - record label type
template <class T, template <class> class Property, class Label = std::string>
class storage
{
public:
	typedef storage<T, Property, Label> _My_type;
	typedef annotation<T, Property, Label> annotation_t;
	typedef typename annotation_t::offset_t offset_t;
	typedef trie<std::vector<offset_t>> trie_t;
	typedef Kstat kstat_t;
	typedef typename kstat_t::alphabet_t alphabet_t;
	typedef std::vector<std::vector<int>> matrix_t;

	class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
	{
	public:
		const_iterator(_My_type &_storage, std::vector<int>& _data)
			: m_storage(_storage), m_data(_data), m_index(0)
		{
		}

		const_iterator(const const_iterator &_right)
			: m_storage(_right.m_storage), m_data(_right.m_data), m_index(_right.m_index)
		{
		}

		const_iterator& operator = (const const_iterator &_right)
		{
			m_storage = _right.m_storage;
			m_data = _right.m_data;
			m_index = _right.m_index;
			return *this;
		}

		bool valid() const
		{
			return m_index >= 0 && m_index < m_data.size();
		}

		std::vector<Label> get_labels()
		{
			std::vector<Label> result;
			typename trie_t::iterator it = m_storage.m_trie.find(m_data[m_index]);
			for(int i=0; i<(*it).size(); ++i)
			{
				int top = (*it)[i].first;
				result.push_back(m_storage.m_annotation.getRecordData(top));
			}
			return result;
		}

		const T& operator*() const
		{
		}

		const T* operator->() const
		{
			return (&**this);
		}

		//bool operator == (const const_iterator& _right) const
		//{
		//	//return m_node == _right.m_node;
		//}

		//bool operator!=(const const_iterator& _right) const
		//{
		//	return (!(*this == _right));
		//}

		const_iterator& operator++()
		{
			++m_index;
			return *this;
		}

		//const_iterator operator++(int)
		//{
		//	_My_type_iter _Tmp = *this;
		//	++*this;
		//	return _Tmp;
		//}

		//const_iterator& operator--()
		//{
		//	--m_index;
		//	return *this;
		//}

		//const_iterator operator--(int)
		//{
		//	_My_type_iter _Tmp = *this;
		//	--*this;
		//	return _Tmp;
		//}

    //Key_type key() const
    //{
    //  return m_node->cached_key;
    //}

    //Index_type index() const
    //{
    //  return m_node->index;
    //}

	private:
		_My_type m_storage;
		std::vector<int> m_data;
		int m_index;
	};

	class iterator : public const_iterator
	{
	public:
		iterator(_My_type& _storage, std::vector<int>& _data)
			: const_iterator(_storage, _data)
		{
		}

		T& operator*() const
		{	// return designated value
			return ((T&)**(const_iterator *)this);
		}

		T* operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		iterator& operator++()
		{	// preincrement
			++(*(const_iterator *)this);
			return (*this);
		}

		iterator operator++(int)
		{	// postincrement
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		//iterator& operator--()
		//{	// preincrement
		//	--(*(const_iterator *)this);
		//	return (*this);
		//}

		iterator operator--(int)
		{	// postincrement
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};

public:
	storage(alphabet_t _alphabet, size_t _K) : m_kstat(_alphabet, _K) {
	}

	~storage() {
	}

	template <class Iterator>
	void pushSequence(Iterator _begin, Iterator _end, Label _label) {
		Iterator iter = _begin;
		typename trie_t::iterator lastTrie = m_trie.root();
		m_annotation.push_back(_label, std::distance(_begin, _end));	
		size_t k = m_kstat.getK();
		Property<char> tmpprp = Property<char>("");
		while(iter != _end) {
			lastTrie = m_trie.insert(lastTrie, *iter);

			m_annotation.insert_back(lastTrie.index(), tmpprp);

			lastTrie->push_back(std::make_pair(m_annotation.size()-1, m_annotation.record_size(m_annotation.size()-1)));
			m_kstat.add(iter, _end, lastTrie.index());
			
			++iter;
		}
	}

	//template<typename Y, typename Key_type, typename Link>
	//trie<Y, Key_type, Link> copyTrie () {
	//	trie<T, Key_type, Link> tmp = m_trie;
	//	return tmp;
	//}

	template <class Iterator>
	iterator find(Iterator _begin, Iterator _end) {
		std::vector<int> result;

		trie<char, char> my_color_trie(m_trie);
		std::fill(my_color_trie.begin(), my_color_trie.end(), false);

		size_t dist = std::distance(_begin, _end);
		size_t k = m_kstat.getK();
		for(size_t i = 0; i < dist - k; ++i, ++_begin)
		{
			std::set<size_t> *similar = m_kstat.get(_begin, _begin + k);
			if(similar == NULL)
				continue;
			typename std::set<size_t>::iterator similar_iter = (*similar).begin();
			while(similar_iter != similar->end()) {
				typename trie<char, char>::iterator iter = my_color_trie.find(*similar_iter);
				*iter = true;
				++similar_iter;
			}
		}
		std::set<size_t> *similar = m_kstat.get(_begin, _begin + k);
		typename std::set<size_t>::iterator similar_iter = similar->begin();
		for(;similar_iter != similar->end(); ++similar_iter)
		{
			typename trie<char>::const_iterator iter = my_color_trie.find(*similar_iter);
			typename trie<char>::const_iterator last_iter = iter;
			bool found = true;
			for(int c = dist - k; c > 0; --c)
			{
				--iter;
				if(iter == my_color_trie.root() || !*iter)
				{
					found = false;
					break;
				}
			}
			if(found)
			{
				result.push_back( last_iter.index() );
			}
		}

		return iterator(*this, result);
	}

	template <class Iterator>
	std::vector<std::string> align(Iterator _begin, Iterator _end, size_t _n = 1)
	{
		std::vector<std::string> result;
		std::vector<std::pair<size_t, std::string> > results_with_scores;

		size_t distance = std::distance(_begin, _end);

		struct Cell
		{
			size_t score;
			char backtrace;
		};
		trie<std::vector<Cell> > align_trie(m_trie);

		(*align_trie.root()).resize(distance + 1);
		(*align_trie.root())[0].score = 0;
		(*align_trie.root())[0].backtrace = ' ';
		for(int i=1; i <= distance; ++i)
		{
			(*align_trie.root())[i].score = i;
			(*align_trie.root())[i].backtrace = '|';
		}

		bool need_to_cleanup = false;
		typename trie<std::vector<Cell> >::iterator iter = align_trie.begin();
		for(; iter != align_trie.end(); ++iter)
		{
			if(need_to_cleanup)
			{
				typename trie<std::vector<Cell> >::iterator iter_back = iter;
				while(--iter_back != iter.parent())
				{
					(*iter_back).swap(std::vector<Cell>());
				}
				need_to_cleanup = false;
			}

			(*iter).resize(distance + 1);
			(*iter)[0].score = (*iter.parent())[0].score + 1;
			(*iter)[0].backtrace = '-';
			Iterator str_iter = _begin;
			for(size_t i = 1; str_iter != _end; ++str_iter, ++i)
			{
				size_t best_score = (*iter.parent())[i].score + 1;
				char backtrace = '-';

				size_t score = (*iter)[i - 1].score + 1;
				if(score < best_score)
				{
					best_score = score;
					backtrace = '|';
				}

				score = (*iter.parent())[i - 1].score + (*str_iter == iter.key()? 0: 1);
				if(score < best_score)
				{
					best_score = score;
					backtrace = (*str_iter == iter.key()? *str_iter: '*');
				}


				(*iter)[i].score = best_score;
				(*iter)[i].backtrace = backtrace;
			}
			if(iter.is_leaf())
			{
				std::string s;

				typename trie<std::vector<Cell> >::iterator iter1 = iter;
				for(size_t i = distance; iter1 != align_trie.root() && i != 0; )
				{
					s.push_back((*iter1)[i].backtrace);
					switch((*iter1)[i].backtrace)
					{
					case '-':
						iter1 = iter1.parent();
						break;
					case '|':
						--i;
						break;
					default:
						--i;
						iter1 = iter1.parent();
					}
				}

				std::reverse(s.begin(), s.end());
				results_with_scores.push_back(std::make_pair((*iter)[distance].score,s));
				need_to_cleanup = true;
			}
		}

		struct LocalNamespace
		{
			static bool CompareScores (const std::pair<size_t, std::string>& i, const std::pair<size_t, std::string>& j) { return (i.first<j.first); }
		};

		if(results_with_scores.size() > _n)
		{
			std::partial_sort(results_with_scores.begin(), results_with_scores.begin() + _n, results_with_scores.end(), LocalNamespace::CompareScores);
		}
		else
		{
			std::sort(results_with_scores.begin(), results_with_scores.end(), LocalNamespace::CompareScores);
		}

		for(int i = 0; i < _n; ++i)
		{
			result.push_back(results_with_scores[i].second);
		}

		return result;
	}

private:
	annotation_t m_annotation;
	trie_t m_trie;
	kstat_t m_kstat;
};
}
#endif
