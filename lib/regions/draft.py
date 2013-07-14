num2regions = ["FR1", "CDR1", "FR2", "CDR2", "FR3", "CDR3", "FR4"]

from sklearn import svm
import numpy as np
from itertools import chain

def getAminoMatrix(alphabet):
    aminoMatrices = {}
    for i, c in enumerate(alphabet):
        values = []
        for j in range(len(alphabet)):
            if i == j:
                values.append(1)
            else:
                values.append(0)
        aminoMatrices[c] = values
    aminoMatrices['-'] = [0] * len(alphabet)
    return aminoMatrices


def cleanLearningSequences(sequence, markup):
    if len(markup) != len(sequence):
        print "error: length is not equals"
        return 0
    resultMarkup = []
    resultSequence = []
    for i in range(len(sequence)):
        if sequence[i] != ' ' and sequence[i] != '-':
            resultSequence.append(sequence[i])
            resultMarkup.append(markup[i])

    return resultSequence, resultMarkup


def getLearnSequences(sequence, markup, radius, values):
    s = [values['-']] * (radius + 1)
    length = len(sequence)
    for i in range(radius):
        if i < length:
            s.append(values[sequence[i]])
        else:
            s.append(values['-'])
    X = []
    y = []
    for i in range(length):
        s = s[1:]
        if i + radius < length:
            s.append(values[sequence[i + radius]])
        else:
            s.append(values['-'])
        X.append(s)
        y.append(markup[i])
        print markup[i], s
    return X, y

alphabet = "ACDEFGHIKLMNPQRSTVWY"
matrix = getAminoMatrix(alphabet)

for a, value in matrix.items():
    print a, value

result   = "000000000000000000000000000000  1111111  22222222222222  3333333333333333333  444444444444444444444444444  55555"
sequence = "QVQLVQSGAEVKKPGASVKVSCKASGYTFT  G--YYMH  WVRQAPGQGLEWMG  WINP--NSGGTNYAQKFQG  RVTMTRDTSISTAYMELSRLRSDDTAV  YYCAR"

a, b = cleanLearningSequences(sequence, result)
print a
print b
print

classifier = svm.SVC(kernel='rbf')

X, y = getLearnSequences(a, b, 3, matrix)
# classifier.fit(np.asarray(list(chain(*X))), np.asarray(y))