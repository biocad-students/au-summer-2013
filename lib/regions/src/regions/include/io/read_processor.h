#ifndef READ_PROCESSOR_H_
#define READ_PROCESSOR_H_

#include "generic_exception.h"

class ReadProcessor {
public:
	template<typename Reader, typename Handler>
	void readAndProcess(Reader& reader, Handler& handler) const {

		//TODO TBB concurrent_queue here
		if(reader.is_opened()) {
			while (!reader.is_eof()) {
				Read r;
				reader >> r;
				handler(r);
			}
		}
	}
};

#endif /* READ_PROCESSOR_H_ */
