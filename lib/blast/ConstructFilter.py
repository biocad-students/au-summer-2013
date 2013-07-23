import json
from datetime import datetime

start_time = datetime.now()
namesfile = open('necessary_names.txt')
taxfile = open('necessary_taxes.txt')
filterfile = open('FilterData', 'w+')

missmatch = 0
mainDict = {9606 : [[], [], []], 9596 : [[], [], []], 9844 : [[], [], []], 10090 : [[], [], []], 9838 : [[], [], []], 9986 : [[], [], []]}
taxdict = {}
for pair in taxfile.readlines():
    gi, tax = (int(i) for i in pair.split())
    taxdict[gi] = tax
print('taxdict is completed')
for pair in namesfile.readlines():
    ls = pair.split('   ')
    current_gi = int(ls[0])
    if 'heavy' in ls[1] or 'VH' in ls[1] or 'Vh' in ls[1]:
        chain = 1
    elif 'light' in ls[1] or 'VL' in ls[1] or 'Vl' in ls[1]:
        chain = 2
    else:
        chain = 0

    if 'IgG' in ls[1]:
        type = 1
    elif 'IgM' in ls[1]:
        type = 2
    elif 'IgA' in ls[1]:
        type = 3
    elif 'IgD' in ls[1]:
        type = 4
    elif 'IgE' in ls[1]:
        type = 5
    else:
        type = 0

    if taxdict.get(current_gi) == None:
        missmatch += 1
        continue
    taxid = taxdict[current_gi]
    mainDict[taxid][0].append(current_gi)
    mainDict[taxid][1].append(chain)
    mainDict[taxid][2].append(type)

filterfile.write(json.dumps(mainDict))
filterfile.close()
print(missmatch, datetime.now()-start_time)
input()
