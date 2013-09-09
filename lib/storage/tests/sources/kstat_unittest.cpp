#include "../include/unittest.h"
#include "../../include/kstat/kstat.hpp"

#include <vector>
#include <deque>

static void default_traits_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin default_traits_instatiation_unittest");

        kstat<7> kstat_instance;

        PERFOMANCE_TIME("End default_traits_instatiation_unittest");
}

static void insert_short_sequence_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin insert_short_sequence_unittest");

        kstat<7> kstat_instance;

        std::string str("ACGT");
        kstat_instance.add(str.begin(), str.end(), 1);

        PERFOMANCE_TIME("End insert_short_sequence_unittest");
}

static void insert_long_sequence_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin insert_long_sequence_unittest");

        kstat<7> kstat_instance;

        std::string str("ACGTACGT");
        kstat_instance.add(str.begin(), str.end(), 2);

        PERFOMANCE_TIME("End insert_long_sequence_unittest");
}

static void insert_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin insert_unittest");

        kstat<7> kstat_instance;

        std::string str("ACGTACG");
        kstat_instance.add(str.begin(), str.end(), 3);

        PERFOMANCE_TIME("End insert_unittest");
}

static void multiple_insert_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin multiple_insert_unittest");

        kstat<7> kstat_instance;

        std::string str("AAACCCGGGTTT");
        for(std::string::iterator p = str.begin(); p != str.end() - 7; ++p)
        {
                kstat_instance.add(str.begin(), str.begin() + 7, std::distance(p, str.begin()));
        }

        PERFOMANCE_TIME("End multiple_insert_unittest");
}

static void find_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin find_unittest");

        kstat<7> kstat_instance;

        std::string str("AAACCCGGGTTTATATATATATATATATAT");
        for(std::string::iterator p = str.begin(); p != str.end() - 7; ++p)
        {
                kstat_instance.add(p, p + 7, std::distance(str.begin(), p));
        }

        std::string needle("ACCCGGG");
        std::pair<std::set<kstat<7>::link_type>::const_iterator,
                  std::set<kstat<7>::link_type>::const_iterator> result = kstat_instance.find(needle.begin(), needle.end());

        for(std::set<kstat<7>::link_type>::const_iterator i = result.first; i != result.second; ++i)
        {
                std::cout << *i << ' ';
        }
        std::cout << std::endl;

        std::string needle2("ATATATA");
        result = kstat_instance.find(needle2.begin(), needle2.end());

        for(std::set<kstat<7>::link_type>::const_iterator i = result.first; i != result.second; ++i)
        {
                std::cout << *i << ' ';
        }
        std::cout << std::endl;

        PERFOMANCE_TIME("End find_unittest");
}


void run_kstat_unittests(void)
{
        default_traits_instatiation_unittest();
        insert_short_sequence_unittest();
        insert_long_sequence_unittest();
        insert_unittest();
        multiple_insert_unittest();
        find_unittest();
}
