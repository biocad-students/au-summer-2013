__author__ = 'Aspart'

class KStat:
    def __init__(self, k=7):
        self.hashtable = [None for _ in range(pow(4,k)+1)]


    def get_hash(self, seq):
        hash = 0
        p=0
        for i in range(len(seq)):
            if seq[i]=='A':
                p=0
            elif seq[i]=='C':
                p=1
            elif seq[i]=='G':
                p=2
            elif seq[i]=='T':
                p=3
            ## TODO: handle 'N' nucleotide
            #elif seq[i]=='N':
            #    p=0
            hash += pow(4,i)*p
        return hash

    def add(self, seq, pos, node, k=7):
        if pos+k > len(seq):
            return
        hash = self.get_hash(seq[pos:pos+k])
        self.hashtable[hash] = node
