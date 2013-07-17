import csv
from itertools import chain

from Bio import SeqIO
from sklearn import svm
import numpy as np

__author__ = 'bondarevts'


class Regions(object):
    def __init__(self, alphabet, radius=11):
        self.regions = {}
        self.init_symbol_vectors(alphabet)
        self.region_names = ["FR1", "CDR1", "FR2", "CDR2", "FR3", "CDR3", "FR4"]
        self.radius = radius
        self.classifier = svm.SVC(kernel='rbf')

    def learn_on_file(self, kabatPath, fastaPath):
        self.load_kabat(kabatPath)
        with open(fastaPath, "rU") as handle:
            i = 0
            for record in SeqIO.parse(handle, "fasta"):
                i += 1
                keys = self.create_dataset(record.seq)
                if not record.id in self.regions:
                    continue
                results = self.markup_dataset(keys, self.regions[record.id])
                self.classifier.fit(np.asarray(keys, float), np.asarray(results, float))
                if i % 1000 == 0:
                    print i

    def predict(self, gene):
        dataset = self.create_dataset(gene)
        return self.classifier.predict(dataset)

    def create_dataset(self, sequence):
        data = [self.symbol_vectors['-']] * (self.radius + 1)
        length = len(sequence)
        for i in range(self.radius):
            data.append(self.symbol_vectors[sequence[i] if i < length else '-'])
        dataset = []
        for i in range(length):
            data = data[1:]
            data.append(self.symbol_vectors[sequence[i + self.radius] if i + self.radius < length else '-'])
            dataset.append(list(chain(*data)))
        return dataset

    def markup_dataset(self, dataset, regions):
        results = []
        for i in range(len(dataset)):
            number = self.get_region_number(regions, i)
            results.append(number)
        return results

    def get_region_number(self, regions, position):
        for i in range(len(regions) / 2):
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
            self.symbol_vectors[c] = [1 if i == j else 0 for j in range(len(alphabet))]
        self.symbol_vectors['-'] = [0] * len(alphabet)
        return self.symbol_vectors


if __name__ == "__main__":
    regions = Regions("ACGTN", 22)
    regions.learn_on_file("LearningSequenceMarkup\VJL_combinations.kabat",
                          r"..\..\data\germline\human\VJL_combinations.fasta")
    print regions.predict("CAGTCTGTGCTGACTCAGCCACCCTCGGTGTCTGAAGCCCCCAGGCAGAGGGTCACCATCTCCTGTTCTGGAAGCAGCTCCAACATCGGAAATAATGCTGTAAACTGGTACCAGCAGCTCCCAGGAAAGGCTCCCAAACTCCTCATCTATTATGATGATCTGCTGCCCTCAGGGGTCTCTGACCGATTCTCTGGCTCCAAGTCTGGCACCTCAGCCTCCCTGGCCATCAGTGGGCTCCAGTCTGAGGATGAGGCTGATTATTACTGTGCAGCATGGGATGACAGCCTGAATGGTCCTGCTGTGTTCGGAGGAGGCACCCAGCTGACCGTCCTCG")
