import csv
from itertools import chain
from os import path
from random import shuffle

from Bio import SeqIO
from sklearn import svm
from sklearn.externals import joblib
import numpy as np


class Regions(object):
    def __init__(self, alphabet, radius=11, dump=None):
        self.regions = {}
        self.init_symbol_vectors(alphabet)
        self.region_names = ["FR1", "CDR1", "FR2", "CDR2", "FR3", "CDR3", "FR4"]
        self.radius = radius
        self.classifier = svm.SVC(kernel='rbf') if dump is None else joblib.load(dump)

    def learn_on_file(self, kabatPath, fastaPath, count):
        self.load_kabat(kabatPath)
        with open(fastaPath, "rU") as handle:
            learn_dataset, learn_results = ([], [])
            i = 0
            for record in SeqIO.parse(handle, "fasta"):
                if not record.id in self.regions:
                    continue
                i += 1
                keys = self.create_dataset(record.seq)
                results = self.markup_dataset(keys, self.regions[record.id])
                learn_dataset.extend(keys)
                learn_results.extend(results)
                if i > count:
                    break
            self.classifier.fit(np.asarray(learn_dataset, float), np.asarray(learn_results, float))

    def dump(self, filename):
        joblib.dump(self.classifier, filename, compress=9)

    def predict(self, gene):
        dataset = self.create_dataset(gene)
        return self.classifier.predict(dataset)

    def create_dataset(self, sequence):
        data = [self.symbol_vectors['-']] * (self.radius + 1)
        seq_len = len(sequence)
        for i in xrange(self.radius):
            data.append(self.symbol_vectors[sequence[i] if i < seq_len else '-'])
        dataset = []
        for i in xrange(seq_len):
            data = data[1:]
            data.append(self.symbol_vectors[sequence[i + self.radius] if i + self.radius < seq_len else '-'])
            dataset.append(list(chain(*data)))
        return dataset

    def markup_dataset(self, dataset, regions):
        return [self.get_region_number(regions, i) for i in xrange(len(dataset))]

    def get_region_number(self, regions, position):
        for i in xrange(len(regions) / 2):
            if regions[i * 2] <= position + 1 <= regions[i * 2 + 1]:
                return i
        raise KeyError("Too large position: {}. Regions end on: {}".format(position, regions[len(regions) - 1]))

    def load_kabat(self, path):
        with open(path, "r") as f:
            for row in csv.reader(f, delimiter="\t"):
                self.regions[row[0]] = map(int, row[1:15])

    def init_symbol_vectors(self, alphabet):
        self.symbol_vectors = {}
        for i, c in enumerate(alphabet):
            self.symbol_vectors[c] = [1 if i == j else 0 for j in xrange(len(alphabet))]
        self.symbol_vectors['-'] = [0] * len(alphabet)
        self.symbol_vectors['N'] = [1] * len(alphabet)
        return self.symbol_vectors

def test_parallel(radius, count):
    alphabet = "ACGT"
    regions = Regions(alphabet, radius)
    regions.learn_on_file(path.abspath(r"..\..\data\nomenclature\human\VJL_combinations.kabat"),
                          path.abspath(r"..\..\data\germline\human\VJL_shuffle.fasta"), count)
    return regions


def test():
    count = 300
    t_list = list(range(1000))
    shuffle(t_list)
    for radius in xrange(13, 60, 2):
        s = test_parallel(radius, count)

#
# if __name__ == "__main__":
#     alphabet = "ACGT"
#     radius = 60
#     dump_filename = "classifierVH.dmp"
#
#     # regions = Regions(alphabet, radius)
#     # regions.learn_on_file(path.abspath(r"..\..\data\nomenclature\human\VJL_combinations.kabat"),
#     #                       path.abspath(r"..\..\data\germline\human\VJL_combinations.fasta"))
#     # print regions.predict("CAGTCTGTGCTGACTCAGCCACCCTCGGTGTCTGAAGCCCCCAGGCAGAGGGTCACCATCTCCTGTTCTGGAAGCAGCTCCAACATCGGAAATAATGCTGTAAACTGGTACCAGCAGCTCCCAGGAAAGGCTCCCAAACTCCTCATCTATTATGATGATCTGCTGCCCTCAGGGGTCTCTGACCGATTCTCTGGCTCCAAGTCTGGCACCTCAGCCTCCCTGGCCATCAGTGGGCTCCAGTCTGAGGATGAGGCTGATTATTACTGTGCAGCATGGGATGACAGCCTGAATGGTCCTGCTGTGTTCGGAGGAGGCACCCAGCTGACCGTCCTCG")
#     # regions.dump(dump_filename)
#
#     result = []
#     regions = Regions(alphabet, radius, dump_filename)
#     print "-" * 50
#     with open(path.abspath("VH_corrected.fasta"), "rU") as handle:
#         for i, record in enumerate(SeqIO.parse(handle, "fasta")):
#             result.append((str(record.seq), regions.predict(record.seq)))
#             if i > 50:
#                 break
#     #print regions.predict("CAGTCTGTGCTGACTCAGCCACCCTCGGTGTCTGAAGCCCCCAGGCAGAGGGTCACCATCTCCTGTTCTGGAAGCAGCTCCAACATCGGAAATAATGCTGTAAACTGGTACCAGCAGCTCCCAGGAAAGGCTCCCAAACTCCTCATCTATTATGATGATCTGCTGCCCTCAGGGGTCTCTGACCGATTCTCTGGCTCCAAGTCTGGCACCTCAGCCTCCCTGGCCATCAGTGGGCTCCAGTCTGAGGATGAGGCTGATTATTACTGTGCAGCATGGGATGACAGCCTGAATGGTCCTGCTGTGTTCGGAGGAGGCACCCAGCTGACCGTCCTCG")
#     for i in result:
#         print i[0]
#         print "".join(str(int(j)) for j in i[1])
#         print "\n"