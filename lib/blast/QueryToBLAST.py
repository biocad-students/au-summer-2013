from Bio import Blast
from Bio.Blast.Applications import NcbiblastpCommandline
from Bio import SeqIO
from Bio import Seq

nr_db_name = "nr"
in_protein_name = r"C:\test.fa"
gilist_name = r"C:\human.gi.txt"
seqidlist_name = r"C:\biocad_summer_blast\human.txt"


in_protein = list(SeqIO.parse(open(in_protein_name), format="fasta"))[0]
#print(in_protein)

xml_results_name = "results1.xml"

blastx_cline = NcbiblastpCommandline(query=in_protein_name,
                                     db=nr_db_name, 
                                     gilist=gilist_name,
                                     #seqidlist=seqidlist_name,
                                     db_soft_mask='4',
                                     evalue=0.001, 
                                     outfmt=5, 
                                     out=xml_results_name)
blastx_cline()
print(open(xml_results_name).read())
