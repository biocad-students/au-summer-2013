from Bio.Blast.Applications import NcbiblastxCommandline
import os
import subprocess

def ExtractfromNewGilist(in_GIlist_filename, inout_Names_filename):
    temp_GItitle_filename = 'gi_title.txt'
    titlefile = open(temp_GItitle_filename, 'w+')
    giset = [i[:-1] for i in open(in_GIlist_filename).readlines()]
    print(len(giset))
    while len(giset) > 0:
        gi = giset.pop()
        args = [r'C:\Program Files\NCBI\blast-2.2.28+\bin\blastdbcmd.exe',
            '-db', 'nr',
            #'-entry_batch', os.getcwd()+'\\test.txt',
            '-entry', gi,
            '-outfmt', '%g\t%t'
            #,'-out', os.getcwd()+'\\out.txt'
            ]
        for x in subprocess.check_output(args).split(b'\r\n')[:-1]:
            titlefile.writelines(x.decode("utf-8")+'\n')
        print(len(giset),'left')
    titlefile.close()

    namesfile = open(inout_Names_filename, 'w+')
    print(len(s))
    count=0
    for pair in open(temp_GItitle_filename).readlines():
        current_gi, title = pair.split('\t')
        if 'Ig' in title or 'imm' in title or 'VH' in title or 'VL' in title:
            namesfile.writelines(pair)
            count+=1
    namesfile.close()
