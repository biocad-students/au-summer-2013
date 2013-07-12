from datetime import datetime

gifile = open('necessary_taxes.gi', 'w+')
taxfile = open('necessary_taxes.txt', 'w+')
t = open(r'C:\NR db\taxonomy db\gi_taxid_prot.dmp')
#t.seek(1440000000)
#t.readline()
s = t.readline()
i = 0
taxes = set()
filter_taxes = set([9606, 9592, 9596, 207598, 9844, 10090, 10114, 30536, 9838, 9837, 9836, 9986])
start_time = datetime.now()

while len(s) > 0:
    ls = s.split()
    current_tax = int(ls[1])
    current_gi = ls[0]
    taxes.add(current_tax)
    if current_tax in filter_taxes:
        i+=1
        gifile.writelines(current_gi+'\n')
        taxfile.writelines(current_gi+'\t'+ls[1]+'\n')
    s = t.readline()
gifile.close()
taxfile.close()
print("gi amount",i)
print("taxes amount",len(taxes))
print(datetime.now()-start_time)
