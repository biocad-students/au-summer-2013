import argparse
import logging
import os
import sys
import pylab

from Bio import AlignIO, SeqIO, Phylo
from Bio.Align.Applications import ClustalwCommandline
from Bio.SeqUtils.CheckSum import seguid


def remove_duplicates(fasta):
    check_sums = set()
    for record in fasta:
        check_sum = seguid(record.seq)
        if check_sum in check_sums:
            print ("Ignoring record {0}".format(record.id))
            continue
        check_sums.add(check_sum)
        yield SeqIO.SeqRecord(id="ID=" + record.id,
                              description=record.description,
                              seq=record.seq, name=record.name)


def main(args):
    logging.basicConfig(filename='example.log', level=logging.DEBUG)

    parser = argparse.ArgumentParser(description="Sequence analysis after "
                                                 "processing")
    parser.add_argument("-i", metavar="fasta_file",
                        help="path to fasta file")
    parser.add_argument("-o", metavar="out_dir",
                        help="directory for results files")

    args = parser.parse_args(args)
    raw_fasta_path = args.i
    out_dir_path = args.o

    if not os.path.exists(out_dir_path):
        os.makedirs(out_dir_path)

    deduplicated_fasta = remove_duplicates(
        SeqIO.parse(raw_fasta_path, "fasta"))
    base = os.path.basename(raw_fasta_path)
    fasta_path = os.path.join(out_dir_path, base)
    SeqIO.write(deduplicated_fasta, fasta_path, "fasta")


    # Multiple sequence alignment
    cline = ClustalwCommandline("clustalw2", infile=fasta_path)
    stdout, stderr = cline()
    logging.info(cline)

    clustalw_result_path = fasta_path.replace(".fasta", ".aln")
    alignment = AlignIO.read(clustalw_result_path, "clustal")
    with open(os.path.join(out_dir_path, "alignment.txt"), "w") as fout:
        fout.write("\n".join([str(record.seq) for record in alignment]))

    # Tree drawing
    tree_path = fasta_path.replace(".fasta", ".dnd")
    tree = Phylo.read(tree_path, "newick")
    tree.ladderize()

    # with labels
    Phylo.draw_graphviz(tree, label_func=lambda n: n.name.replace("ID=", ""),
                        prog="neato")
    pylab.savefig(os.path.join(out_dir_path, "fig_with_labels.pdf")) # need pygraphviz, pylab


if __name__ == "__main__":
    main(sys.argv[1:])