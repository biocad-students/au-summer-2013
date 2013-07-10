from Bio.Blast import NCBIXML

xml_results_name = 'blasth.xml'

bad_aligments=0
for blast_record in NCBIXML.parse(open(xml_results_name)):
    print(len(blast_record.alignments))
    for alignment in blast_record.alignments:
        #if len(alignment.hsps) != 1:
            print(len(alignment.hsps))
            for hsp in alignment.hsps:
                if hsp.expect < 0.001:
                    print('****Alignment****')
                    print('sequence:', alignment.title)
                    print('length:', alignment.length)
                    print('e value:', hsp.expect)
                    print(hsp.query[0:75] + '...')
                    print(hsp.match[0:75] + '...')
                    print(hsp.sbjct[0:75] + '...')
                else:
                    bad_aligments += 1
print('bad_aligments = ',bad_aligments)
input()
