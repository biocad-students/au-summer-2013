#ifndef READ_PROCESSOR_H_
#define READ_PROCESSOR_H_

#include "misc/generic_exception.h"

class ReadProcessor {
public:
	template<typename Reader, typename Handler>
	void read_and_process(const Reader& reader, const Handler& handler) const {
		//TODO TBB concurrent_queue here
		throw GenericException("Not implemented yet");
	}
};

#endif /* READ_PROCESSOR_H_ */
