__author__ = 'Aspart'

import kstat

class AhoNode:
    def __init__(self):
        self.goto = {}
        self.out = []
        self.fail = None

class Trie:
    def aho_create_forest(self, patterns):
        self.root = AhoNode()
        self.m_kstat = kstat.KStat()
        for path in patterns:
            node = self.root
            for i in range(len(path)):
                node = node.goto.setdefault(path[i], AhoNode())
                self.m_kstat.add(path, i, node)
            node.out.append(path)


    def create(self, patterns):
        # TODO: switch to Ukkonen`s algos
        # Создаем бор, инициализируем
        # непосредственных потомков корневого узла
        # устанавливаем fail-функцию в корень
        self.aho_create_forest(patterns)

        queue = []
        for node in self.root.goto.values():
            queue.append(node)
            node.fail = self.root

        # Инициализируем остальные узлы:
        # 1. Берем очередной узел (важно, что проход в ширину)
        # 2. Находим самую длинную суффиксную ссылку для этой вершины - это и будет fail-функция
        # 3. Если таковой не нашлось - устанавливаем fail-функцию в корневой узел
        while len(queue) > 0:
            rnode = queue.pop(0)

            for key, unode in rnode.goto.items():
                queue.append(unode)
                fnode = rnode.fail
                while fnode != None and key not in fnode.goto.keys():
                    fnode = fnode.fail
                unode.fail = fnode.goto[key] if fnode else self.root
                unode.out += unode.fail.out


    def find_all(self, s, callback, begin=None):

        if begin == None:
            begin = self.root

        for i in range(len(s)):
            while node != None and s[i] not in node.goto.keys():
                node = node.fail
            if node == None:
                node = begin
                continue
            node = node.goto[s[i]]
            for pattern in node.out:
                callback(i - len(pattern) + 1, pattern)