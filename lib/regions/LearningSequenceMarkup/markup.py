__author__ = 'bondarevts'

from pyfasta import Fasta
import os
import re


class GeneMarkup(object):
    def __init__(self):
        self.kabat = Kabat(r"..\..\..\data\nomenclature\human\human.ndm.kabat")

    def getVGeneRegions(self, geneName):
        if geneName in self.kabat.genes:
            return self.kabat.genes[geneName]
        self.notFoundInKabatCount += 1
        self.notInKabat.write(geneName + "\n")
        return None

    def findFR4(self, geneName, withoutMarkup):
        group = re.search("GG[ACGT]AC[ACGT][AC]A[AG]", withoutMarkup)  # GT[KQ]
        if group is None:
            group = re.search("TGGGG[ACTG](AA[AG]|CA[AG]|CG[ACGT]|AG[AG])", withoutMarkup)  # WG[KQR]
        if group is None:
            self.FR4NotFoundCount += 1
            self.withoutFR4.write(geneName + "\n")
        return group

    def openOutFiles(self, baseName):
        baseName = os.path.splitext(os.path.basename(baseName))[0]
        self.resultKabatFile = open(baseName + ".kabat", 'w')
        self.notInKabat = open(baseName + "_notInKabat.log", 'w')
        self.withoutFR4 = open(baseName + "_withoutFR4.log", 'w')

    def closeOutFiles(self):
        self.resultKabatFile.close()
        self.notInKabat.close()
        self.withoutFR4.close()

    def run(self, filename):
        self.openOutFiles(filename)
        f = Fasta(filename)

        count = len(f.keys())
        (self.notFoundInKabatCount, self.FR4NotFoundCount, current) = (0, 0, 0)

        for name in f.keys():
            current += 1
            if current % 1000 == 0:
                print "All %d. Current: %d" % (count, current)
                # format: vName_jName{frameNumber} or vName_dName{frameNumber}_jName{frameNumber}

            vGeneName = name.split("_")[0]

            vGeneRegions = self.getVGeneRegions(vGeneName)
            if vGeneRegions is None:
                continue

            withoutMarkup = f[name][vGeneRegions[5]:]
            group = self.findFR4(name, withoutMarkup)
            if group is None:
                continue

            self.resultKabatFile.write(name)
            self.resultKabatFile.write(("\t%d" * 6) % tuple(vGeneRegions))
            self.resultKabatFile.write(("\t%d" * 4 + "\n") % tuple(
                [vGeneRegions[5] + i for i in [1, group.start(), group.start() + 1, len(withoutMarkup)]]))

        self.closeOutFiles()
        print "all: %d; not in kabat: %d; without fr4: %d" % (current, self.notFoundInKabatCount, self.FR4NotFoundCount)


class Kabat(object):
    genes = {}

    def __init__(self, path):
        f = open(path, 'r')
        for line in f:
            parts = line.split('\t')
            self.genes[parts[0]] = [int(s) for s in parts[1:7]]


if __name__ == "__main__":
    markup = GeneMarkup()
    markup.run(r"..\..\..\data\germline\human\VJL_combinations.fasta")
    markup.run(r"..\..\..\data\germline\human\VJK_combinations.fasta")
    markup.run(r"..\..\..\data\germline\human\VDJH_combinations.fasta")