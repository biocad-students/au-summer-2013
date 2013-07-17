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
    trie/* \
    annotation/* \
    kstat/* \
    unittest.h \
    contig.hpp \
    algorithm.hpp

QMAKE_CXXFLAGS += -std=c++11
