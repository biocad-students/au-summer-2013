#include <ctime>
#include <iostream>
#include <algorithm>
#include "unittest.h"

using namespace std;

typedef unsigned char byte;

void fill_alphabet(std::vector<unsigned char>* _alphabet) {
	_alphabet->push_back('A');
	_alphabet->push_back('C');
	_alphabet->push_back('G');
	_alphabet->push_back('T');
}


void test()
{
    //time_t s = clock();

    //test_trie();
    //test_anno();
    //test_kstat();
    //test_contig();
    //test_contig2();
    //test_search();
    //test_fasta();
    //test_fasta_push();
    //test_alicont();
    //test_contig_alicont();
    //test_contig_alicont2();
    //s = clock() - s;
    //std::cout << "Test time: " << (double) s / CLOCKS_PER_SEC << std::endl;
}

template<class T, template<class> class Property, class Label>
void process (std::stringstream& _ss, igc::contig<T, Property, Label>* _storage)
{
	std::string arg;

	std::string file;
	std::string seq;
	std::string matrix;
	int count;

	while(_ss >> arg) {
		if (arg=="add") {
			std::string type;
			_ss >> type;
			if(!type.empty()) {
				if(type=="seq") {
					std::string name;
					std::string seq;
					_ss >> name;
					_ss >> seq;
					if(!name.empty() && !seq.empty()) {
						_storage->push(seq.begin(), seq.end(), Lab(name));
					}
					else {
						std::cout << "Empty name or seq argument in add command" << std::endl;
						return;
					}
				}
				else if(type=="file") {
					std::string path;
					_ss >> path;
					if(!path.empty()) {
						FastaReader FR(path);
						if(FR.is_eof()) {
							std::cout << "No file found at path specified: " << path << std::endl;
							return;
						}
						Read tmp;
						std::string::iterator iter;
						std::string::iterator end;
						while(!FR.is_eof()) {
							FR >> tmp;
							iter = tmp.seq.begin();
							end = tmp.seq.end();
							_storage->push(iter, end, Lab(tmp.name));
						}
					}
					else {
						std::cout << "Empty path argument in add command" << std::endl;
						return;
					}
				}
				else {
					std::cout << "Unknown argument in add command: " << type << std::endl;
					return;
				}
			}
			else {
				std::cout << "Empty argument in add command" << std::endl;
				return;
			}

		}
		else if (arg=="find") {
			std::string seq;
			_ss >> seq;
			if(!seq.empty()) {
				std::vector<size_t> result = _storage->find(seq.begin(), seq.end());
				int size = result.size();
				std::cout << size << std::endl;
				return;
			}
			else {
				std::cout << "Empty argument in find command" << std::endl;
				return;
			}
		}
		else if (arg=="align") {
		}
		else {
			std::cout << "Unknown command: " << arg << std::endl;
			return;
		}
	}
}

int main()
{
    //test();
	std::vector<unsigned char> alphabet_;
	fill_alphabet(&alphabet_);
	int K_ = 7;
	igc::contig<char, Prop, Lab> my_storage("main", alphabet_, K_);
	std::cout << "Welcome to IG Storage" << std::endl;
	while(1) {
		std::string input;
		std::stringstream ss;
		std::cout << ">> ";
		std::getline(std::cin, input);
		ss << input;
		process(ss, &my_storage);
	}
    return 0;
}
