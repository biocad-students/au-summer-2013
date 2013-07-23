import os.path

import numpy as np
from sklearn import svm

import svm_tools


def learn_svm_classifier(dataset, radius):
    if not dataset.is_marked():
        return None

    learn_dataset, learn_results = [], []
    for record, markup in zip(dataset.data, dataset.markup):
        keys = svm_tools.get_sequence_dataset(record.seq, radius, dataset.alphabet)
        results = [markup] * len(keys)
        learn_dataset.extend(keys)
        learn_results.extend(results)

    classifier = svm.SVC(kernel='rbf')
    classifier.fit(np.asarray(learn_dataset, float), np.asarray(learn_results, float))
    return classifier


class Dataset(object):
    type_to_number = {"VL": 0, "VK": 1, "VH": 2, "VHH": 3}

    def __init__(self, filename_list, gene_type_list, alphabet, count_per_file, is_random):
        self.alphabet = alphabet
        self.data = []
        self.markup = None if gene_type_list is None else []
        for filename, gene_type in zip(filename_list, gene_type_list):
            raw_data = svm_tools.get_raw_data(filename, is_random)
            self.data.extend(raw_data[:count_per_file])
            if self.is_marked():
                self.markup.extend([self.type_to_number[gene_type]] * count_per_file)

    def is_marked(self):
        return self.markup is not None

if __name__ == "__main__":
    dataset = Dataset([os.path.abspath(r"..\..\..\data\germline\human\VJL_combinations.fasta"),
                       os.path.abspath(r"..\..\..\data\germline\human\VJK_combinations.fasta"),
                       os.path.abspath(r"..\..\..\data\germline\human\VDJH_combinations.fasta")],
                      ["VL", "VK", "VH"], "ACGT", 200, True)
    classifier = learn_svm_classifier(dataset, 20)
    svm_tools.dump_svm_classifier(classifier, "type_classifier.dmp")