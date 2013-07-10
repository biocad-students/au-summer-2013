#ifndef REGIONS_FINDER_H_
#define REGIONS_FINDER_H_

#include <iostream>
#include "aho_corasick.h"
#include "database.h"

class RegionsFinder {
public:
	RegionsFinder(const Database * data, std::ostream& output,
			const AhoCorasick &kmersAhoCorasick, int kmer_size)
      : data(data), output(output), kmersAhoCorasick(kmersAhoCorasick), kmer_size(kmer_size){};

	void operator()(const Read &r);

private:
  const Database * data;
  std::ostream& output;
  AhoCorasick kmersAhoCorasick;
  const int kmer_size;
};

#endif /* REGIONS_FINDER_H_ */
