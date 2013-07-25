from Bio.Blast.Applications import NcbiblastxCommandline
import os
import subprocess

oidfile = open('necessary_taxeslocal.oid', 'w+')
filter_taxes = set(['9606', '9596', '9844', '10090', '9838', '9986'])
giset = set(i[:-1] for i in open('necessary_taxeslocal.gi').readlines())
print(len(giset))
d = dict.fromkeys(giset)
while len(giset) > 0:
    gi = giset.pop()
    args = [r'C:\Program Files\NCBI\blast-2.2.28+\bin\blastdbcmd.exe',
        '-db', 'nr',
        #'-entry_batch', os.getcwd()+'\\test.txt',
        '-entry', gi,
        '-outfmt', '%g %o %T'
        #,'-out', os.getcwd()+'\\out.txt'
        ]
    print('gi='+gi)
    samegi = set()
    for x in subprocess.check_output(args).split(b'\r\n')[:-1]:
        ls = x.decode("utf-8").split()
        if ls[2] in filter_taxes:
            samegi.add(ls[0])
    giset = giset - samegi
    oid = x.decode("utf-8").split()[1]
    for x in samegi:
        oidfile.writelines(x+' '+oid+'\n')
    print(len(giset),'left')
oidfile.close()
input()
