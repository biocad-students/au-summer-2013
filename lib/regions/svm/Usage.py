from os import path

import RegionsClassifier as rc
from SvmTools import get_dataset_prediction_rank


if __name__ == "__main__":
    alphabet = "ACGT"
    radius = 10
    count = 100
    dataset = rc.Dataset(path.abspath(r"../../../data/germline/human/VJL_combinations.fasta"),
                         path.abspath(r"../../../data/nomenclature/human/VJL_combinations.kabat"), alphabet, count, True)
    print("Dataset is ready.")
    classifier = rc.learn_svm_classifier(dataset, radius)
    print("SVM is ready.")

    test_count = 20
    dataset = rc.Dataset(path.abspath(r"../../../data/germline/human/VJL_combinations.fasta"),
                         path.abspath(r"../../../data/nomenclature/human/VJL_combinations.kabat"), alphabet, test_count, True)

    result = rc.predict_dataset(classifier, dataset, radius, alphabet)

    #print '-' * 40, "dataset prediction"
    #for line in result:
    #    print line

    seq = dataset.data[0][0].seq

    #print '-' * 40, "sequence prediction"
    #print rc.predict_sequence(classifier, seq, radius, alphabet)

    check_result = get_dataset_prediction_rank(dataset, result)
    #print '-' * 40, "raw check result"
    print check_result
    #print '-' * 40, "check result with sequence id"
    #print zip([data[0].id for data in dataset.data], check_result)
