#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "config_reader.h"
#include "ssw_cpp.h"

int main() {
//    StripedSmithWaterman::Aligner aligner;
//    StripedSmithWaterman::Alignment alignment;
//    const StripedSmithWaterman::Filter filter;
//    const std::string sequence = "test";
//    const std::string query = "tst";
//    aligner.SetReferenceSequence(sequence.c_str(), sequence.size());
//    aligner.Align(query.c_str(), filter, &alignment);

	log4cxx::PropertyConfigurator::configure("./config/log4cxx.properties");
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));
    LOG4CXX_DEBUG(logger, "Hello World");

    po::variables_map vm = ConfigReader::read_settings();

    return 0;
}
