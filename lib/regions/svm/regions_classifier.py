import csv
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
        results = svm_tools.markup_sequence_dataset(keys, markup)
        learn_dataset.extend(keys)
        learn_results.extend(results)

    classifier = svm.SVC(kernel='rbf')
    classifier.fit(np.asarray(learn_dataset, float), np.asarray(learn_results, float))
    return classifier


def predict_sequence(classifier, sequence, radius, alphabet):
    dataset = svm_tools.get_sequence_dataset(sequence, radius, alphabet)
    return svm_tools.filter_predicted_result(classifier.predict(dataset))


def predict_dataset(classifier, dataset, radius, alphabet):
    result = []
    for data in dataset.data:
        predict_data = svm_tools.get_sequence_dataset(data.seq, radius, alphabet)
        predicted_result = svm_tools.filter_predicted_result(classifier.predict(predict_data))
        result.append((str(data.seq), predicted_result))
    return result


class Dataset(object):
    def __init__(self, fasta_filename, kabat_filename, alphabet, count, is_random=False):
        self.count = count
        self.alphabet = alphabet
        if kabat_filename is not None:
            self.load_kabat(kabat_filename)
            self.markup = []
        else:
            self.markup = None

        raw_data = svm_tools.get_raw_data(fasta_filename, is_random)

        i = 0
        self.data = []
        for record in raw_data:
            if not self.is_marked() or record.id in self.regions:
                i += 1
                self.data.append(record)
                if self.is_marked():
                    self.markup.append(self.regions[record.id])
                if i >= count:
                    break

    def load_kabat(self, path):
        self.regions = {}
        with open(path, "r") as f:
            for row in csv.reader(f, delimiter="\t"):
                self.regions[row[0]] = map(int, row[1:15])

    def is_marked(self):
        return self.markup is not None


class UsageTests(object):
    alphabet = "ACGT"

    def print_marked_dataset(self, size, is_random):
        dataset = Dataset(os.path.abspath(r"..\..\data\germline\human\VJL_combinations.fasta"),
                          os.path.abspath(r"..\..\data\nomenclature\human\VJL_combinations.kabat"),
                          self.alphabet, size, is_random)
        for data, markup in zip(dataset.data, dataset.markup):
            print data.id, str(data.seq), markup

    def print_raw_dataset(self, size, is_random):
        dataset = Dataset(os.path.abspath(r"..\..\data\germline\human\VJL_combinations.fasta"),
                          None, self.alphabet, size, is_random)
        for data in dataset.data:
            print data.id, str(data.seq)


if __name__ == "__main__":
    test = UsageTests()

    print '-' * 40, "marked random"
    test.print_marked_dataset(size=5, is_random=True)
    print '-' * 40, "marked ordered"
    test.print_marked_dataset(size=5, is_random=False)
    print '-' * 40, "raw random"
    test.print_raw_dataset(size=5, is_random=True)
    print '-' * 40, "raw ordered"
    test.print_raw_dataset(size=5, is_random=False)
