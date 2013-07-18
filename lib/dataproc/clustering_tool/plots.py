from matplotlib import pyplot as plt


def plot_cluster_sizes(clusters):
    chunks = [map(len, clusters[seq_type].itervalues())
              for seq_type in clusters]

    plt.boxplot(chunks)

    plt.grid()
    plt.xticks(range(len(clusters) + 1), [""] + list(clusters))
    plt.legend(loc="best")

    plot_path = "cluster_size.png"
    plt.savefig(plot_path)
    return plot_path
