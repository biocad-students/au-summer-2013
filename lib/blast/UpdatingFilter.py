import GetGIbyTaxID
from Bio.Blast.Applications import BlastallCommandline
import os

#in data
dump = r'C:\NR db\taxonomy db\gi_taxid_prot_diff.dmp'
existing_taxdb = 'necessary_taxes.gi'

#out data
fasta_blast_hits = 'result.fa'

temp_newtaxes_cand = 'necessary_taxes1'
temp_newtaxes = temp_newtaxes_cand + 'ok'

#1 
GetGIbyTaxID.readDump(dump, temp_newtaxes_cand)

#2
newtaxes_file = open(temp_newtaxes, 'w+')
existing_taxes = set(open(existing_taxdb).readlines())
existing_tax_file = open(existing_taxdb, 'a')
for gi in open(temp_newtaxes_cand+'.gi').readlines():
    if gi not in existing_taxes:
        newtaxes_file.writelines(gi)
        existing_tax_file.writelines(gi)
newtaxes_file.close()

#3 blastdbcmd
cline = BlastallCommandline(program='blastdbcmd', db='nr', entry_batch=os.getcwd()+'\\'+temp_newtaxes, outfmt="%g %o")
cline()

#4 get title for unique oid and add to filter

#5 constructfilter again

os.remove(temp_newtaxes_cand+'.gi')
os.remove(temp_newtaxes_cand+'.txt')
os.remove(temp_newtaxes)
input()
