#pragma once

#include <string>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

extern clock_t g_begin;

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000
#endif

#define PERFOMANCE_INIT() g_begin = clock();
#define PERFOMANCE_TIME(label) std::cout << label << " " << (double)(clock() - g_begin)/CLOCKS_PER_SEC << " at line " << __LINE__ << std::endl;

template <class T>
struct Prop {
	typedef T alphabet;
	std::string name;
	Prop(std::string name_) : name(name_) {}
	Prop() : name("") {}
};

struct Lab {
	std::string m_name;
	Lab(std::string _name) : m_name(_name) {}
};


void run_trie_topology_unittests(void);
void run_trie_unittests(void);
void run_kstat_unittests(void);
void run_annotation_unittests(void);
void run_storage_unittests(void);
