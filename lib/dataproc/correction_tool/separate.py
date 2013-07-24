import re
from collections import namedtuple, defaultdict

import alignment as aln

Pair = namedtuple("Pair", ["forward", "reverse"])

MATCH = 1
MISSMATCH = -1
GAP_OPEN = -1
GAP_EXT = -1
aligner = aln.Aligner(MATCH, MISSMATCH, GAP_OPEN, GAP_EXT)


def local_alignment(seq1, seq2):
    return aligner.align(seq1, seq2, True)


def with_sequence(mids, primers, seq):
    FWD_MISSMATCH = 6
    REV_MISSMATCH = 10

    reverse_seq = seq.reverse_complement()

    for forward_direction, opposite_direction in [(0, 1), (1, 0)]:
        #speeding up alignments
        start_flank = 2 * (len(mids[forward_direction]) +
                           len(primers[forward_direction]))
        end_flank = 4 * (len(mids[opposite_direction]) +
                         len(primers[opposite_direction]))

        start_seq = mids[forward_direction] + primers[forward_direction]
        start_aln_a, start_aln_b, start_aln_score = local_alignment(
            str(seq)[:start_flank], start_seq)

        if start_aln_score < len(start_seq) - FWD_MISSMATCH:
            continue

        end_seq = mids[opposite_direction] + primers[opposite_direction]
        end_aln_a, end_aln_b, end_aln_score = local_alignment(
            str(reverse_seq)[:end_flank], end_seq)

        if end_aln_score < len(end_seq) - REV_MISSMATCH:
            return None, None   # incompleted

        start = len(start_aln_b.rstrip("-"))
        end = len(end_aln_b.rstrip("-"))

        res = seq[start:-end]
        if forward_direction == 0:
            return res, True
        else:
            return res.reverse_complement(), True

    return None, False  # hasn't mid


def split(config, fasta):
    mids, primers = {}, {}
    seq_types = ["VH", "VK", "VL"]

    splited_fasta = defaultdict(list)

    for seq_type in seq_types:
        type_config = config[seq_type]
        forwardMid = type_config["forwardMid"]
        reverseMid = type_config["reverseMid"]
        forwardPrimer = type_config["forwardPrimer"]
        reversePrimer = type_config["reversePrimer"]
        mids[seq_type] = Pair(forwardMid, reverseMid)
        primers[seq_type] = Pair(forwardPrimer, reversePrimer)

    for record in fasta:
        seq = record.seq.strip("acgtn")
        if "N" in seq:
            splited_fasta["filtered"].append(record)

        has_match = False
        for seq_type in seq_types:
            res_seq, status = with_sequence(mids[seq_type],
                                            primers[seq_type], seq)
            if status is True:
                record.seq = res_seq
                splited_fasta[seq_type].append(record)
                has_match = True
                break
            elif status is None:
                splited_fasta[seq_type + "_incomplete"].append(record)
                has_match = True
                break

        if not has_match:
            splited_fasta["filtered"].append(record)

    vhh = re.compile(config["VHH"]["regex"])
    for record in splited_fasta["VH"]:
        seq = record.seq
        if vhh.search(str(seq.translate())) or \
                vhh.search(str(seq[1:].translate())) or \
                vhh.search(str(seq[2:].translate())):
            splited_fasta["VHH"].append(record)
            splited_fasta["VH"].remove(record)

    return splited_fasta
