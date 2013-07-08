__author__ = 'Aspart'

from kstat import KStat

class AhoNode(object):
    def __init__(self):
        self.goto = {}
        self.out = []
        self.fail = None

class Trie(object):
    def aho_create_forest(self, patterns):
        self.root = AhoNode()
        self.m_kstat = KStat()
        for path in patterns:
            node = self.root
            for i in range(len(path)):
                node = node.goto.setdefault(path[i], AhoNode())
            node.out.append(path)

    def create(self, patterns):
        # TODO: switch to Ukkonen`s algos
        self.aho_create_forest(patterns)
        queue = []
        for node in self.root.goto.values():
            queue.append(node)
            node.fail = self.root
        while len(queue) > 0:
            rnode = queue.pop(0)
            for key, unode in rnode.goto.items():
                queue.append(unode)
                fnode = rnode.fail
                while fnode and key not in fnode.goto:
                    fnode = fnode.fail
                unode.fail = fnode.goto[key] if fnode else self.root
                unode.out += unode.fail.out

    def find_all(self, s, callback, begin=None):
        if not begin:
            begin = self.root
        for i, _ in enumerate(s):
            while node and s[i] not in node.goto:
                node = node.fail
            if node:
                node = begin
                continue
            node = node.goto[s[i]]
            for pattern in node.out:
                callback(i - len(pattern) + 1, pattern)