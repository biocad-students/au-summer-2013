Installation
=============

    pip install -r requirement.txt

Usage
======

    analysis.py [-h] out_dir fasta_file

    Sequence analysis after processing

    positional arguments:
      out_dir     directory for result files
      fasta_file  path to fasta file with peptide sequences

    optional arguments:
      -h, --help  show this help message and exit

Results
========

out_dir contains
* clustalw alignment out files .aln and .dnd
* alignment.txt with one alignment per line
* figure_with_labels.pdf is an alignment tree given from .dnd file
* .fasta file with unique sequences from fasta_file
* clusters is directory with clusters and meta information about
* dendogram.png for clustering


out_dir/clusters contains
* .fasta files with one cluster per file
* clusters_meta.txt with some meta information about clusters: cluster ID, size,
consensus, PSSM, letter frequencies in consensus
* .png files for letter frequencies in consensus, one cluster per figure
