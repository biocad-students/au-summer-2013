#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>

struct settings_t {
	int max_threads, kmer_size;
	std::string reference_file, input_file, output_file;
};

#endif /* SETTINGS_H_ */
