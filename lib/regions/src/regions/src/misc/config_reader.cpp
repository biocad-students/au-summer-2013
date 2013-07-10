#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config_reader.h"

struct settings_t ConfigReader::read_settings() {
	std::ifstream settings_file( "./config/config.ini" );

	po::options_description desc("Options");
	desc.add_options()
			("data.reference_file", po::value<std::string>(), "reference file with igs")
			("data.input_file", po::value<std::string>(), "input fasta files")
			("data.output_file", po::value<std::string>(), "output files")
			("settings.max_nthreads", po::value<std::string>(), "max threads")
			("settings.kmer_size", po::value<std::string>(), "kmers size");
	po::variables_map vm;
	po::store(po::parse_config_file(settings_file, desc, true), vm);
	settings_file.close();
	po::notify(vm);

	struct settings_t settings;
	settings.max_threads = atoi((vm["settings.max_nthreads"].as<std::string>()).c_str());
	settings.kmer_size = atoi((vm["settings.kmer_size"].as<std::string>()).c_str());
	settings.reference_file = vm["data.reference_file"].as<std::string>();
	settings.input_file = vm["data.input_file"].as<std::string>();
	settings.output_file = vm["data.output_file"].as<std::string>();

	return settings;
}
