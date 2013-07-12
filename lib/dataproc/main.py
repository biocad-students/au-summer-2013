from collections import defaultdict
import json
import argparse
import os
import sys

from Bio import SeqIO
from Bio.SeqUtils.CheckSum import seguid
from region_finding import find_cdr3
from separate import split


def remove_duplicates(fasta):
    check_sums = set()
    for record in fasta:
        check_sum = seguid(record.seq)
        if check_sum in check_sums:
            print ("Ignoring record {0}".format(record.id))
            continue
        check_sums.add(check_sum)
        yield SeqIO.SeqRecord(id=record.id, description=record.description,
                              seq=record.seq, name=record.name)


def fasta_translation(fasta):
    for record in fasta:
        yield SeqIO.SeqRecord(id=record.id, description=record.description,
                        seq=record.seq.translate(), name=record.name)


def main(args):
    parser = argparse.ArgumentParser(
        description="Clustering sequences by cdr3, cdr2, cdr1")
    parser.add_argument("-i", metavar="fasta_path", required=True,
                        type=argparse.FileType("r"),
                        help="path to FASTA file with reads")
    parser.add_argument("-o", metavar="out_dir", required=True,
                        help="path to file you want to write result")
    parser.add_argument("-c", metavar="config_path", default="config.json",
                        type=argparse.FileType("r"),
                        help="path to config in .json (by "
                             "default is config.json")
    parser.add_argument("--clusters", metavar="clusters_path", nargs=1,
                        type=argparse.FileType("r"),
                        help="path to file with known clusters in .cl, use "
                             "if need add new sequence")
    parser.add_argument("--correction", action="store_true", default=False,
                        help="read correction only")
    parser.add_argument("--clustering", action="store_true", default=False,
                        help="clustering only")
    parser.add_argument("-a", "--add", action="store_true", default=False,
                        help="add new sequences in known clusters")

    args = parser.parse_args(args)
    fasta_file = args.i
    config_file = args.c
    out_dir_path = args.o

    if not os.path.exists(out_dir_path):
        os.makedirs(out_dir_path)

    config = json.load(config_file)

    fasta = SeqIO.parse(fasta_file, "fasta")
    separated_fasta = split(config, fasta)
    for seq_type, records in separated_fasta.iteritems():
        separated_fasta[seq_type] = list(remove_duplicates(records))
        print seq_type, len(separated_fasta[seq_type])

    peptide_fasta = defaultdict(list)
    for seq_type, records in separated_fasta.iteritems():
        file_path = os.path.join(out_dir_path, seq_type + ".fasta")
        with open(file_path, "w") as fout:
            SeqIO.write(records, fout, "fasta")

        peptide_fasta[seq_type] = list(fasta_translation(records))

    cdr3 = defaultdict(list)
    for seq_type in config.iterkeys():
        records = peptide_fasta[seq_type]
        cdr3[seq_type] = list(find_cdr3(records, config[seq_type]["cdr3regex"]))

if __name__ == "__main__":
    main(sys.argv[1:])

