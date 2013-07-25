from datetime import datetime

def readDump(dump_filename, out_filename):
    gifile = open(out_filename+'.gi', 'w+')
    taxfile = open(out_filename+'.txt', 'w+')
    t = open(dump_filename)
    #t.seek(1440000000)
    #t.readline()
    s = t.readline()
    i = 0
    taxes = set()
    filter_taxes = set([9606, 9596, 9844, 10090, 9838, 9986])

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

def readTxt():
    t = open(r'C:\Users\student.BIOCAD\AppData\Local\VirtualStore\Program Files\NCBI\blast-2.2.28+\bin\taxlist.txt')
    gifile = open('necessary_taxeslocal.gi', 'w+')
    taxfile = open('necessary_taxeslocal.txt', 'w+')
    filter_taxes = set(['9606', '9596', '9844', '10090', '9838', '9986'])
    count=i=0
    for s in t:
        count+=1
        current_gi, current_tax = s.split()
        if current_tax in filter_taxes:
            i+=1
            gifile.writelines(current_gi+'\n')
            taxfile.writelines(s)
    print("gi amount",i)
    print("in amount",count)

if __name__ == "__main__":
    start_time = datetime.now()
    readDump(r'C:\NR db\taxonomy db\gi_taxid_prot_diff.dmp', 'necessary_taxes1')
    print(datetime.now()-start_time)
    input()
