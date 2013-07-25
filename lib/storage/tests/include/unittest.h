#pragma once

#include <string>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "../../common/include/fasta_reader.h"
#include "../../common/include/scorematrix.h"
#include "../../include/storage/storage.hpp"
#include "../../include/kstat/kstat.hpp"
#include "../../include/annotation/annotation.hpp"

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

extern void fill_alphabet(std::vector<unsigned char>* _alphabet);

void kstat_unittest(void);
void storage_unittest(void);
void annotation_unittest(void);
void matrix_unittest(void);
void find_unittest(void);
void align_unittest(void);