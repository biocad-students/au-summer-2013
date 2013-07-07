#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "config_reader.h"
#include "ssw_cpp.h"

int main() {
	log4cxx::PropertyConfigurator::configure("./config/log4cxx.properties");
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

    LOG4CXX_DEBUG(logger, "Hello World");
	try {
		po::variables_map vm = ConfigReader::read_settings();
		std::cout << "res: " << vm["settings.max_nthreads"].as<std::string>() << std::endl;
	} catch (std::exception & e) {
	    LOG4CXX_DEBUG(logger, e.what());
	}

    LOG4CXX_DEBUG(logger, "Goodbye!");

    return 0;
}
