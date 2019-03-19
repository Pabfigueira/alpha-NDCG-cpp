#include<bits/stdc++.h>

using namespace std;

typedef map<string,vector<double>> dcgStruct;
typedef map<string,vector<string>> dictString;
typedef long long int ll;

class AlphaNDCG{
    double alpha;
    dictString query_topics_dict;
    dictString doc_topics_dict;
    dcgStruct dcg_values;
    dcgStruct ndcg_values;
    public:
        AlphaNDCG(dictString query_topics, dictString doc_topics, double alpha=0.5) {
            set_alpha(alpha);
            load_query_topics(query_topics);
            load_doc_topics(doc_topics);    
            this->dcg_values.clear();
            this->ndcg_values.clear();
        }

        void set_alpha(double alpha) {
            this->alpha = alpha;
        }

        double get_alpha() {
        	return this->alpha;
        }

        void load_query_topics(dictString query_topics) {
            this->query_topics_dict = query_topics;
        }

        void load_doc_topics(dictString doc_topics) {
            this->doc_topics_dict = doc_topics;
        }

        dcgStruct get_dcg_values() {
        	return this->dcg_values;
        }

        dcgStruct get_ndcg_values() {
        	return this->ndcg_values;
        }

        dcgStruct calculate_Alpha_DCG(dictString ranking_query_doc, ll depth=20ll) {
        	double value;
        	string query;
        	ll local_depth, i;
        	dcgStruct local_dcg_values;
        	set<string> topics_query, topics_intersection;
        	map<string,ll> topics_number_of_occurrences;

        	for(auto iteratorQuery : ranking_query_doc) {
        		query = iteratorQuery.first;
        		topics_query = createSet(this->query_topics_dict[query]);
        		topics_number_of_occurrences.clear();

        		local_depth = min(depth, (ll)ranking_query_doc[query].size());
        		local_dcg_values[query].resize(local_depth,0.0);

        		value = 0.0;
        		for(i=0; i<local_depth; i++) {
        			topics_intersection = intersection_set(createSet(this->doc_topics_dict[ranking_query_doc[query][i]]), topics_query);
        			for(auto topic : topics_intersection) {
        				value += myPow((1 - this->alpha), topics_number_of_occurrences[topic]) / logInBase(2+i,2);
        				topics_number_of_occurrences[topic]++;
        			}
        			local_dcg_values[query][i] = value;
        		}
        	}
        	return local_dcg_values;
        }
        
        void compute_Alpha_DCG(dictString ranking_query_doc, ll depth=20ll) {
        	this->dcg_values = calculate_Alpha_DCG(ranking_query_doc,depth);	
        }

        void calculate_Alpha_nDCG(dictString ranking_query_doc, ll depth=20ll) {
        	compute_Alpha_DCG(ranking_query_doc, depth);

        	string query;
        	ll local_depth, i;
        	vector<string> idealRanking;
        	dcgStruct dcg_ideal_ranking;
        	dictString auxiliarDict;

        	for(auto iteratorQuery : ranking_query_doc) {
        		query = iteratorQuery.first;

        		local_depth = min(depth, (ll)ranking_query_doc[query].size());
 				idealRanking = get_ideal_ranking(query, ranking_query_doc[query], local_depth);

 				cout << "Printing ideal ranking: " << query << endl;
 				for(auto elem : idealRanking) {
 					cout << elem << " ";
 				}
 				cout << endl;

 				auxiliarDict.clear();
 				auxiliarDict[query] = idealRanking;
 				dcg_ideal_ranking = calculate_Alpha_DCG(auxiliarDict,local_depth);

 				this->ndcg_values[query].resize(local_depth,0.0);

 				for(i=0; i<local_depth; i++) {
 					if(this->dcg_values[query][i]==0.0) {
 						this->ndcg_values[query][i] = 0.0;
 					} else {
 						this->ndcg_values[query][i] = (this->dcg_values[query][i]/(double)dcg_ideal_ranking[query][i]);
 					}
 				}
        	}
        }

        vector<string> get_ideal_ranking(string query, vector<string> actual_ranking, ll depth=20ll) {
        	string topic, whoIsBest;
        	double bestValue, value;
        	vector<string> ideal_ranking;
        	map<string,ll> topics_number_of_occurrences;
        	map<string,set<string>> topics_intersection;
        	set<string> topics_query, doc_candidates, topics_of_best; 

        	topics_query = createSet(this->query_topics_dict[query]);
        	
        	doc_candidates = createSet(actual_ranking);

        	while((ll)doc_candidates.size()>0 and (ll)ideal_ranking.size()<depth) {
        		bestValue = numeric_limits<double>::lowest();
        		whoIsBest = "noOne";
        		topics_of_best.clear();

        		// Memoization
        		topics_intersection.clear();
        		for(auto document : doc_candidates) {
        			topics_intersection[document] = intersection_set(createSet(this->doc_topics_dict[document]), topics_query);
        		}

        		for(auto document : doc_candidates) {
        			value = 0.0;

        			for(auto topic : topics_intersection[document]) {
        				value += myPow((1 - this->alpha),topics_number_of_occurrences[topic]) / logInBase(2+(ll)ideal_ranking.size(), 2);
        			}

        			if(value > bestValue) {
        				bestValue = value;
        				whoIsBest = document;
        				topics_of_best = topics_intersection[document];
        			}
        		}

        		for(auto topic : topics_of_best) {
        			topics_number_of_occurrences[topic]++;
        		}
        		ideal_ranking.push_back(whoIsBest);
        		doc_candidates.erase(whoIsBest);
        	}
        	return ideal_ranking;
        }

        vector<double> compute_single_Alpha_DCG(string query, vector<string> ranking, ll depth=20ll) {
        	double value;
        	ll local_depth, i;
        	vector<double> local_dcg_values;
        	set<string> topics_query, topics_intersection;
        	map<string,ll> topics_number_of_occurrences;

        	topics_query = createSet(this->query_topics_dict[query]);

        	local_depth = min(depth,(ll)ranking.size());
        	local_dcg_values.resize(local_depth,0.0);

        	value = 0.0;
        	for(i=0; i<local_depth; i++) {
        		topics_intersection = intersection_set(createSet(this->doc_topics_dict[ranking[i]]), topics_query);

        		for(auto topic : topics_intersection) {
        			value += myPow((1 - this->alpha), topics_number_of_occurrences[topic]) / logInBase(2+i,2);
        			topics_number_of_occurrences[topic]++;
        		}
        		local_dcg_values[i] = value;
        	}
        	return local_dcg_values;
        }


        vector<double> compute_single_Alpha_nDCG(string query, vector<string> target_ranking, vector<string> ideal_ranking, ll depth=20ll) {        	
        	ll local_depth, i;
        	vector<double> dcg_target_ranking, dcg_ideal_ranking, ndcg_values;

        	local_depth = min(depth, min((ll)target_ranking.size(), (ll)ideal_ranking.size()));

        	dcg_target_ranking = compute_single_Alpha_DCG(query, target_ranking, local_depth);
        	dcg_ideal_ranking  = compute_single_Alpha_DCG(query, ideal_ranking, local_depth);

        	ndcg_values.resize(local_depth,0.0);

        	for(i=0; i<local_depth; i++) {
        		if(dcg_target_ranking[i]==0.0){
        			ndcg_values[i] = 0.0;
        		} else {
        			ndcg_values[i] = dcg_target_ranking[i]/(double)dcg_ideal_ranking[i];
        		}
        	}
        	return ndcg_values;
        }


        /* BEGIN: Util Functions */
        set<string> createSet(vector<string>& arr) {
        	set<string> ans;
        	for(auto word : arr) {
        		ans.insert(word);
        	}
        	return ans;
        }

        set<string> intersection_set(set<string> setOne, set<string> setTwo) {
        	if(setOne.size() > setTwo.size()) {
        		return intersection_set(setTwo,setOne);
        	}

        	set<string> ans;
        	for(auto word : setOne) {
        		if(setTwo.find(word)!=setTwo.end()) {
        			ans.insert(word);
        		}
        	}
        	return ans;
        }

        double myPow(double base, double power) {
        	double ans = 1;
        	while(power) {
        		ans *= base;
        		power--;
        	}
        	return ans;
        }

        double logInBase(double number, double base) {
        	return (log(number)/(double)log(base));
        }
        /* END: Util Functions */

};


int main() {
/*
	// Query-Topics Dictionary 
	dictString queryDict;
	queryDict["QA Example"] = {"85.1", "85.2", "85.3", "85.4", "85.5", "85.6"};
	queryDict["QB Example"] = {"85.1", "85.2", "85.3", "85.4", "85.5", "85.6"};

	// Doc-Topics Dictionary 
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

	// Ranking Query-Doc Dictionary 
	dictString rankingDict;
	rankingDict["QA Example"] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
	rankingDict["QB Example"] = {"a", "e", "g", "b", "f", "c", "h", "i", "j", "d"};

	AlphaNDCG myAlpha = AlphaNDCG(queryDict, docDict);
	myAlpha.calculate_Alpha_nDCG(rankingDict);

	string query;

	cout << "DCG Values" << endl;
	for(auto iteratorQuery : myAlpha.get_dcg_values()) {
		query = iteratorQuery.first;
		cout << query << ": "; 
		for(auto elem : iteratorQuery.second) {
			cout << elem << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
	
	cout << "nDCG Values" << endl;
	for(auto iteratorQuery : myAlpha.get_ndcg_values()) {
		query = iteratorQuery.first;
		cout << query << ": ";
		for(auto elem : iteratorQuery.second) {
			cout << elem << " ";
		}
		cout << endl;
	}


	// Testing single calculation //
	cout << endl << endl << "nDCG Values - Single" << endl;
	vector<string> ideal_ranking;
	ideal_ranking = myAlpha.get_ideal_ranking("QA Example", rankingDict["QA Example"]);
	cout << "QB Example: ";
	for(auto value : myAlpha.compute_single_Alpha_nDCG("QA Example", rankingDict["QB Example"], ideal_ranking)) {
		cout << value << " ";
	}
	cout << endl;
	cout << "QA Example: ";
	for(auto value : myAlpha.compute_single_Alpha_nDCG("QA Example", rankingDict["QA Example"], ideal_ranking)) {
		cout << value << " ";
	}
	cout << endl;
*/	
    return 0;
}
