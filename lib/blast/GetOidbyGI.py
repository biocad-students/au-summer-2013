import os
import subprocess

def createOIDlist(in_GIlist_filename, out_OIDlist_filename):
    oidfile = open(out_OIDlist_filename, 'w+')
    filter_taxes = set(['9606', '9596', '9844', '10090', '9838', '9986'])
    giset = set(i[:-1] for i in open(in_GIlist_filename).readlines())
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

def PickOutGIforNewOID(in_GIlist_filename, inout_existingOID_filename, out_newGIlist_filename):
    new_OIDlist = 'temp_oidlist'
    createOIDlist(in_GIlist_filename, new_OIDlist)

    gifile = open(out_newGIlist_filename, 'w+')
    existing_oid = set(open(inout_existingOID_filename).readlines())
    oidlist = open(new_OIDlist).readlines()
    for i in oidlist:
        existing_oid.writelines(i)
    d = dict.fromkeys(set(i[:-1].split()[1] for i in oidlist)-existing_oid, 0)
    for l in oidlist:
        if d.get(l[1]) == 0:
            d[l[1]] = l[0]
    print(len(d))
    for v in d.values():
        gifile.writelines(v+'\n')
    os.remove(new_OIDlist)
