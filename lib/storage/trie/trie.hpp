#pragma once

#include <unordered_map>
#include <vector>
#include <string>

template <class NodeType, class KeyType=int, class NodeProperties=int>
class TrieNode
{
public:
    // Constructors
    TrieNode(NodeType const & val, size_t number_of_children=0)
    {
        throw "Unrealized";
    }

    TrieNode(TrieNode const & trie_node)
    {
        throw "Unrealized";
    }

    TrieNode(TrieNode && trie_node)
    {
        throw "Unrealized";
    }

    // Methods
    void add_child(NodeType child)
    {
        throw "Unrealized";
    }

    void del_child(NodeType child)
    {
        throw "Unrealized";
    }

    TrieNode* get_child(NodeType child)
    {
        throw "Unrealized";
    }

    std::vector<TrieNode> get_children()
    {
        throw "Unrealized";
    }

    bool has_keys()
    {
        throw "Unrealized";
    }

    std::vector<KeyType> keys()
    {
        throw "Unrealized";
    }

    NodeProperties get_node_properties()
    {
        throw "Unrealized";
    }

private:
    std::vector<NodeType*> m_children;
    std::vector<KeyType>   m_node_keys;
    NodeProperties         m_node_properties;
};


template <class NodeType, class KeyType=int, class Properties=int, class NodeProperties=int>
class Trie
{
public:
    // Constructors and destructor
    Trie() {}

    Trie(std::vector<std::vector<NodeType>> const & strings)
    {
        throw "Unrealized";
    }

    Trie(sed::vector<std::vector<NodeType>> && strings)
    {
        throw "Unrealized";
    }

    Trie(Trie const & trie)
    {
        throw "Unrealized";
    }

    Trie(Trie && trie)
    {
        throw "Unrealized";
    }

    ~Trie()
    {
        throw "Unrealized";
    }

    // Data manipulation methods
    bool add_string(KeyType const & key, std::vector<NodeType> const & string)
    {
        throw "Unrealized";
    }

    bool del_string(KeyType const & key)
    {
        throw "Unrealized";
    }

    std::vector<NodeType> get(KeyType const & key)
    {
        throw "Unrealized";
    }

    KeyType find(std::vector<NodeType> const & string)
    {
        throw "Unrealized";
    }

    template<class StatisticsCounter>
    StatisticsCounter::stat_hash get_statistics()
    {
        throw "Unrealized";
    }

    template<class StatisticsCounter>
    Trie get_sub_trie_by_statistics(StatisticsCounter::stat_hash const & statistics)
    {
        throw "Unrealized";
    }

    Trie align_trie(std::vector<NodeType> const & string)
    {
        throw "Unrealized";
    }

    Trie multiple_align_trie(std::vector<std::vector<NodeType>> const & strings)
    {
        throw "Unrealized";
    }

    Properties get_properties()
    {
        throw "Unrealized";
    }

    template<template<class Properties, class NodeProperties> class Serizalizer>
    bool save_to_file(std::string const & filename)
    {
        throw "Unrealized";
    }

    template<template<class Properties, class NodeProperties> class Serizalizer>
    bool load_from_file(std::string const & filename)
    {
        throw "Unrealized";
    }

private:
    std::vector<KeyType>                        m_keys;
    TrieNode<NodeType, KeyType, NodeProperties> m_root;
    Properties                                  m_properties;
};