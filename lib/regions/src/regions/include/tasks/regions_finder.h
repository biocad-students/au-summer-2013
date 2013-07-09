#ifndef REGIONS_FINDER_H_
#define REGIONS_FINDER_H_

#include <iostream>
#include "aho_corasick.h"
#include "database.h"

class ReginosFinder {
public:
	ReginosFinder(const Database * data, std::ostream& output,
			const AhoCorasick &dbAhoCorasick, const AhoCorasick &kmersAhoCorasick, int kmer_size)
      : data(data), output(output), dbAhoCorasick(dbAhoCorasick), kmersAhoCorasick(kmersAhoCorasick), kmer_size(kmer_size){};

	void operator()(const Read &r);

private:
  const Database * data;
  std::ostream& output;
  AhoCorasick dbAhoCorasick;
  AhoCorasick kmersAhoCorasick;
  const int kmer_size;
};

#endif /* REGIONS_FINDER_H_ */
