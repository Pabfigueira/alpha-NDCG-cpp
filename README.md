# alpha-NDCG-cpp
C++ Implementation of alpha-NDCG

## Usage (Example from [Clarke, 2008])

Query-Topics Dictionary [A dictionary of topics relevants per query]
```c++
typedef map<string,vector<double>> dcgStruct;
typedef map<string,vector<string>> dictString;
typedef long long int ll;


dictString queryDict;
queryDict["QA Example"] = {"85.1", "85.2", "85.3", "85.4", "85.5", "85.6"};
queryDict["QB Example"] = {"85.1", "85.2", "85.3", "85.4", "85.5", "85.6"};
```
Doc-Topics Dictionary [A dictionary of topics relevants per document]
```c++
dictString docDict;
docDict["a"] = {"85.2", "85.4"};
docDict["b"] = {"85.2"};
docDict["c"] = {"85.2"};
docDict["d"] = {};
docDict["e"] = {"85.1", "85.6"};
docDict["f"] = {"85.1"};
docDict["g"] = {"85.3"};
docDict["h"] = {"85.1"};
docDict["i"] = {};
docDict["j"] = {};
```

Ranking Query-Doc Dictionary [A dictionary of ranking to calculate the alphaNDCG per query]
```c++
dictString rankingDict;
rankingDict["QA Example"] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
rankingDict["QB Example"] = {"a", "e", "g", "b", "f", "c", "h", "i", "j", "d"};
```

Returns score
```c++
AlphaNDCG myAlpha = AlphaNDCG(queryDict, docDict);
myAlpha.calculate_Alpha_nDCG(rankingDict);

cout << "nDCG Values" << endl;
for(auto iteratorQuery : myAlpha.get_ndcg_values()) {
	query = iteratorQuery.first;
	cout << query << ": ";
	for(auto elem : iteratorQuery.second) {
		cout << elem << " ";
	}
	cout << endl;
}
```

## References
Clarke, Charles LA, et al. "Novelty and diversity in information retrieval evaluation." Proceedings of the 31st annual international ACM SIGIR conference on Research and development in information retrieval. ACM, 2008. [\[pdf\]](http://plg.uwaterloo.ca/~gvcormac/novelty.pdf)
