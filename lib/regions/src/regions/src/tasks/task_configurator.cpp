#include <log4cxx/logger.h>
#include <iostream>
#include <fstream>
#include "task_configurator.h"
#include "regions_finder.h"
#include "read_processor.h"
#include "fasta_reader.h"
#include "settings.h"

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

void TaskConfigurator::configureAndRun(const struct settings_t& settings) {
	LOG4CXX_INFO(logger, "Reading reference data...");
	Database db(settings);
	LOG4CXX_INFO(logger, "Done");

	std::ofstream output(settings.output_file);
	if (!output.is_open()) {
		LOG4CXX_ERROR(logger, "Cannot open output file");
		return;
	}

	AhoCorasick kmersAhoCorasick;
	TaskConfigurator::getKmersAhoCorasick(db, kmersAhoCorasick);
	RegionsFinder rf(&db, output, kmersAhoCorasick, settings.kmer_size);
	ReadProcessor rp(settings.max_threads);
	FastaReader input(settings.input_file);

	LOG4CXX_INFO(logger, "Start processing...");
	rp.readAndProcess(input, rf);
	LOG4CXX_INFO(logger, "Done");
	LOG4CXX_INFO(logger, "Total reads processed: " << rp.get_num_processed_reads());

	kmersAhoCorasick.cleanup();
	output.close();
}

void TaskConfigurator::getKmersAhoCorasick(const Database& data, AhoCorasick& ahoCorasick) {
	LOG4CXX_INFO(logger, "Creating Aho-Corasick automata for kmers... ");

	auto it = data.get_kmer_iterator();
	const int amount = data.get_num_kmers();

	for (int i = 0; i < amount; ++i) {
		ahoCorasick.addString(it->first);
		it++;
	}
	ahoCorasick.init();

	LOG4CXX_INFO(logger, "Done");
}
