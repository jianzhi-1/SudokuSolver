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
vector<pii> vsearch;

bool checkerror(){
	if (error1){
		printf("ERROR#1: Input sudoku is problematic i.e. identical numbers in same row OR column OR box.\n");
		return true;
	} else if (error2){
		return true;
	}
	return false;
}

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
	
	/*
	for (int k = 0; k < 9; k++){
		printf("BOX %d:\n", k);
		for (vector<pi>::iterator it = boxes[k].begin(); it != boxes[k].end(); it++){
			printf("%d %d\n", it -> first, it -> second);
		}
		printf("\n");
	}
	*/
	
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

int main(){
	init();
	input();
	if (checkerror()) return 0;
	while (sweep()){
		continue;
	}
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
