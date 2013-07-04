from Bio import Blast
from Bio.Blast.Applications import NcbiblastxCommandline
from Bio import SeqIO
from Bio import Seq
from Bio.Blast import NCBIWWW

nr_db_name = "C:\\NR db\\nr.00"
in_protein_name = "VH_pro_trust.fasta"

in_protein = list(SeqIO.parse(open(in_protein_name), format="fasta"))[0]
print(in_protein)

xml_results_name = "results.xml"

blastx_cline = NcbiblastxCommandline(query=in_protein, 
                                     db=nr_db_name, 
                                     evalue=0.001, 
                                     outfmt=5, 
                                     out=xml_results_name)
blastx_cline()

for blast_record in NCBIXML.parse(open(xml_results_name)):
    print(blast_record)
