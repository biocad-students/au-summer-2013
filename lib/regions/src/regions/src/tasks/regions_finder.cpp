#include "job_wrappers.hpp"
#include "logger/log_writers.hpp"
#include "adapter_index.hpp"
#include "valid_kmer_generator.hpp"
#include "adapter_index.hpp"

#include "ssw/ssw_cpp.h"

#include <set>

static bool is_alignment_good(const StripedSmithWaterman::Alignment& a, const std::string& sequence, const std::string& query, double aligned_part_fraction) {
  return (std::min(a.query_end - a.query_begin + 1, a.ref_end - a.ref_begin + 1) / (double) query.size() > aligned_part_fraction) &&
		  (a.ref_begin == 0 || a.ref_end == sequence.size() - 1); //check that query adjoins or even overlaps the sequence edge
}

bool ExactAndAlignJobWrapper::operator()(const Read &r) {
  try {
    const std::string& name = r.getName();
    const std::string& sequence = r.getSequenceString();

    // Try to exact match the sequences
    auto matchingSequences = dbAhoCorasick.search(sequence);
    if (!matchingSequences.empty()) {
#     pragma omp critical
      {
        aligned_ += 1;
        print_match(output, bed, matchingSequences, name, sequence, data);
      }

      return false; // Exact match is better than an alignment -> no need to align
    }

    std::set<size_t> setOfContaminations2check;
    ValidKMerGenerator<cclean::K> gen(sequence.c_str(), NULL, sequence.size());
    while (gen.HasMore()) {
      cclean::KMer kmer = gen.kmer();

      auto it = index_.find(kmer);
      if (it != index_.end())
        setOfContaminations2check.insert(it->second.begin(), it->second.end());

      gen.Next();
    }

    // Try to align the artifacts for corresponding kmers
    StripedSmithWaterman::Aligner aligner;
    StripedSmithWaterman::Filter filter;
    aligner.SetReferenceSequence(sequence.c_str(), sequence.size());
    for (auto it = setOfContaminations2check.begin(), et = setOfContaminations2check.end(); it != et; ++it) {
      StripedSmithWaterman::Alignment alignment;
      const std::string& query = index_.seq(*it);
      aligner.Align(query.c_str(), filter, &alignment);

      std::string database_name;
      data->get_name_by_sequence(query, database_name);

      if (alignment.mismatches < mismatch_threshold && is_alignment_good(alignment, sequence, query, aligned_part_fraction)) {
#       pragma omp critical
        {
          aligned_ += 1;
          print_alignment(output, alignment, sequence, query, name, database_name);
          print_bed(bed, name, alignment.ref_begin, alignment.ref_end);
        }
      }
    }

  } catch (std::exception& e) {
    ERROR(e.what());
  }

  return false;
}
