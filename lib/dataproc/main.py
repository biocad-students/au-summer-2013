import json
import argparse

from Bio import SeqIO
import sys


def main(args):
    # Parse parameters
    parser = argparse.ArgumentParser(
        description="Clustering sequences by cdr3, cdr2, cdr1")
    parser.add_argument("-i", metavar="fasta_path", nargs=1, required=True,
                        type=argparse.FileType("r"),
                        help="path to FASTA file with reads")
    parser.add_argument("-o", metavar="out_dir", nargs=1, required=True,
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

    # Read config (by default in config.json)
    config = json.load(config_file)

    # Read and process fasta
    fasta = SeqIO.parse(fasta_file, "fasta")


if __name__ == "__main__":
    main(sys.argv[1:])

