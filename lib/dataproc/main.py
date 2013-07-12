import json
import argparse
import os
import sys

from Bio import SeqIO
from Bio.SeqUtils.CheckSum import seguid

from separate import split


def remove_duplicates(fasta):
    check_sums = set()
    for record in fasta:
        checksum = seguid(record.seq)
        if checksum in check_sums:
            print ("Ignoring record {0}".format(record.id))
            continue
        check_sums.add(checksum)
        yield record


def fasta_translation(fasta):
    for record in fasta:
        record.seq = record.seq.translate()
        yield record


def main(args):
    # Parse parameters
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

    # Read config (by default in config.json)
    config = json.load(config_file)

    # Read and process fasta
    fasta = SeqIO.parse(fasta_file, "fasta")
    separated_fasta = split(config, fasta)

    # Write record groups in files
    for seq_type, records in separated_fasta.iteritems():
        file_path = os.path.join(out_dir_path, seq_type + ".fasta")
        with open(file_path, "w") as fout:
            SeqIO.write(records, fout, "fasta")

    


if __name__ == "__main__":
    main(sys.argv[1:])

