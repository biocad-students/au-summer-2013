__author__ = 'Aspart'

import os
import property
import trie
import kstat
from Bio import SeqIO

class Gene:
    def create(self,path):
        self.property = property.Property()
        self.property.load(path)
        for key, value in self.property.data.items():
            if key == "names":
                for id in range(len(self.property.data[key])):
                    self.create_from_file(path, self.property.data[key][id])

    def create_from_file(self, path, filename):
        file = os.path.join(path, filename+'.fasta')
        handle = open(file, "rU")
        patterns = []
        ## TODO: add kstat handling
        for record in SeqIO.parse(handle, "fasta") :
            patterns.append(record.seq)
        handle.close()

        setattr(self, filename, trie.Trie())
        worktrie = getattr(self, filename)
        worktrie.create(patterns)

    def find(self, seq):
        for key, value in self.property.data.items():
            if key == "names":
                for id in range(len(value)):
                    worktrie = getattr(self, value[id])
                    hash = worktrie.m_kstat.get_hash(seq)
                    return worktrie.m_kstat.hashtable[hash]

