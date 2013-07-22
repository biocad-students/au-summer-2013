#include <map>
namespace igc {
// Trie node class for trie topology and trie
template<typename T, typename Link>
struct Node
{
	typedef typename std::map<T, Node*> child_t;

	Node(Link _index, Node *_parent, T _cached_key, bool _is_root, bool _is_end)
		: m_index(_index), m_parent(_parent), m_cached_key(_cached_key), m_is_root(_is_root), m_is_end(_is_end)
	{
	}

	~Node()
	{
		for (auto i = m_child.begin(); i != m_child.end(); ++i)
		{
			delete i->second;
		}
	}

	Node* get_child_for(T _key)
	{
		if(m_child.count(_key) == 1)
		{
			return m_child[_key];
		}
		return NULL;
	}

	Link m_index;
	child_t m_child;
	Node* m_parent;
	bool m_is_root;
	bool m_is_end;
	T m_cached_key;
};
}