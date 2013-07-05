#ifndef AMINO_ACID_CONVERTER_H_
#define AMINO_ACID_CONVERTER_H_

#include <string>
#include <list>
#include "misc/generic_exception.h"

class AminoAcidConverter {
public:
	static void convert(const std::string& src, std::list<std::string*>& dst) const {
		throw new GenericException("Not Implemented Yet");
	}
};


#endif /* AMINO_ACID_CONVERTER_H_ */
