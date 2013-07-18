import os
import re
from cStringIO import StringIO

from Bio import SeqIO


def parse_cl(f):
    seqs = {}

    for match in re.finditer("=(?P<name>\S+)(?P<chunk>[^=]+)", f.read()):
        name, chunk = match.groups()
        seqs[name] = list(SeqIO.parse(StringIO(chunk), "fasta"))

    return seqs

cluster_dir = "../../../clusters"
for type in ["VH", "VL", "VK"]:
    file_path = os.path.join(cluster_dir, type + "/" + type + "_cdr_corrected.cl")
    f = open(file_path)
    for name, seqs in parse_cl(f).iteritems():
        out_path = cluster_dir + "/" + type + "/" + name + ".fasta"
        SeqIO.write(seqs, out_path, "fasta")