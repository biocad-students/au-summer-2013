#include <iostream>
#include <fstream>
#include <string>
#include "config_reader.h"

po::variables_map ConfigReader::read_settings() {
	std::ifstream settings_file( "./config/settings.ini" );

	po::options_description desc("Options");
	desc.add_options()
			("data.reference_file", "reference file with igs")
			("data.input_file", "input fasta files")
			("settings.max_nthreads", "max threads");
	po::variables_map vm;
	po::store(po::parse_config_file(settings_file, desc), vm);
	settings_file.close();
	po::notify(vm);

	return vm;
}
