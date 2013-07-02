__author__ = 'Aspart'

import os
from property import Property
from trie import Trie
import kstat

from Bio import SeqIO

class Gene(object):
    def create(self,path):
        self.property = Property()
        self.property.load(path)
        [self.create_from_file(path, content) for key, value in self.property.data.items()
         for content in value if key == "names"]

    def create_from_file(self, path, filename):
        file = os.path.join(path, filename+'.fasta')
        handle = open(file, "rU")
        patterns = [record.seq for record in SeqIO.parse(handle, "fasta")]
        ## TODO: add kstat handling
        handle.close()
        setattr(self, filename, Trie())
        worktrie = getattr(self, filename)
        worktrie.create(patterns)

    def find(self, seq):
        for key, value in self.property.data.items():
            if key == "names":
                for id, _ in enumerate(value):
                    worktrie = getattr(self, value[id])
                    hash = worktrie.m_kstat.get_hash(seq)
                    return worktrie.m_kstat.hashtable[hash]

