TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tests/trie_test.cpp \
    tests/kstat_test.cpp \
    tests/annotation_test.cpp \
    tests/test.cpp

HEADERS += \
    contig/trie/* \
    contig/annotation/* \
    contig/kstat/* \
    contig/contig.hpp \
    contig/algorithm.hpp \
    unittest.h

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lz
