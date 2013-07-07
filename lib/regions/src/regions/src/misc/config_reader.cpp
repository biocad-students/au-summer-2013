#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config_reader.h"

po::variables_map ConfigReader::read_settings() {
	std::ifstream settings_file( "./config/config.ini" );

	po::options_description desc("Options");
	desc.add_options()
			("data.reference_file", po::value<std::string>(), "reference file with igs")
			("data.input_file", po::value<std::string>(), "input fasta files")
			("settings.max_nthreads", po::value<std::string>(), "max threads");
	po::variables_map vm;
	po::store(po::parse_config_file(settings_file, desc, true), vm);
	settings_file.close();
	po::notify(vm);

	return vm;
}
