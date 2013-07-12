from sklearn.ensemble.forest import MAX_INT


def distance(record, cluster):
    return 0


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
