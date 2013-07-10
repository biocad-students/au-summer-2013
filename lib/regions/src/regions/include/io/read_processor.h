#ifndef READ_PROCESSOR_H_
#define READ_PROCESSOR_H_

#include <iostream>
#include <fstream>
#include "generic_exception.h"

class ReadProcessor {
public:
	ReadProcessor(): read(0) {};

	template<typename Reader, typename Handler>
	void readAndProcess(Reader& reader, Handler& handler) {
		//TODO TBB concurrent_queue here
		if(reader.is_open()) {
			while (!reader.eof()) {
				Read r;
				reader >> r;
				handler(r);

				if (++read % 10000 == 0) {
				  std::clog << read << " reads processed\r";
				}
			}
		}
	}

	int get_num_processed_reads() const {
		return read;
	}

private:
	int read;
};

#endif /* READ_PROCESSOR_H_ */
