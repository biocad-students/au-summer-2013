from itertools import chain


def init_symbol_vectors(alphabet):
    symbol_vectors = {
        '-': [0] * len(alphabet),
        'N': [1] * len(alphabet)
    }
    for i, c in enumerate(alphabet):
        symbol_vectors[c] = [1 if i == j else 0 for j in xrange(len(alphabet))]
    return symbol_vectors


def get_sequence_dataset(sequence, radius, alphabet):
    symbol_vectors = init_symbol_vectors(alphabet)
    data = [symbol_vectors['-']] * (radius + 1)
    seq_len = len(sequence)
    for i in xrange(radius):
        data.append(symbol_vectors[sequence[i] if i < seq_len else '-'])
    dataset = []
    for i in xrange(seq_len):
        data = data[1:]
        data.append(symbol_vectors[sequence[i + radius] if i + radius < seq_len else '-'])
        dataset.append(list(chain(*data)))
    return dataset


def get_region_number(regions, position):
    for i in xrange(len(regions) / 2):
        if regions[i * 2] <= position + 1 <= regions[i * 2 + 1]:
            return i
    raise KeyError("Too large position: {}. Regions end on: {}".format(position, regions[len(regions) - 1]))


def markup_sequence_dataset(sequence_dataset, regions):
    return [get_region_number(regions, i) for i in xrange(len(sequence_dataset))]


def filter_predicted_result(result):
    return [int(i) for i in result]


def get_line_prediction_rank(regions, prediction):
    success = 0
    for i, prediction in enumerate(prediction):
        if get_region_number(regions, i) == prediction:
            success += 1
    return 1.0 * success / len(prediction)


def get_dataset_prediction_rank(dataset, prediction):
    lines = zip([dataset.data[i][1] for i in xrange(len(dataset.data))], [p[1] for p in prediction])
    return [get_line_prediction_rank(line[0], line[1]) for line in lines]