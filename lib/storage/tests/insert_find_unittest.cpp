// *iterator <-> pair<char, annotation>
template<class Iterator>
void add(Iterator start, Iterator end) {
	Iterator iter = start;
	trieIterator lastTrie = m_trie.begin();
	annotationIterator lastAnno = m_annotation.begin();
	// kmer buf
	std::vector<char> tmp(std::distance(start, end));
	int i = 0;
	while(iter !=end) {
		tmp[i] = iter->first;
		i++;
	}
	std::vector<char>::iterator kmer_iter = tmp.begin();
	// main cycle
	iter = start;
	while(iter != end) {
		lastTrie = m_trie.insert(lastTrie, iter->first);
		lastAnno = m_annotation.insert(lastAnno, iter->first);
		lastTrie->addLink(lastAnno);
		lastAnno->addLink(lastTrie);
		lastAnno->setAnno(iter->second);
		m_kstat.insert(kmer_iter, tmp.end(), lastanno);
		++kmer_iter;
		++iter;
	}
}

template<class Iterator>
void find(Iterator begin, Iterator end)
{
    trie<bool> color_trie(m_trie);
	std::fill(color_trie/*.DFS*/.begin(), color_trie/*.DFS*/.end(), false);
	size_t count = std::distance(begin, end) - 7;
	for(size_t i = 0; i != count; ++i)
	{
	    k_stat::value_type anno_cont = m_kstat.find(begin + i);
	    k_stat::value_type::const_iterator anno_iter = anno_cont.begin();
		for(; anno_iter != anno_cont.end(); ++anno_iter)
		{
		    color_trie[*anno_iter] = true;
		}
	}

	
	k_stat::value_type anno_cont = m_kstat.find(begin + count);
	k_stat::value_type::const_iterator anno_iter = anno_cont.begin();
	for(; anno_iter != anno_cont.end(); ++anno_iter)
	{
		bool found = true;
	    color_trie::itertor iter(*anno_iter);
		do
		{
			--iter;
			if(!*iter)
			{
				found = false;
				break;
			}
		}
		while(count != 0);

		if(found)
		{
			//visitor(*iter);
		}
	}
}