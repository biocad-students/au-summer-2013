#include "../include/unittest.h"
#include "../../include/trie/trie.hpp"

#include <vector>
#include <deque>

struct Traits_vector
{
        enum { alphabet_size = 4 };
        typedef char symbol_type;
        typedef unsigned int link_type;
        template<typename T> struct container
        {
                typedef std::vector<T> container_type;
        };
};

struct Traits_deque
{
        enum { alphabet_size = 4 };
        typedef char symbol_type;
        typedef unsigned int link_type;
        template<typename T> struct container
        {
                typedef std::deque<T> container_type;
        };
};


static void default_traits_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin default_traits_instatiation_unittest");

        trie<int> instance;

        PERFOMANCE_TIME("End");
}

static void vector_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin vector_instatiation_unittest");

        trie<int, Traits_vector> instance;

        PERFOMANCE_TIME("End");
}


static void deque_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin deque_instatiation_unittest");

        trie<int, Traits_deque> instance;

        PERFOMANCE_TIME("End");
}

static void insert_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin insert_unittest");

        std::string str("ACGT");
        trie<int> trie_instance;

        trie<int>::iterator trie_iter = trie_instance.root();

        for(std::string::iterator str_iter = str.begin(); str_iter != str.end(); ++str_iter)
        {
                trie_iter = trie_instance.insert(trie_iter, *str_iter);
                (*trie_iter) = *str_iter;
        }

        PERFOMANCE_TIME("End");
}


void run_trie_unittests(void)
{
        default_traits_instatiation_unittest();
        vector_instatiation_unittest();
        deque_instatiation_unittest();
        insert_unittest();
}
