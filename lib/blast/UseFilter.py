import json

def Filter(taxes, chains, types):
    dict = json.load(open('FilterData'))
    gifile = open('filter.gi', 'w+')
    if taxes == 'all':
        taxes = dict.keys()
    else:
        taxes = taxes.split()
    if chains != 'all':
        chains = [int(i) for i in chains.split()]
    if types != 'all':
        types = [int(i) for i in types.split()]
    for t in taxes:
        a,b,c = dict[t]
        for gi, ch, ty in zip(a,b,c):
            if chains != 'all' and ch not in chains:
                continue
            if types != 'all' and ty not in types:
                continue
            gifile.writelines(str(gi)+'\n')
    gifile.close()
    return 'filter.gi'

def Stat():
    dict = json.load(open('FilterData'))
    for t in dict.items():
        print(t[0], len(t[1][1]))
        unch= h= l = 0
        unt=g=m=a=d=e=0
        for i in range(len(t[1][1])):
            if t[1][1][i] == 1:
                h += 1
            elif t[1][1][i] == 2:
                l += 1
            else: unch += 1

            if t[1][2][i] == 1:
                g += 1
            elif t[1][2][i] == 2:
                m += 1
            elif t[1][2][i] == 3:
                a += 1
            elif t[1][2][i] == 4:
                d += 1
            elif t[1][2][i] == 5:
                e += 1
            else: unt += 1
        print(unch, h, l)
        print(unt,g,m,a,d,e,0)

Stat()
