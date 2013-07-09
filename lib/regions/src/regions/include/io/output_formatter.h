#ifndef OUTPUT_FORMATTER_H_
#define OUTPUT_FORMATTER_H_

#include <string>
#include <iostream>
#include "ssw_cpp.h"

class OutputFormatter {
public:
	static void print_alignment(std::ostream& output, const StripedSmithWaterman::Alignment& a,
			const std::string& ref, const std::string& query, const std::string& ref_name,
			const std::string& query_name);
private:
	static void print_n_times(std::ostream& output, char c, int n);
	static void restoreFromCigar(const std::string& ref, const std::string& query, std::string& out_ref,
			std::string& out_query, const StripedSmithWaterman::Alignment& a);
};


#endif /* OUTPUT_FORMATTER_H_ */
