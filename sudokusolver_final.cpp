#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
using namespace std;
typedef pair<int,int> pi;
typedef pair<char, pair<pi, int> > pii;

int sudoku[9][9];
set<int> possudoku[9][9]; //stores the possible values of the cell
vector<pi> boxes[9];
bool error1 = false; //input sudoku problematics
bool error2 = false;

//Heuristic vector vsearch
vector<pii> vsearch;
//<'G', <pos, val> > --- Guess position pos has value val
//<'D', <pos, val> > --- Delete possible value val from position pos
//<'C', <pos, val> > --- removal of possible value val from position pos as a Consequence of a guess

bool checkerror(){
	if (error1){
		printf("ERROR#1: Input sudoku is problematic i.e. identical numbers in same row OR column OR box.\n");
		return true;
	} else if (error2){
		return true;
	}
	return false;
}

//INITIALISE SUDOKU
void init(){
	memset(sudoku, 0, sizeof(sudoku));
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			while (!possudoku[i][j].empty()){
				possudoku[i][j].erase(possudoku[i][j].begin());
			}
			possudoku[i][j].clear();
			for (int k = 0; k < 9; k++){
				possudoku[i][j].insert(k + 1);
			}
		}
	}
	
	//setting up each 3 by 3 box
	for (int k = 0; k < 9; k++){
		int w = k%3;
		int ww = (k - (k%3))/3;
		for (int i = ww*3; i < ww*3+3; i++){
			for (int j = w*3; j < w*3+3; j++){
				boxes[k].push_back(make_pair(i, j));
			}
		}
	}
	
	vsearch.clear();

}

void confirm(pi pos, int val){
	if (possudoku[pos.first][pos.second].find(val) == possudoku[pos.first][pos.second].end()){
		error1 = true;
		return;
	}
	while (!possudoku[pos.first][pos.second].empty()){
		possudoku[pos.first][pos.second].erase(possudoku[pos.first][pos.second].begin());
	}
	possudoku[pos.first][pos.second].insert(val);
	
	//delete row
	for (int i = 0; i < 9; i++){
		if (possudoku[pos.first][i].find(val) == possudoku[pos.first][i].end()) continue;
		if (i == pos.second) continue;
		possudoku[pos.first][i].erase(val);
	}
	
	//delete column
	for (int i = 0; i < 9; i++){
		if (possudoku[i][pos.second].find(val) == possudoku[i][pos.second].end()) continue;
		if (i == pos.first) continue;
		possudoku[i][pos.second].erase(val);
	} 
	
	//delete box
	int boxx = (pos.first/3)*3 + pos.second/3;
	
	for (int i = 0; i < boxes[boxx].size(); i++){
		pi cur = boxes[boxx][i];
		if (cur == pos) continue;
		if (possudoku[cur.first][cur.second].find(val) == possudoku[cur.first][cur.second].end()) continue;
		possudoku[cur.first][cur.second].erase(val);
	}
}

//input()
//handles the input in the form of 9*9 grid, with ? denoting cells with unknown value
void input(){
	char c;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			scanf(" %c", &c);
			if (c == '?'){
				sudoku[i][j] = 0;
			} else {
				sudoku[i][j] = (int)c - 48;
				confirm(make_pair(i, j), sudoku[i][j]);
				if(checkerror()) return;
				
			}
		}
	}
}

//checkvalid() 
//TRUE: current sudoku is valid
//FALSE: current sudoku not valid, a cell has 0 possible value
bool checkvalid(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (possudoku[i][j].size() == 0) return false;
		}
	}
	return true;
}

//checkcomplete() 
//TRUE: sudoku is completed and has a valid solution
//FALSE: sudoku not yet completed OR not valid
bool checkcomplete(){
	if (!checkvalid()) return false;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (possudoku[i][j].size() != 1) return false;
		}
	}
	set<int> ss;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			ss.insert(*possudoku[i][j].begin());
		}
		if (ss.size() != 9) return false;
		while (!ss.empty()){
			ss.erase(ss.begin());
		}
	}
	for (int j = 0; j < 9; j++){
		for (int i = 0; i < 9; i++){
			ss.insert(*possudoku[i][j].begin());
		}
		if (ss.size() != 9) return false;
		while (!ss.empty()){
			ss.erase(ss.begin());
		}
	}
	for (int k = 0; k < 9; k++){
		for (vector<pi>::iterator it = boxes[k].begin(); it != boxes[k].end(); it++){
			ss.insert(*possudoku[it -> first][it -> second].begin());
		}
		if (ss.size() != 9) return false;
		while (!ss.empty()){
			ss.erase(ss.begin());
		}
	}
	return true;
}

bool sweep(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (possudoku[i][j].size() == 1 && sudoku[i][j] == 0){
				sudoku[i][j] = *possudoku[i][j].begin();
				confirm(make_pair(i, j), sudoku[i][j]);
				return true;
			}
		}
	}
	return false;
}

void cconfirm(pi pos, int val){
	int x = pos.first, y = pos.second;
	if (possudoku[x][y].find(val) == possudoku[x][y].end()){
		error1 = true;
		return;
	}
	while (!possudoku[x][y].empty()){
		if (*possudoku[x][y].begin() != val){
			vsearch.push_back(make_pair('C', make_pair(pos, *possudoku[x][y].begin())));
		}
		possudoku[x][y].erase(possudoku[x][y].begin());
	}
	possudoku[x][y].insert(val);
	
	//delete row
	for (int i = 0; i < 9; i++){
		if (possudoku[x][i].find(val) == possudoku[x][i].end()) continue;
		if (i == y) continue;
		vsearch.push_back(make_pair('C', make_pair(make_pair(x, i), val)));
		possudoku[x][i].erase(val);
	}
	
	//delete column
	for (int i = 0; i < 9; i++){
		if (possudoku[i][y].find(val) == possudoku[i][y].end()) continue;
		if (i == x) continue;
		vsearch.push_back(make_pair('C', make_pair(make_pair(i, y), val)));
		possudoku[i][y].erase(val);
	} 
	
	//delete box
	int boxx = (x/3)*3 + y/3;
	
	for (int i = 0; i < boxes[boxx].size(); i++){
		pi cur = boxes[boxx][i];
		if (cur == pos) continue;
		if (possudoku[cur.first][cur.second].find(val) == possudoku[cur.first][cur.second].end()) continue;
		vsearch.push_back(make_pair('C', make_pair(cur, val)));
		possudoku[cur.first][cur.second].erase(val);
	}
}

//csweep
//consequential sweep
bool csweep(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (possudoku[i][j].size() == 1 && sudoku[i][j] == 0){
				sudoku[i][j] = *possudoku[i][j].begin();
				vsearch.push_back(make_pair('W', make_pair(make_pair(i, j), sudoku[i][j])));
				cconfirm(make_pair(i, j), sudoku[i][j]);
				return true;
			}
		}
	}
	return false;
}

void solved(){
	printf("SOLVED:\n");
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (sudoku[i][j] != 0){
				printf("%d ", sudoku[i][j]);
			} else {
				printf("?");
			}
		}
		printf("\n");
	}
}


int main(){
	init();
	input();
	printf("\n");
	if (checkerror()) return 0;
	while (sweep()){
		continue;
	}
	if (checkcomplete()){
		solved();
		return 0;
	}
	if (!checkvalid()){
		printf("Unsolvable!\n");
		return 0;
	}
	//heuristic
	while (!checkcomplete()){
		if (vsearch.size() == 0){
			while (sweep()){
				continue;
			}
		}
		//puzzle should be valid at this point
		//beginning a guess
			
		//find the position with the smallest possibilities --> check from there
		pi pos;
		int val = 1000;
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				if (sudoku[i][j] != 0) continue;
				if (possudoku[i][j].size() < val){
					val = possudoku[i][j].size();
					pos = make_pair(i, j);
				}
			}
		}
		int x = pos.first, y = pos.second;
		
		int vall = *possudoku[x][y].begin();
		vsearch.push_back(make_pair('G', make_pair(pos, vall)));
		while (!possudoku[x][y].empty()){
			if (*possudoku[x][y].begin() != vall){
				//deleting the rest of the options is a consequence of the guess
				vsearch.push_back(make_pair('C', make_pair(pos, *possudoku[x][y].begin())));
			}
			possudoku[x][y].erase(possudoku[x][y].begin());
		}
		possudoku[x][y].insert(vall);
		while (csweep()){
			continue;
		}
		while (!checkvalid()){
			//NOT VALID --> THIS GUESS IS WRONG --> BACKTRACK
			pii curstep = vsearch.back();
			while (curstep.first != 'G'){
				int x = curstep.second.first.first;
				int y = curstep.second.first.second;
				int vall = curstep.second.second;
				if (curstep.first == 'C'){
					possudoku[x][y].insert(vall);
				} else if (curstep.first == 'W'){
					sudoku[x][y] = 0;
				}
				vsearch.pop_back();
				curstep = vsearch.back();
			}
			
			curstep = vsearch.back();
			int x = curstep.second.first.first;
			int y = curstep.second.first.second;
			int vall = curstep.second.second;
			if (vsearch.size() == 1){
				//means initial guess
				possudoku[x][y].erase(vall);
				vsearch.pop_back();
				while (sweep()){
					continue;
				}
			} else {
				possudoku[x][y].erase(vall);
				vsearch.pop_back();
				vsearch.push_back(make_pair('C', make_pair(make_pair(x,y), vall)));
				//because this guess is a consequence of a previous guess
				while (csweep()){
					continue;
				}
			}

		}
			
		if (checkcomplete()) break;
	}
	
	if (checkcomplete()){
		solved();
		return 0;
	} else {
		printf("UNSOLVABLE!\n");
	}
	
}
