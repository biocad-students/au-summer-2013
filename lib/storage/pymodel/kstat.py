__author__ = 'Aspart'

class KStat(object):
    nucleo = "ACGTN"

    def __init__(self, k=7):
        self.hashtable = [None for _ in range(pow(len(self.nucleo),k)+1)]

    def get_hash(self, seq):
        hash = sum(pow(len(self.nucleo), i) * self.nucleo.index(seq[i]) for i, _ in enumerate(seq))
        return hash

    def add(self, seq, pos, node, k=7):
        if pos+k > len(seq):
            return
        hash = self.get_hash(seq[pos:pos+k])
        self.hashtable[hash] = node
