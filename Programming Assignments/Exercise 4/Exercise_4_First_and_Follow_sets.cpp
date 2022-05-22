/*
Name: Corbin Ulloa
Student ID: 006973376
Class: CSE 5700 SPRING 2022
Programming Excerise 4: First and Follow sets
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class ReadGrammar
{
public :
void getGrammar(string filename);
bool is_term(char x);
bool is_nonterm(char x);
void first();
void printFirst();
void follows();
void printFollows();

private :
set<char> go_first(char x);
vector<char> terminals;
vector<char> nonterms;
vector<string> grammar;
vector<char> allPos;
vector< set<char> > inFirsts;
vector< set<char> > inFollows;
};

void ReadGrammar::getGrammar(string filename)
{
	string line;
	ifstream file (filename);
	if(!file.good()){
		cout<<"Error! File may not exist.\n";
		exit(1);
	}

	int count = 0;
	getline(file, line);
	if(count == 0){
		while(count != 1){
			terminals.push_back(line[0]);
			getline(file,line);
			if(line == "$"){
				count = 1;
			}
		}
	}
	getline(file, line);
	int index_nont = 0;
	while(line != "$"){
		grammar.push_back(line);
		if(index_nont > 0){
			if(nonterms[index_nont-1] != line[0]){
				nonterms.push_back(line[0]);
				index_nont++;
			}	
		}
		if(index_nont == 0){
			nonterms.push_back(line[0]);
			index_nont++;
		}
		getline(file, line);
	}
	allPos = terminals;
	for(int i=0; i < nonterms.size();i++){
		allPos.push_back(nonterms[i]);
	}
}

bool ReadGrammar::is_term(char x)
{
	for(int i = 0; i < terminals.size(); i++){
		if(x == terminals[i]){
			return true;
		}
	}
	return false;
}

//Checks to see if given character is a nonterminal
bool ReadGrammar::is_nonterm(char x)
{
	for(int i =0; i < nonterms.size();i++){
		if(x == nonterms[i]){
			return true;
		}
	}
	return false;
}
void ReadGrammar::first(){
	set<char> temp;
	int start = 0;
	int end = 0;
	for(int i = 0; i < allPos.size();i++){
		inFirsts.push_back(temp);
	}
	do{
		end = 0;
		start=0;
		for(int i = 0; i < inFirsts.size();i++){
			start+= inFirsts[i].size();
		}
		for(int i =0; i < allPos.size(); i++){
			temp = go_first(allPos[i]);
			inFirsts[i]=temp;
		}
		for(int i=0; i<inFirsts.size();i++){
			end+=inFirsts[i].size();
		}
	}
	while(start!=end);
}
set<char> ReadGrammar::go_first(char x){
	set<char> temp;
	string t;
	if(is_term(x)){//Rule 1
		temp.insert(x);
	}
	if(is_nonterm(x)){
		for(int i =0; i < grammar.size(); i++){
			t = grammar[i];
			if(t[0]==x){
				if(t[3] == 'e'){
					temp.insert(t[3]);
				}
				else if(is_term(t[3])){
					temp.insert(t[3]);
				}
				else if(is_nonterm(t[3])){
					for(int i = 0; i < allPos.size();i++){
						if(allPos[i] == t[3]){
							temp.insert(inFirsts[i].begin(), inFirsts[i].end());
						}
					}
				}
			}
		}
	}
	return temp;
}
void ReadGrammar::printFirst(){
	cout << "FIRSTS\n";
	for(int i = 0; i < inFirsts.size();i++){
		cout << allPos[i] << " = ";
		for(set<char>::iterator iter = inFirsts[i].begin();iter!=inFirsts[i].end();iter++){
			cout << *iter;
		}
		cout << endl;
	}
}
void ReadGrammar::follows()
{
	set<char> temp;
	for(int i = 0; i < nonterms.size();i++){
		inFollows.push_back(temp);
	}
	inFollows[0].insert('$');//Rule 1
	int index_nonterm;
	int index_firsts;

	for(int k=0; k<grammar.size();k++){
		for(int i=3; i <grammar[k].size()-1;i++){
			if(is_nonterm(grammar[k][i])){
				for(int j= 0; j < nonterms.size();j++){
					if(grammar[k][i]==nonterms[j]){
						index_nonterm = j; break;
					}
				}
				for(int j=0;j<allPos.size();j++){
					if(grammar[k][i+1]==allPos[j]){
						index_firsts = j; break;
					}
				}
				temp = inFirsts[index_firsts];
				inFollows[index_nonterm].insert(temp.begin(),temp.end());
				inFollows[index_nonterm].erase('e');
			}
		}
	}

	int index_follow;
	for(int k=0; k<grammar.size();k++){
		for(int i =0; k<grammar.size();k++){
			if(is_nonterm(grammar[k][grammar[k].size()-1])){
				for(int j= 0; j < nonterms.size();j++){
					if(grammar[k][i]==nonterms[j]){
						index_nonterm = j; break;
					}
				}
				for(int j=0; j<nonterms.size();j++){
					if(grammar[k][0]==nonterms[j]){
						index_follow=j; break;
					}
				}
				temp = inFollows[index_follow];
				inFollows[index_nonterm].insert(temp.begin(),temp.end());
			}
		}
	}
	bool found = false;
	for(int k =0; k<grammar.size();k++){
		for(int i =3; i <grammar[k].size();i++){ //-1
			if(is_nonterm(grammar[k][i])){				
				for(int j=terminals.size(); j<allPos.size();j++){
					if(grammar[k][i+1]==allPos[j]){
						for(set<char>::iterator itr = inFirsts[j].begin();itr!=inFirsts[j].end();itr++){
							if(*itr=='e') {
								/*index_nonterm = j-terminals.size();*/ found = true; break; 
							}
						}
					}
					if(found == true){
						for(int h= 0;h<nonterms.size();h++){
							if(nonterms[h]==grammar[k][i]) {index_nonterm=h; break;}
						}
						break;
					}
				}
				for(int j=0; j<nonterms.size();j++){
					if(grammar[k][0]==nonterms[j]){
						index_follow=j; break;
					}
				}
				temp = inFollows[index_follow];
				inFollows[index_nonterm].insert(temp.begin(),temp.end());
			}
		}
	}
}
//Function to output FOLLOWS
void ReadGrammar::printFollows(){
	cout << "FOLLOWS\n";
	for(int i = 0; i < inFollows.size();i++){
		cout << nonterms[i] << " = ";
		for(set<char>::iterator iter = inFollows[i].begin();iter!=inFollows[i].end();iter++){
			cout << *iter;
		}
		cout << endl;
	}
}

int main(){

	ReadGrammar lab2;
	lab2.getGrammar("g417");
    lab2.getGrammar("g419");
	lab2.first();
	lab2.printFirst();
	lab2.follows();
	lab2.printFollows();
	return 0;
}