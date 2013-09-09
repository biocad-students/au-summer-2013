#pragma once

#include <deque>
#include <vector>

// ������� ���� �� ������� ���� �������� int alphabet_size = 4
template<typename key_type>
inline size_t standard_ACGT_key_mapping(key_type _key)
{
        static const size_t key_map[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1,  0, -1,  1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1,  0, -1,  1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        return key_map[static_cast<unsigned char>(_key)];
}

template<typename key_type>
inline size_t standard_ACGT_key_mapping_N(key_type _key)
{
        static const size_t key_map[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, +4, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1,  0, -1,  1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, +4, -1,
                                         -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1,  0, -1,  1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        return key_map[static_cast<unsigned char>(_key)];
}

// _default_traits
// alphabet_size: ������ �������� �������� �����. �������������� ��� ����������� 4, ��� ����������� 20.
// symbol_type: ��� ������� � ��������. ��������, unsigned char, uint_16. �� ��������� char.
// link_type: ��� ���������� ������ � ����. ��������, size_t, unsigned int, uint_16.
// get_key_mapping(): ������� ��� �������� ����������� ������� ������� �� ��������.
//                    �������������� ���������� �� O(1) �� �������.
//                    ������, ����� ������ ��������� ���� ���������������� � ��������.
struct _default_traits
{
        enum { alphabet_size = 4 };
        typedef char symbol_type;
        typedef unsigned int link_type;
        template<typename T> struct container
        {
                typedef std::deque<T> container_type;
        };

        inline static size_t get_key_mapping(symbol_type _key) { return standard_ACGT_key_mapping(_key); }
};


struct _default_traits_N
{
        enum { alphabet_size = 5 };
        typedef char symbol_type;
        typedef unsigned int link_type;
        template<typename T> struct container
        {
//                typedef std::deque<T> container_type;
                typedef std::vector<T> container_type;
        };

        inline static size_t get_key_mapping(symbol_type _key) { return standard_ACGT_key_mapping_N(_key); }
};