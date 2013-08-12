import GetGIbyTaxID
import GetOIDbyGI
from Bio.Blast.Applications import BlastallCommandline
import os

#in data
dump = r'C:\NR db\taxonomy db\gi_taxid_prot_diff.dmp'
existing_taxdb = 'necessary_taxes.gi'
existing_oid = 'necessary_taxeslocal.oid'
names_for_filter = 'necessary_names.txt'

#out data
fasta_blast_hits = 'result.fa'

temp_newtaxes_cand = 'necessary_taxes1'
temp_newtaxes = temp_newtaxes_cand + 'ok'
temp_newGI = temp_newtaxes_cand + '.oid'

#1 
GetGIbyTaxID.readDump(dump, temp_newtaxes_cand)

#2 get new GI list only and add to existing gi list
newtaxes_file = open(temp_newtaxes, 'w+')
existing_taxes = set(open(existing_taxdb).readlines())
existing_tax_file = open(existing_taxdb, 'a')
for gi in open(temp_newtaxes_cand+'.gi').readlines():
    if gi not in existing_taxes:
        newtaxes_file.writelines(gi)
        existing_tax_file.writelines(gi)
newtaxes_file.close()

#3
GetOIDbyGI.PickOutGIforNewOID(temp_newtaxes, existing_oid, temp_newGI)

#4 get title for unique oid and add to filter
ExpandNames.ExtractfromNewGilist(temp_newGI, names_for_filter)

#5 constructfilter again

os.remove(temp_newtaxes_cand+'.gi')
os.remove(temp_newtaxes_cand+'.txt')
os.remove(temp_newtaxes)
os.remove(temp_newGI)
input()
