import csv
from os import path
from random import shuffle

import numpy as np
from Bio import SeqIO
from sklearn import svm
from sklearn.externals import joblib

import SvmTools


def learn_svm_classifier(dataset, radius):
    learn_dataset, learn_results = ([], [])
    for record in dataset.data:
        keys = SvmTools.get_sequence_dataset(record[0].seq, radius, dataset.alphabet)
        results = SvmTools.markup_sequence_dataset(keys, record[1])
        learn_dataset.extend(keys)
        learn_results.extend(results)

    classifier = svm.SVC(kernel='rbf')
    classifier.fit(np.asarray(learn_dataset, float), np.asarray(learn_results, float))
    return classifier


def dump_svm_classifier(classifier, dump_filename):
    joblib.dump(classifier, dump_filename, compress=9)


def load_svm_classifier(dump_filename):
    return joblib.load(dump_filename)


def predict_sequence(classifier, sequence, radius, alphabet):
    dataset = SvmTools.get_sequence_dataset(sequence, radius, alphabet)
    return SvmTools.filter_predicted_result(classifier.predict(dataset))


def predict_dataset(classifier, dataset, radius, alphabet):
    result = []
    for data in dataset.data:
        predict_data = SvmTools.get_sequence_dataset(data[0].seq, radius, alphabet)
        predicted_result = SvmTools.filter_predicted_result(classifier.predict(predict_data))
        result.append((str(data[0].seq), predicted_result))
    return result


class Dataset:
    def __init__(self, fasta_filename, kabat_filename, alphabet, count, is_random=False):
        self.is_marked = not kabat_filename is None
        self.count = count
        self.alphabet = alphabet
        if self.is_marked:
            self.load_kabat(kabat_filename)

        raw_data = self.get_raw_data(fasta_filename, is_random)

        i = 0
        self.data = []
        for record in raw_data:
            if not self.is_marked or record.id in self.regions:
                i += 1
                if self.is_marked:
                    self.data.append((record, self.regions[record.id]))
                else:
                    self.data.append((record,))
                if i >= count:
                    break

    def load_kabat(self, path):
        self.regions = {}
        with open(path, "r") as f:
            for row in csv.reader(f, delimiter="\t"):
                self.regions[row[0]] = map(int, row[1:15])

    def get_raw_data(self, fasta_filename, random):
        with open(fasta_filename, "rU") as handle:
            raw_data = list(SeqIO.parse(handle, "fasta"))
            if random:
                shuffle(raw_data)
        return raw_data


class UsageTests(object):
    alphabet = "ACGT"

    def print_marked_dataset(self, size, is_random):
        dataset = Dataset(path.abspath(r"..\..\data\germline\human\VJL_combinations.fasta"),
                          path.abspath(r"..\..\data\nomenclature\human\VJL_combinations.kabat"),
                          self.alphabet, size, is_random)
        for data in dataset.data:
            print data[0].id, str(data[0].seq), data[1]

    def print_raw_dataset(self, size, is_random):
        dataset = Dataset(path.abspath(r"..\..\data\germline\human\VJL_combinations.fasta"),
                          None, self.alphabet, size, is_random)
        for data in dataset.data:
            print data[0].id, str(data[0].seq)


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
