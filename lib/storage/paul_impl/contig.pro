TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tests/trie_test.cpp \
    tests/kstat_test.cpp \
    tests/annotation_test.cpp \
    tests/fasta_test.cpp \
    utils/fasta_reader.cpp \
    tests/contig_test.cpp

HEADERS += \
    contig/trie/* \
    contig/annotation/* \
    contig/kstat/* \
    contig/contig.hpp \
    unittest.h \
    utils/read.h \
    utils/kseq.h \
    utils/fasta_reader.h \
    contig/contig_const_iterator.hpp \
    contig/contig_iterator.hpp

QMAKE_CXXFLAGS += -std=c++11 -fopenmp -Wall
LIBS += -lz
