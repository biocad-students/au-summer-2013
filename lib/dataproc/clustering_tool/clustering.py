from sklearn.cluster import spectral_clustering


labels = spectral_clustering(graph, n_clusters=2, eigen_solver='arpack')
