#pragma once

#include <vector>
#include <queue>
#include <map>
#include <exception>
#include <unordered_map>

template <class T>
class trie_node
{
public:
	typedef std::unordered_map<T, trie_node*> children_t;

	trie_node(trie_node<T>* parent, T const & pval, trie_node<T> *sufflink = nullptr)
		: m_parent(parent), m_parent_val(pval), m_sufflink(sufflink), m_out(-1)
	{
	}

	~trie_node()
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
			delete it->second;
	}

	trie_node<T> * parent()
	{
		return m_parent;
	}

	void setOut(size_t out)
	{
		m_out = out;
	}

	size_t getOut()
	{
		return m_out;
	}

	void addMoreOut(size_t out)
	{
		m_addout.push_back(out);
	}

	std::vector<size_t> & getMoreOut()
	{
		return m_addout;
	}

	void setSufflink(trie_node<T>* sufflink)
	{
		m_sufflink = sufflink;
	}

	trie_node<T>* getSufflink()
	{
		return m_sufflink;
	}

	trie_node<T>* go(T const & c)
	{
		children_t::iterator i = m_children.find(c);
		if (i != m_children.end())
			return i->second;
		return nullptr;
	}

	bool hasNext(T const & c)
	{
		return m_children.find(c) != m_children.end();
	}

	trie_node<T>* & operator[](T const & key)
	{
		return m_children[key];
	}

	trie_node<T> * begin()
	{
		return m_children.begin();
	}

	trie_node<T> * end()
	{
		return m_children.end();
	}

	bool isLeaf()
	{
		return m_out >= 0;
	}

	T getParentVal()
	{
		return m_parent_val;
	}

private:
	children_t          m_children;
	trie_node<T>*		m_parent;
	T                   m_parent_val;
	trie_node<T>*       m_sufflink;
	size_t              m_out;
	std::vector<size_t> m_addout;
};


template <class T>
class trie_impl
{
public:
	trie_impl() : m_counter(0), m_root(new trie_node<T>(nullptr, T()))
	{
	}

	~trie_impl()
	{
		delete m_root;
	}

	template <class Iterator>
	std::pair<size_t, trie_node<T>*> pushSequence(Iterator start, Iterator end)
	{
		bool exist_flag = true;
		trie_node<T> *current = m_root;
		for (auto it = start; it != end; ++it)
		{
			T key = *it;
			trie_node<T>* child = current->go(key);
			if (child == nullptr)
			{
				exist_flag = false;
				(*current)[key] = new trie_node<T>(current, key, m_root);
			}
			current = (*current)[key];
		}
		if (!exist_flag)
			current->setOut(m_counter++);

		return std::make_pair(current->getOut(), current);
	}

	template <template <class> class Container>
	std::pair<size_t, trie_node<T>*> pushSequence(Container<T> const & sequence)
	{
		pushSequence(sequence.begin(), sequence.end());
	}

	void build()
	{
		std::queue<trie_node<T>*> q;
		q.push(&m_root);
		while (!q.empty())
		{
			trie_node<T>* current = q.front();
			q.pop();

			for (auto it = current->begin(); it != current->end(); ++it)
			{
				T key = it->first;
				trie_node<T>* next_child = it->second;

				q.push(next_child);
				trie_node<T>* sufflink = current->getSufflink();
				while (sufflink != nullptr && sufflink->hasNext(key))
				{
					sufflink = sufflink->getSufflink();
				}
				next_child->setSufflink(sufflink ? sufflink->go(key) : &m_root);
				for (auto it = next_child->getSufflink()->getMoreOut().begin();
						  it != next_child->getSufflink()->getMoreOut().end();
						  ++it)
				{
					next_child->getMoreOut().push_back(*it);
				}
			}
		}
	}

private:
	trie_node<T>* m_root;
	size_t        m_counter;
};
