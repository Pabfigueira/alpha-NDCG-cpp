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

        void load_query_topics(dictString query_topics) {
            this->query_topics_dict = query_topics;
        }

        void load_doc_topics(dictString doc_topics) {
            this->doc_topics_dict = doc_topics;
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
        	ll local_depth;

        	for(auto iteratorQuery : ranking_query_doc) {
        		query = iteratorQuery.first;

        		local_depth = min(depth, (ll)ranking_query_doc[query].size());
        		// TODO Finish this function and finish the other functions
        		/*
 				idealRanking = get_ideal_ranking(query, ranking_query_doc[query], local_depth);
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
 				}*/
        	}
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
    return 0;
}
