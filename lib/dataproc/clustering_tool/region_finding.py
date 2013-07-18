import re
from Bio import SeqIO


def find_cdr3(fasta, regex):
    cdr3_pattern = re.compile(regex)
    for record in fasta:
        str_seq = str(record.seq)
        cdr3 = cdr3_pattern.search(str_seq)
        if cdr3:
            seq = str_seq[cdr3.start():cdr3.end()]
            yield SeqIO.SeqRecord(id="cdr3" + record.id,
                                  description=record.description,
                                  seq=seq, name=record.name)

