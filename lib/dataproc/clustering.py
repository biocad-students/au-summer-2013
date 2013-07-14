from collections import defaultdict
from Bio.Seq import Seq
from sklearn.ensemble.forest import MAX_INT


def can_add_to_cluster(cluster, record):
    consensus = cluster[0].description.split()[1]
    consensus = Seq(consensus).translate()
    return record.seq.find(str(consensus))


def add_to_cluster(clusters, fasta):
    seq_types = clusters.iterkeys()
    results = defaultdict(list)
    for seq_type in seq_types:
        results[seq_type] = defaultdict(list)
        if not fasta[seq_type]:
            continue
        for record in fasta[seq_type]:
            result_id, result_len = None, 0
            for cluster_id, cluster in clusters[seq_type].iteritems():
                if can_add_to_cluster(cluster, record):
                    current_len = len(cluster)
                    if current_len > result_len:
                        cluster.append(record)
                        result_len = current_len
                        result_id = cluster_id
            if result_id:
                clusters[seq_type][result_id].append(record)
            results[seq_type][record.id] = (seq_type, result_id, result_len)
    return results


def add_to_cdr3_cluster(clusters, fasta, cdr3):
    seq_types = clusters.iterkeys()
    for seq_type in seq_types:
        if fasta[seq_type]:
            for record in fasta[seq_type]:
                distance, nearest_cluster_id = MAX_INT, ""
                for cluster_id, cluster in clusters[seq_type].iteritems():
                    current_distance = distance(record, cluster)
                    if current_distance < distance:
                        distance = current_distance
                        nearest_cluster_id = cluster_id
                clusters[seq_type][nearest_cluster_id].append(record)
                print seq_type, record.id, nearest_cluster_id, \
                    clusters[seq_type][nearest_cluster_id]
