from Bio import SeqIO
from Bio.Seq import Seq
from Bio.SeqRecord import SeqRecord
from Bio.Blast.Applications import NcbiblastpCommandline
from Bio.Blast import NCBIXML
import UseFilter

in_protein_name = r"C:\test.fa"
out_aligments_file = "result.fa"
filter_taxid = '9606'
filter_chain = 'all'
filter_Igtype = 'all'

xml_results_name = "temp_results.xml"
nr_db_name = "nr"
gilist_name = UseFilter.Filter(filter_taxid, filter_chain, filter_Igtype)
in_protein = list(SeqIO.parse(open(in_protein_name), format="fasta"))
blastx_cline = NcbiblastpCommandline(query=in_protein_name,
                                     db=nr_db_name, 
                                     gilist=gilist_name,
                                     evalue=0.001, 
                                     outfmt=5, 
                                     out=xml_results_name)
blastx_cline()
bad_aligments=0
resultlist = []
for blast_record in NCBIXML.parse(open(xml_results_name)):
    for alignment in blast_record.alignments:
        for hsp in alignment.hsps:
            if hsp.expect < 0.001: 
                resultlist.append(SeqRecord(Seq(hsp.sbjct), id=alignment.title, description=';length:'+str(alignment.length)+';e-value:'+str(hsp.expect)+';similarity:'+str(hsp.identities*1000//hsp.align_length/10)+'%'))
            else:
                bad_aligments += 1
#print('bad_aligments = ',bad_aligments)
SeqIO.write(resultlist, out_aligments_file, "fasta")
