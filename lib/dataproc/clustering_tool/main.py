from collections import defaultdict
import json
import argparse
import os
import sys

from Bio import SeqIO
from Bio.SeqUtils.CheckSum import seguid
from clustering_tool.region_finding import find_cdr3
from clustering_tool.separate import split


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


def cluster_reading(clusters_path):
    result = {}
    for seq_type in ["VH", "VHH", "VL", "VK"]:
        result[seq_type] = defaultdict(list)
        clusters_dir = os.path.join(clusters_path, seq_type)
        if not os.path.exists(clusters_dir):
            print "Directory {0} does not exist". \
                format(os.path.abspath(clusters_dir))
            continue
        for cluster_filename in os.listdir(clusters_dir):
            cluster_id, _ = \
                os.path.splitext(os.path.basename(cluster_filename))
            cluster_path = os.path.join(clusters_dir, cluster_filename)
            result[seq_type][cluster_id] = \
                list(SeqIO.parse(cluster_path, "fasta"))
    return result


def prepare(fasta, config, out_dir_path):
    separated_fasta = split(config, fasta)
    for seq_type, records in separated_fasta.iteritems():
        separated_fasta[seq_type] = list(remove_duplicates(records))

    peptide_fasta = defaultdict(list)
    for seq_type, records in separated_fasta.iteritems():
        file_path = os.path.join(out_dir_path, seq_type + ".fasta")
        SeqIO.write(records, file_path, "fasta")

        peptide_fasta[seq_type] = list(fasta_translation(records))

    return peptide_fasta


def main(args):
    parser = argparse.ArgumentParser(
        description="Clustering sequences by cdr3, cdr2, cdr1")
    parser.add_argument("-i", metavar="fasta_path", required=True,
                        type=argparse.FileType("r"),
                        help="path to FASTA file with reads")
    parser.add_argument("-p", action="store_true", default=False,
                        help="if your fasta is in peptide alphabet")
    parser.add_argument("-o", metavar="out_dir", required=True,
                        help="path to file you want to write result")
    parser.add_argument("-c", metavar="config_path", default="config.json",
                        type=argparse.FileType("r"),
                        help="path to config in .json (by "
                             "default is config.json")
    parser.add_argument("--clusters", metavar="clusters_path",
                        help="path to directory with known clusters in "
                             "grouped by sequences type .fasta files,"
                             "use if need add new sequence")
    parser.add_argument("--correction", action="store_true", default=False,
                        help="read correction only")
    parser.add_argument("--clustering", action="store_true", default=False,
                        help="clustering only")
    parser.add_argument("-a", "--add", action="store_true", default=False,
                        help="add new sequences in known clusters")

    args = parser.parse_args(args)
    fasta_file = args.i
    is_peptide = args.p
    config_file = args.c
    out_dir_path = args.o

    if not os.path.exists(out_dir_path):
        os.makedirs(out_dir_path)

    config = json.load(config_file)

    peptide_fasta = defaultdict(list)
    if is_peptide:
        fasta = list(SeqIO.parse(fasta_file, "fasta"))
        for seq_type in ["VH", "VHH", "VK", "VL"]:
            peptide_fasta[seq_type] = fasta[:]
    else:
        fasta = SeqIO.parse(fasta_file, "fasta")
        peptide_fasta = prepare(fasta, config, out_dir_path)

    cdr3 = defaultdict(list)
    for seq_type in config.iterkeys():
        records = peptide_fasta[seq_type]
        cdr3[seq_type] = list(
            find_cdr3(records, config[seq_type]["cdr3regex"]))

#        plot_path = plot_cluster_sizes(clusters)


if __name__ == "__main__":
    main(sys.argv[1:])

