#include "../include/unittest.h"

#include "../../include/annotation/annotation.hpp"

#include <vector>

static void default_traits_instatiation_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin default_traits_instatiation_unittest");

        annotation_sequence_registry< std::string > annotation_seq_registry_instance;

        PERFOMANCE_TIME("End default_traits_instatiation_unittest");
}

static void access_new_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin access_new_unittest");

        annotation_sequence_registry<std::string> instance;

        annotation_sequence_registry<std::string>::annotation_sequence seq = instance["Seq 1"];

        PERFOMANCE_TIME("End access_new_unittest");
}

static void add_unittest(void)
{
        PERFOMANCE_INIT();
        PERFOMANCE_TIME("Begin add_unittest");

        annotation_sequence_registry<std::string> instance;

        //Prop<int> tmpprp = Prop<int>("prop");


        //annotation_sequence_registry<std::string>::annotation_sequence &seq1 = instance["Seq 1"];
        //seq1.push_back(annotation_sequence_registry<std::string>::annotation("Property 1.1", std::make_pair(1, 1)));
        //seq1.push_back(annotation_sequence_registry<std::string>::annotation("Property 1.2", std::make_pair(1, 2)));
        //seq1.push_back(annotation_sequence_registry<std::string>::annotation("Property 1.3", std::make_pair(1, 3)));
        //seq1.push_back(annotation_sequence_registry<std::string>::annotation("Property 1.4", std::make_pair(1, 4)));
        //annotation_sequence_registry<std::string>::annotation_sequence &seq2 = instance["Seq 2"];
        //seq2.push_back(annotation_sequence_registry<std::string>::annotation("Property 2.1", std::make_pair(2, 1)));
        //seq2.push_back(annotation_sequence_registry<std::string>::annotation("Property 2.2", std::make_pair(2, 2)));

        annotation_sequence_registry<std::string>::annotation_sequence &seq1 = instance["Seq 1"];
        seq1->push_back(annotation_sequence_registry<std::string>::annotation("Property 1.1"));
        seq1->push_back(annotation_sequence_registry<std::string>::annotation("Property 1.2"));
        seq1->push_back(annotation_sequence_registry<std::string>::annotation("Property 1.3"));
        seq1->push_back(annotation_sequence_registry<std::string>::annotation("Property 1.4"));
        annotation_sequence_registry<std::string>::annotation_sequence &seq2 = instance["Seq 2"];
        seq2->push_back(annotation_sequence_registry<std::string>::annotation("Property 2.1"));
        seq2->push_back(annotation_sequence_registry<std::string>::annotation("Property 2.2"));

        PERFOMANCE_TIME("End add_unittest");
}


void run_annotation_unittests(void)
{
        default_traits_instatiation_unittest();
        access_new_unittest();
        add_unittest();
}
