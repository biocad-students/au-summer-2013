#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class ConfigReader {
public:
	static po::variables_map read_settings();
};

#endif /* CONFIG_READER_H_ */
