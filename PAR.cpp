#include <cstdio>
#include <ctime>
#include <cassert>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
const int Thres = 4;
const int VertexMAX = 200;
const int EdgeMAX = 250;
const int GraphMAX = 10005;
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
		int get_size() const { return Seq.size(); }
		SequenceElem get_ith(int i) const { return Seq[i]; }
		void print(){
			fprintf(stderr,"%d ",root);
			for(int i=0;i<Seq.size();i++) fprintf(stderr, "(%d, %d) ",Seq[i].first, Seq[i].second);
		}
};
bool operator < (const Sequence& s, const Sequence& t){
	if(s.root < t.root) return true;
	else if(s.root > t.root) return false;
	else{
		int s_sz = s.get_size();
		int t_sz = t.get_size();
		for(int i=0;i<min(s_sz, t_sz);i++){
			SequenceElem ss, tt;
			ss = s.get_ith(i);
			tt = t.get_ith(i);
			if(ss.first < tt.first) return true;
			else if(ss.first > tt.first) return false;
			else if(ss.first == tt.first){
				if(ss.second < tt.second) return true;
				else if(ss.second > tt.second) return false;
			}
		}
		if(s_sz < t_sz) return true;
		else return false;
	}
}

bool operator > (const Sequence& s, const Sequence& t){
	if(s.root > t.root) return true;
	else if(s.root < t.root) return false;
	else{
		int s_sz = s.get_size();
		int t_sz = t.get_size();
		for(int i=0;i<min(s_sz, t_sz);i++){
			SequenceElem ss, tt;
			ss = s.get_ith(i);
			tt = t.get_ith(i);
			if(ss.first > tt.first) return true;
			else if(ss.first < tt.first) return false;
			else if(ss.first == tt.first){
				if(ss.second > tt.second) return true;
				else if(ss.second < tt.second) return false;
			}
		}
		if(s_sz > t_sz) return true;
		else return false;
	}
}

bool operator == (const Sequence& s, const Sequence& t){
	if(s.root != t.root) return false;
	if(s.get_size() != t.get_size()) return false;
	int sz = s.get_size();
	for(int i=0;i<sz;i++){
		SequenceElem ss, tt;
		ss = s.get_ith(i);
		tt = t.get_ith(i);
		if(ss.first != tt.first || ss.second != tt.second)
			return false;
	}
	return true;
}

bool operator <= (const Sequence& s, const Sequence& t){
	return s < t || s == t;
}

bool operator >= (const Sequence& s, const Sequence& t){
	return s > t || s == t;
}

bool operator != (const Sequence& s, const Sequence& t){
	return !(s == t);
}


class KatIndex{
	public:
		map <Sequence, GraphID> Index;
		int count;
		KatIndex(){
			count = -1;
			Index.clear();
		}
		GraphID Push(const Sequence& s){
			int temp = Index.count(s);
			if(temp != 0) return -1;
			Index[s] = ++count;
			return count;
		}
		GraphID getID(const Sequence& s){
			if(Index.count(s) <= 0) return -1;
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
			else{ // there is no such s
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
		int GraphNo;
		int VertexNum;
		int EdgeNum;
		int maxDegree;
		int VLabel[VertexMAX];
		int ELabel[EdgeMAX];
		map <int, int> Table[K];
		vector <ListElem> List[VertexMAX];
		vector <int> Partition[Thres+1];
		Graph() { GraphNo = VertexNum = EdgeNum = maxDegree = 0; }
		int getELabel(int v1, int v2) const{
			for(int i=0;i<List[v1].size();i++){
				ListElem Node2 = List[v1][i];
				if(Node2.second == v2)
					return ELabel[Node2.first];
			}
			return -1;
		}
		void print_Table(){
			for(int i=0;i<K;i++){
				fprintf(stderr, "*** k = %d *** \n",i);
				map<int, int> :: iterator it;
				for(it = Table[i].begin();it != Table[i].end();it++)
					fprintf(stderr, "(%d, %d)\n",(*it).first, (*it).second);
				fprintf(stderr, "\n\n");
			}
		}
		void getMaxDegree(){
			for(int i=0;i<VertexNum;i++)
				maxDegree = max(maxDegree, (int)List[i].size());
		}
		void make_partition(){
			bool Check[VertexMAX] = {0,};
			queue <int> Queue[Thres+2];
			assert(VertexNum >= Thres+1);
			for(int i=0;i<Thres+1;i++){
				while(1){
					int x = rand() % VertexNum;
					if(!Check[x]){
						Queue[i].push(x);
						Check[x] = true;
						Partition[i].push_back(x);
						break;
					}
				}
			}
			//fprintf(stderr, "hmm..\n");
			while(1){
				bool flag2 = false;
				for(int i=0;i<Thres+1;i++){
					if(!Queue[i].empty()){
						flag2 = true;
						bool flag = false;
						int Node = Queue[i].front();
						for(int j=0;j<List[Node].size();j++){
							ListElem Node2 = List[Node][j];
							if(!Check[Node2.second]){
								Check[Node2.second] = true;
								Queue[i].push(Node2.second);
								Partition[i].push_back(Node2.second);
								flag = true;
								break;
							}
						}
						if(!flag) Queue[i].pop();
					}
				}
				if(!flag2) break;
			}
			
			/*
			for(int i=0;i<Thres+1;i++){
				for(int j=0;j<Partition[i].size();j++) fprintf(stderr, "%d ",Partition[i][j]);
				fprintf(stderr, "\n");
			}
			*/
		}
		int Input(FILE* fp){
			/*
			fscanf(fp,"%d %d",&VertexNum, &EdgeNum);
			if(feof(fp)) return -1;
			for(int i=0;i<VertexNum;i++){
			int a;
			fscanf(fp, "%d",&a);
			VLabel[i] = a;
			}
			for(int i=0;i<EdgeNum;i++){
			int a, b, c;
			fscanf(fp,"%d %d %d",&a, &b, &c);
			List[a].push_back(ListElem(i, b));
			List[b].push_back(ListElem(i, a));
			ELabel[i] = c;
			}
			getMaxDegree();*/
			char temp1, temp2;
			fscanf(fp,"%c %d",&temp2, &GraphNo);
			if(feof(fp)) return -1;
			while(1){
				fscanf(fp,"%c",&temp1);
				if(feof(fp)) break;
				if(temp1 == 'v'){
					int x, y;
					fscanf(fp,"%d %d",&x, &y);
					VLabel[x] = y;
					assert(x == VertexNum);
					VertexNum++;
				}
				else if(temp1 == 'e'){
					int x, y, z;
					fscanf(fp,"%d %d %d",&x, &y, &z);
					List[x].push_back(ListElem(EdgeNum, y));
					List[y].push_back(ListElem(EdgeNum, x));
					ELabel[EdgeNum++] = z;
				}
				else if(temp1 == 't') break;
				else assert(true);
			}
			getMaxDegree();
			assert(VertexNum < VertexMAX);
			assert(EdgeNum < EdgeMAX);
			return 0;
		}
		void Output(FILE *fp1){
			fprintf(fp1,"%d %d\n",VertexNum, EdgeNum);
			for(int i=0;i<VertexNum;i++) fprintf(fp1,"%d ",VLabel[i]);
			fprintf(fp1,"\n");
			for(int i=0;i<EdgeNum;i++) fprintf(fp1,"%d ",ELabel[i]);
			fprintf(fp1,"\n");

			for(int i=0;i<VertexNum;i++){
				fprintf(fp1,"%d : ",i);
				for(int j=0;j<List[i].size();j++){
					ListElem Node2 = List[i][j];
					fprintf(fp1,"(%d, %d)", Node2.first, Node2.second);
				}
				fprintf(fp1,"\n");
			}
		}
};
int get_common(const map<int, int>& A, const map<int, int>& B){
	int result = 0;
	map<int, int> :: const_iterator it1;
	map<int, int> :: const_iterator it2;

	it1 = A.begin();
	it2 = B.begin();

	while(it1 != A.end() && it2 != B.end()){
		if(it1->first == it2->first){
			result += min(it1->second, it2->second);
			it1++;
			it2++;
		}
		else if(it1->first < it2->first) it1++;
		else it2++;
	}

	return result;
}
int get_power(int x, int y){
	if(y <= 0) return 1;
	else{
		if(y % 2 == 0){
			int temp = get_power(x, y/2);
			return temp * temp;
		}
		else return x * get_power(x, y-1);
	}
}
bool SubgraphIsomorphicRecursion(int cur, const vector <int>& order, int mapping[VertexMAX], int inv[VertexMAX], const Graph& s, const Graph& t){
	if(cur >= order.size()) return true;

	int Node = order[cur];
	for(int i=0;i<t.VertexNum;i++){
		if(inv[i] == -1 && s.VLabel[Node] == t.VLabel[i]){
			// Node --> i
			bool flag = true;
			for(int j=0;j<s.List[Node].size();j++){
				ListElem Node2 = s.List[Node][j];
				if(mapping[Node2.second] != -1){
					if(s.getELabel(Node, Node2.second) != t.getELabel(i, mapping[Node2.second])){
						flag = false;
						break;
					}
				}
			}
			if(flag){
				mapping[Node] = i;
				inv[i] = Node;
				if(SubgraphIsomorphicRecursion(cur+1, order, mapping, inv, s, t) == true)
					return true;
				mapping[Node] = -1;
				inv[i] = -1;
			}
		}
	}
	return false;
}

bool SubgraphIsomorphic(const Graph& s, const Graph& t){
	int mapping[VertexMAX] = {0,}, inv[VertexMAX] = {0,};
	for(int i=0;i<VertexMAX;i++){
		mapping[i] = -1;
		inv[i] = -1;
	}
	for(int i=0;i<Thres+1;i++){
		if(SubgraphIsomorphicRecursion(0, s.Partition[i], mapping, inv, s, t))
			return true;
	}
	return false;
}
Graph dataGraph[GraphMAX];
int main(){
	srand(time(NULL));
	int GraphCnt = 0;
	int QueryCnt = 0;
	int cnt = 0;
	FILE *fp = fopen("aids_4000.db","r");
	FILE *fp1 = fopen("aids_100.q","r");
	//FILE *fp2 = fopen("data.out","w");
	//FILE *fp3 = fopen("query.out","w");

	char temp;
	fscanf(fp,"%c",&temp);
	for(GraphCnt = 0;;GraphCnt++){
		fprintf(stderr, "processing %d\n",GraphCnt);
		int ter;
		ter = dataGraph[GraphCnt].Input(fp);
		if(ter == -1) break;
		//dataGraph[GraphCnt].build_Table(true);
		dataGraph[GraphCnt].make_partition();
	}

	fprintf(stderr, "Database load done\n");

	for(QueryCnt = 0;;QueryCnt++){
		fprintf(stderr, "Processing %d\n",QueryCnt);
		Graph queryGraph;
		int ter;
		ter = queryGraph.Input(fp1);
		if(ter == -1) break;
		//queryGraph.build_Table(false);
		for(int i=0;i<GraphCnt;i++){
			if(SubgraphIsomorphic(dataGraph[i], queryGraph))
				cnt++;
			//fprintf(stderr, "** common : %d with graph %d\n",x, i);
			//fprintf(stderr, "%d %d %d %d %d\n",queryGraph.VertexNum, Thres, get_power(queryGraph.maxDegree - 1, K-1), queryGraph.maxDegree-1, K-1);
		}
	}

	fclose(fp);
	fclose(fp1);
	//fclose(fp2);
	//fclose(fp3);

	printf("** result : %d ** \n",cnt);

	return 0;
}
