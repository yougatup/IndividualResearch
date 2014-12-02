#include <cstdio>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
const int VertexMAX = 100;
const int GraphMAX = 100;
const int kMAX = 100;
const int K = 2;
typedef int EdgeID;
typedef int VertexID;
typedef int GraphID;
typedef int VertexLabel;
typedef int EdgeLabel;
typedef pair<EdgeLabel, VertexLabel> SequenceElem;
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
		void setRoot(const VertexLabel s){
			root = s;
		}
		bool operator < (const Sequence& s, const Sequence& t){
			if(s.root < t.root) return true;
			else if(s.root > t.root) return false;
			assert(false);
		}
};
class KatIndex{
	public:
		map <Sequence, GraphID> Index;
		int count;
		KatIndex(){
			count = 0;
			Index.clear();
		}
		void Push(const Sequence& s){
			if(Index.count(s) != 0) return;
			Index.insert(s);
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
				vector <pair<GraphID, int> > obj& = Inv[s];
				for(int i=0;i<obj.size();i++){
					if(obj[i].first == x){
						obj[i].second++;
						return;
					}
				}
				obj.push(make_pair(x, 1));
			}
			else{
				vector <pair<GraphID, int> > temp;
				temp.push_back(make_pair(x, 1));
				Inv[s] = temp;
			}
		}
};

KatIndex index[K];
InvIndex inv[K];


class Graph{
	public:
		int VertexNum;
		int EdgeNum;
		int Label[VertexMAX];
		vector <SequenceElem> List[VertexMAX];
		void build_Table(bool flag){
			int tempLabel[VertexMAX] = {0,};
			for(int i=0;i<VertexNum;i++) tempLabel[i] = Label[i];
			for(int k=0;k<K;k++){
				for(int i=0;i<VertexNum;i++){
					Sequence temp;
					temp.setRoot(tempLabel[i]);
					for(int j=0;j<temp[i].size();j++){
						SequenceElem& Node2 = temp[i][j];
						int label = tempLabel[Node2.second];
					}
				}
			}
		}
}
int main(){

	return 0;
}