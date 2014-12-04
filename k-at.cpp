#include <cstdio>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
const int VertexMAX = 100;
const int EdgeMAX = 100;
const int GraphMAX = 100;
const int kMAX = 100;
const int K = 2;
typedef int EdgeID;
typedef int VertexID;
typedef int GraphID;
typedef int VertexLabel;
typedef int EdgeLabel;
typedef pair<EdgeLabel, VertexLabel> SequenceElem;
typedef pair<EdgeID, VertexID> ListElem;
class Sequence{
	public:
		VertexLabel root;
		vector <SequenceElem> Seq;
		Sequence(){
			root = -1;
			Seq.clear();
		}
		void push_elem(const SequenceElem& elem){
			Seq.push_back(elem);
		}
		void sort_sequence(){
			sort(Seq.begin(), Seq.end());
		}
		void setRoot(const VertexLabel s){
			root = s;
		}
};
bool operator < (const Sequence& s, const Sequence& t){
	if(s.root < t.root) return true;
	else if(s.root > t.root) return false;
	assert(false);
}

bool operator > (const Sequence& s, const Sequence& t){
	if(s.root > t.root) return true;
	else if(s.root < t.root) return false;
	assert(false);
}

bool operator == (const Sequence& s, const Sequence& t){
	return s.root == t.root;
}
class KatIndex{
	public:
		map <Sequence, GraphID> Index;
		int count;
		KatIndex(){
			count = 0;
			Index.clear();
		}
		GraphID Push(const Sequence& s){
			int temp = Index.count(s);
			if(temp != 0) return temp;
			Index[s] = ++count;
			return count;
		}
		GraphID getID(const Sequence& s){
			if(Index.count(s) == 0) return -1;
			return Index[s];
		}
};
class InvIndex{
	public:
		map <Sequence, vector <pair<GraphID, int> > > Inv;
		InvIndex(){
			Inv.clear();
		}
		void Push(const Sequence& s, const GraphID x){
			if(Inv.count(s) != 0){
				vector <pair<GraphID, int> > &obj = Inv[s];
				for(int i=0;i<obj.size();i++){
					if(obj[i].first == x){
						obj[i].second++;
						return;
					}
				}
				obj.push_back(make_pair(x, 1));
			}
			else{
				vector <pair<GraphID, int> > temp;
				temp.push_back(make_pair(x, 1));
				Inv[s] = temp;
			}
		}
};

KatIndex Kindex[K];
InvIndex inv[K];


class Graph{
	public:
		int VertexNum;
		int EdgeNum;
		int VLabel[VertexMAX];
		int ELabel[EdgeMAX];
		vector <ListElem> List[VertexMAX];
		void build_Table(bool flag){
			int tempLabel[VertexMAX] = {0,};
			int newLabel[VertexMAX] = {0,};
			for(int i=0;i<VertexNum;i++) tempLabel[i] = VLabel[i];
			for(int k=0;k<K;k++){
				for(int i=0;i<VertexNum;i++){
					Sequence temp;
					temp.setRoot(tempLabel[i]);
					for(int j=0;j<List[i].size();j++){
						SequenceElem& Node2 = List[i][j];
						VertexLabel vl = tempLabel[Node2.second];
						EdgeLabel el = ELabel[Node2.first];
						temp.push_elem(SequenceElem(el, vl));
					}
					temp.sort_sequence();
					int id = Kindex[k].getID(temp);
					if(id == -1) id = Kindex[k].Push(temp);
					newLabel[i] = id;
				}
			}
		}
};
int main(){
	Graph dataGraph[GraphMAX];
	return 0;
}
