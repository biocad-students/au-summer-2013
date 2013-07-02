_author__ = 'Aspart'

import gene

readpath = "C:\\Users\\Aspart\\Biocad\\taxon_human"

def on_occurence(pos, patterns):
    print("At pos %s found pattern: %s" % (pos, patterns))

genedata = gene.Gene()
genedata.create(readpath)
print(genedata.find("CAGC").goto)