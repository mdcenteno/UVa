// uva10047.cpp
//
// The Monocycle
//

#include <cstdio>
#include <set>
#include <vector>
#include <queue>
#include <iostream>
#include <string>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

int L,R;
vvi maze;
int SI,SJ,TI,TJ;

struct MonoStatus {
	int I;
	int J;
	int DIR; //DIR: 0N, 1E, 2S, 3W
	int COLOR;	
	int TIME;
};

vector<MonoStatus> visited;

class Mono {
public:	
	MonoStatus status;		
	Mono();
	Mono(int,int,int,int,int);
	~Mono();
	bool blocked();
	void turn_left();
	void turn_right();
	void move();
	bool arrive();
	void print_status();
};

Mono::Mono() {
}
Mono::Mono(int i,int j,int color,int dir, int time) {	
	status.I=i; status.J=j; status.COLOR=color; status.DIR=dir; status.TIME=time;
}
Mono::~Mono() {
}
bool Mono::blocked() {
	if (status.DIR==0)		
		if (status.I-1<0 || maze[status.I-1][status.J]==1)		
			return true;			
	if (status.DIR==1)	
		if (status.J+1>=R || maze[status.I][status.J+1]==1)
			return true;	
	if (status.DIR==2)	
		if (status.I+1>=L || maze[status.I+1][status.J]==1)
			return true;			
	if (status.DIR==3)	
		if (status.J-1<0 || maze[status.I][status.J-1]==1)
			return true;	
	return false;
}
void Mono::turn_left() {	
	status.DIR--;
	if (status.DIR<0) 
		status.DIR=3;
	status.TIME++;
}
void Mono::turn_right() {	
	status.DIR++;
	if (status.DIR>3) 
		status.DIR=0;
	status.TIME++;
}
void Mono::move() {
	if (blocked())
		return;
	if (status.DIR==0)		
		status.I--;
	if (status.DIR==1)	
		status.J++;
	if (status.DIR==2)	
		status.I++;
	if (status.DIR==3)	
		status.J--;
	status.COLOR++;
	if (status.COLOR>4)	
		status.COLOR=0;	
	status.TIME++;
}
bool Mono::arrive() {
	return status.I==TI && status.J==TJ && status.COLOR==0;
}
void Mono::print_status() {
	cout << "I" << status.I << " J" << status.J << " C" << status.COLOR << " D" << status.DIR << endl;
}

void print_maze() {
	for (int i = 0; i < L; i++)
	{
		for (int j=0;j<R;j++)
			cout << maze[i][j];
		cout << endl;
	}
}
void set_target_position(int i,int j) {
	TI=i; TJ=j;
}
void set_start_position(int i, int j) {
	SI=i; SJ=j;
}
int translate(char c) {
	int out;
	switch (c)
	{
		case 'S':		
			out=2;
			break;
		case 'T':
			out=3;
			break;
		case '#':
			out=1;
			break;
		case '.':
			out=0;
			break;
		default:
			out=-1;
	}
	return out;
}
int acquire() {
	SI=SJ=TI=TJ=L=R=0;
	maze.clear();	
	cin >> L >> R;
	if(L==0&&R==0)
		return 0;
	maze.assign(L, vi()); // assign blank vectors of pair<int, int>s to AdjList
	for (int i = 0; i < L; i++) 
	{
		vi line;
		line.assign(R,0);
		string a;
		cin >> a;
		for (int j=0;j<R;j++)
		{
			line[j]=translate(a[j]);				
			if (line[j]==2)			
				set_start_position(i,j);
			if (line[j]==3)
				set_target_position(i,j);			
		}
		maze[i]=line;
		line.clear();
	}	
	//print_maze();
	//cout <<"SI"<<SI<<"SJ"<<SJ<<endl;
	//cout <<"TI"<<TI<<"TJ"<<TJ<<endl;
	
	return L;
}

bool is_visited(MonoStatus ms) {
	for (int i = 0; i < visited.size(); i++)
		if (ms.I==visited[i].I && ms.J==visited[i].J && ms.DIR==visited[i].DIR && ms.COLOR==visited[i].COLOR)
			return true;
	return false;
}
int navigator() {		
	visited.clear();
	queue<Mono> paths;
	Mono m(SI,SJ,0,0,0);		
	paths.push(m);
	visited.push_back(m.status);
	while (!paths.empty())
	{		
		m=paths.front();
		paths.pop();
		
		//m.print_status();
		
		if(m.arrive())
			return m.status.TIME;
					
		Mono ma = m;
		ma.move();
		if (!is_visited(ma.status))
		{
			paths.push(ma);
			visited.push_back(ma.status);
		}
		
		ma = m;
		ma.turn_left();
		if (!is_visited(ma.status))
		{
			paths.push(ma);
			visited.push_back(ma.status);
		}

		ma = m;
		ma.turn_right();
		if (!is_visited(ma.status))
		{
			paths.push(ma);
			visited.push_back(ma.status);
		}
	}
	return -1;
}

void output(int t) {
	static int count=0;
	if (count++)
		cout << endl;
	cout << "Case #" <<count<<endl;
	if (t==-1)	
		cout << "destination not reachable" << endl;	
	else
		cout << "minimum time = " << t << " sec"<< endl;	
}

int main() {	
	while(acquire())		
		output(navigator());		
	return 0;
}
