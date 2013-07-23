from Bio.SubsMat.MatrixInfo import blosum62

mmatch = {}
for i, j in blosum62.keys():
    mmatch[i, j] = mmatch[j, i] = blosum62[i, j]


def score(seq1, seq2, gap, match, d_old=None, depth=0):
    if not d_old:
        d = [[i*gap for i in range(len(seq1)+1)]]
    else:
        d = [d_old[-1]]
    j = depth
    for s in seq2:
        cur = [(j+1) * gap]
        for i, t in enumerate(seq1):
            cur.append(max(d[-1][i+1] + gap, d[-1][i] + match[s, t], cur[-1] + gap))
        d.append(cur)
        j += 1
    return d if not d_old else d[1:]


def backtrace(seq1, seq2, score_matrix, gap, match):
    i, j = len(seq2), len(seq1)

    res1, res2 = [], []
    while i != 0 or j != 0:
        s, t = seq2[i - 1], seq1[j - 1]
        l = [score_matrix[i - 1][j] + gap,
             score_matrix[i][j - 1] + gap,
             score_matrix[i - 1][j - 1] + match[s, t]]
        score = score_matrix[i][j]
        if score == l[0]:
            i -= 1
            res1.append(s)
            res2.append('-')
        elif score == l[1]:
            j -= 1
            res1.append('-')
            res2.append(t)
        elif score == l[2]:
            i -= 1
            j -= 1
            res1.append(s)
            res2.append(t)
        else:
            assert False
    return "".join(reversed(res1)), "".join(reversed(res2))


class stack(object):
    def __init__(self):
        self.st = []

    def push(self, val):
        self.st.append(val)

    def pop(self):
        return self.st.pop() if not self.empty() else None

    def top(self):
        return self.st[-1] if not self.empty() else None

    def empty(self):
        return len(self.st) == 0

    def data(self):
        return self.st

    def __len__(self):
        return len(self.st)


class alimatrix(object):
    def __init__(self, stack):
        self.data = stack

    def __getitem__(self, item):
        i = item
        for data in self.data.data():
            if len(data) <= i:
                i -= len(data)
            else:
                return data[i]

    def __len__(self):
        return sum(len(i) for i in self.data.data())


class alicont(object):
    def __init__(self, line, gap, match):
        self.line = line
        self.stack = stack()
        self.seqs = stack()
        self.gap = gap
        self.match = match

    def add(self, edge):
        self.stack.push(score(self.line, edge, self.gap, self.match, self.top()[0], self.depth()))
        self.seqs.push(edge)

    def depth(self):
        return sum(len(i) for i in self.stack.data())

    def pop(self):
        return self.stack.pop(), self.seqs.pop()

    def top(self):
        return self.stack.top(), self.seqs.top()

    def score(self):
        top = self.stack.top()
        return [-1][-1] if top else float("NaN")

    def target(self):
        return "".join(self.seqs.data())

    def alignment(self):
        return backtrace(self.line, self.target(), alimatrix(self.stack), self.gap, self.match)


def test():
    a = alicont("MEANLY", -5, mmatch)
    a.add("PLEA")
    a.add("SANT")
    a.add("LY")
    print("%s\n%s" % a.alignment())