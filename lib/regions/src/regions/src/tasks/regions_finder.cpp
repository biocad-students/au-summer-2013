#include "ssw_cpp.h"
#include "output_formatter.h"

void ReginosFinder::operator()(const Read &r) {
  try {
    const std::string& name = r.getName();
    const std::string& sequence = r.getSeq();

    // Try to exact match the sequences
    auto matchingSequences = dbAhoCorasick.search(sequence);
    if (!matchingSequences.empty()) {
#     pragma omp critical
      {
        OutputFormatter::print_match(output, matchingSequences, name, sequence, data);
      }
    }

    std::set<size_t> setOfReferences2check;
    std::vector<std::string> kmers = KmerGenerator::getKmers(r, kmer_size);

	std::set<std::string *, Compare> sequences;
    for (auto it = kmers.begin(); it != kmers.end(); ++it) {
    	sequences.clear();
    	data->get_sequences_for_kmer(*it, sequences);
        setOfReferences2check.insert(sequences.begin(), sequences.end());
    }

    // Try to align the artifacts for corresponding kmers
    StripedSmithWaterman::Aligner aligner;
    StripedSmithWaterman::Filter filter;
    aligner.SetReferenceSequence(sequence.c_str(), sequence.size());

    for (auto it = setOfReferences2check.begin(), et = setOfReferences2check.end(); it != et; ++it) {
      StripedSmithWaterman::Alignment alignment;
      const std::string& query = *it;
      aligner.Align(query.c_str(), filter, &alignment);

      std::string database_name;
      data->get_name_by_sequence(query, database_name);

#pragma omp critical
        {
    	  OutputFormatter::print_alignment(output, alignment, sequence, query, name, database_name);
        }
    }

  } catch (std::exception& e) {
	  LOG4CXX_DEBUG(logger, "error for read " << r.getSeq() << ":" << e.what());
  }
}
