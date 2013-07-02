__author__ = 'mactep'

from lib.representation.aminocolors import AMINO_COLORS


class RepresentationBar(object):
    def __init__(self, data, count=1):
        self.data = data
        self.count = count


class RepresentationLine(object):
    def __init__(self, name, seq):
        self.name = name
        self.seq = seq
        self.bars = {}

    def add_bar(self, pos, bar):
        if pos not in self.bars:
            self.bars[pos] = [bar]
        else:
            self.bars[pos].append(bar)


class LatexSeqRepresentation(object):
    def __init__(self):
        self.lines = []
        self.color_table = AMINO_COLORS
        self.colors = {}

    def add_line(self, line):
        self.lines.append(line)

    def _generate_color_table(self):
        self.colors = {}
        text = []
        for aa in self.color_table:
            val = self.color_table[aa]
            s = str(val)
            if s not in self.colors:
                i = len(self.colors)
                self.colors[s] = "color%i" % i
                text.append("\\definecolor{color%i}{rgb}{%f, %f, %f}\n" % (i, val[0] / 255,
                                                                           val[1] / 255, val[2] / 255))
        return text

    def _generate_bar_line(self, line_id, bar_n):
        line = []
        seq = self.lines[line_id].seq
        bars = self.lines[line_id].bars
        for i, _ in enumerate(seq):
            if i in bars and len(bars[i]) > bar_n:
                a = bars[i][bar_n]
                line.append("\\cellcolor{%s}%s" % (self.colors[str(self.color_table[a])], a))
            else:
                line.append(" ")
        return " & ".join(line) + "\\\\ \n"

    def _generate_table(self, line_id):
        seq = self.lines[line_id].seq
        bars = self.lines[line_id].bars

        text = ["\section*{%s}\n" % self.lines[line_id].name]
        tstart = "  \\begin{tabular}{%s}\n" % ("c" * len(seq))
        text.append(tstart)

        # Generate mainline
        mainline_text = []
        for i in seq:
            mainline_text.append("\\cellcolor{%s}%s" % (self.colors[str(self.color_table[i])], i))
        text.append(" & ".join(mainline_text) + "\\\\ \n")

        # Generate bars
        try:
            n = max(len(bars[i]) for i in bars)
            print(n)
        except ValueError:
            n = 0
        for i in range(n):
            text.append(self._generate_bar_line(line_id, i))

        tend = "  \\end{tabular}\n"
        text.append(tend)

        return "".join(text)

    def generate_latex(self):
        text = []
        # Generate document header
        head = "\\documentclass[a4paper,10pt,landscape]{article}\n" \
               "\\usepackage[utf8]{inputenc}\n" \
               "\\usepackage[table]{xcolor}\n" \
               "\\usepackage{color, colortbl}\n" \
               "\\usepackage[a4paper, top=10mm, left=10mm, right=10mm, bottom=20mm]{geometry}\n"
        text.append(head)

        # Generating colors
        for i in self._generate_color_table():
            text.append(i)

        # Generating document
        doc_start = "\\begin{document}\n" \
                    " \\begin{center}\n" \
                    "  \\footnotesize\n"

        text.append(doc_start)

        for i, _ in enumerate(self.lines):
            text.append(self._generate_table(i))

        doc_end = " \\end{center}\n" \
                  "\\end{document}\n"
        text.append(doc_end)

        return "".join(text)

    def save_to_file(self, filename):
        text = self.generate_latex()
        with open(filename, "wt") as fd:
            fd.write(text)