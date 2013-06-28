/*
	This is part of pyahocorasick Python module.
	
	AutomatonSearchIter const, struct & methods declarations.
	This class implements iterator walk over Aho-Corasick
	automaton. Object of this class is returnd by 'iter' method
	of Automaton class.

	Author    : Wojciech Mu�a, wojciech_mula@poczta.onet.pl
	WWW       : http://0x80.pl/proj/pyahocorasick/
	License   : 3-clauses BSD (see LICENSE)
	Date      : $Date: 2011-04-10 20:52:26 +0200 (nie) $

	$Id: AutomatonSearchIter.h 2324 2011-04-10 18:52:26Z wojtek $

*/
#ifndef ahocorasick_AutomatonSearchIter_h_included
#define ahocorasick_AutomatonSearchIter_h_included

#include "common.h"
#include "Automaton.h"

typedef struct AutomatonSearchIter {
	PyObject_HEAD

	Automaton*	automaton;
	int			version;	///< automaton version
	PyObject*	object;		///< unicode or buffer
	TRIE_LETTER_TYPE* data;	///< Py_UNICODE or char*
	TrieNode*	state;		///< current state of automaton
	TrieNode*	output;		///< current node, i.e. yielded value
	
	int			index;		///< current index in data
	int			shift;		///< shift + index => output index
	int			end;		///< end index
} AutomatonSearchIter;


static PyObject*
automaton_search_iter_new(
	Automaton* automaton,
	PyObject* object,
	int start,
	int end
);

#endif
