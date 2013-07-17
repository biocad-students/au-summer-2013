__author__ = 'bondarevts'

import os
import csv
import re

from pyfasta import Fasta


class GeneMarkup(object):
    def __init__(self, path_to_kabat):
        self.kabat = Kabat(path_to_kabat)

    def getVGeneRegions(self, geneName):
        if geneName in self.kabat.genes:
            return self.kabat.genes[geneName]
        self.not_found_in_kabat += 1
        self.not_in_kabat.write(geneName + "\n")

    def findFR4(self, geneName, withoutMarkup):
        # GT[KQ]|WG[KQR]
        group = re.search("GG[ACGT]AC[ACGT][AC]A[AG]|TGGGG[ACTG](AA[AG]|CA[AG]|CG[ACGT]|AG[AG])", withoutMarkup)
        if group is None:
            self.fr4_not_found += 1
            self.without_fr4.write(geneName + "\n")
        return group

    def openOutFiles(self, baseName):
        baseName = os.path.splitext(os.path.basename(baseName))[0]
        self.result_kabat_file = open(baseName + ".kabat", 'w')
        self.not_in_kabat = open(baseName + "_notInKabat.log", 'w')
        self.without_fr4 = open(baseName + "_withoutFR4.log", 'w')

    def closeOutFiles(self):
        self.result_kabat_file.close()
        self.not_in_kabat.close()
        self.without_fr4.close()

    def run(self, filename):
        self.openOutFiles(filename)
        f = Fasta(filename)

        count = len(f)
        self.not_found_in_kabat, self.fr4_not_found, current = (0, 0, 0)

        for name in f.keys():
            current += 1
            if current % 1000 == 0:
                print "All %d. Current: %d" % (count, current)
                # format: vName_jName{frameNumber} or vName_dName{frameNumber}_jName{frameNumber}

            vGeneName = name.split("_")[0]

            vGeneRegions = self.getVGeneRegions(vGeneName)
            if vGeneRegions is None:
                continue

            withoutMarkup = f[name][vGeneRegions[self.kabat.regions_count * 2 - 1]:]
            group = self.findFR4(name, withoutMarkup)
            if group is None:
                continue

            self.result_kabat_file.write(name)
            self.result_kabat_file.write(("\t%d" * 10) % tuple(vGeneRegions))
            self.result_kabat_file.write(("\t%d" * 4 + "\n") % tuple(
                [vGeneRegions[9] + i for i in [1, group.start(), group.start() + 1, len(withoutMarkup)]]))

        self.closeOutFiles()
        print "all: {}; not in kabat: {}; without fr4: {}".format(current, self.not_found_in_kabat, self.fr4_not_found)


class Kabat(object):
    genes = {}

    def __init__(self, path, regions_count=5):
        self.regions_count = regions_count
        with open(path, "r") as f:
            for row in csv.reader(f, delimiter="\t"):
                self.genes[row[0]] = map(int, row[1:regions_count * 2 + 1])


def test():
    markup = GeneMarkup(r"..\..\..\data\nomenclature\human\human.ndm.kabat")
    markup.run("test.fasta")
    return True

if __name__ == "__main__":
    if test():
        markup = GeneMarkup(r"..\..\..\data\nomenclature\human\human.ndm.kabat")
        markup.run(r"..\..\..\data\germline\human\VJL_combinations.fasta")
        markup.run(r"..\..\..\data\germline\human\VJK_combinations.fasta")
        markup.run(r"..\..\..\data\germline\human\VDJH_combinations.fasta")
    else:
        print "test error"