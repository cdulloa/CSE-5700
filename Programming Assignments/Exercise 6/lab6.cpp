/*
Name: Corbin Ulloa
Student ID: 006973376
Class: CSE 5700 SPRING 2022
Programming Excerise 6: Grammars
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set> 


using namespace std;

class extendedProduction
{
public:
    char lhs;
    string rhs;
    extendedProduction(char _lhs, string _rhs) : lhs(_lhs), rhs(_rhs) {}
};

class singleRP
{
private: vector <extendedProduction*> productions;
public:
    map<int, char> gotos;
    singleRP() {}
    void push(extendedProduction* p) { productions.push_back(p); }
    int Size() { return int(productions.size()); }
    bool Contains(string str) {
        for (auto it = productions.begin(); it != productions.end(); it++)
        {
            string existing = string(&(*it)->lhs, 1) + "->" + (*it)->rhs;
            if (strcmp(str.c_str(), existing.c_str()) == 0)
                return true;
        }
        return false;
    }
    extendedProduction* operator[](const int index) { return productions[index]; }
};


typedef map<char, vector<string> > extendedGrammar;
typedef map<string, int> gotoMap;
bool isTerminal(char, vector<char>);
void getSingleRPs(vector<singleRP>&, extendedGrammar&, int&, gotoMap&);
void add_closure(char lookahead, singleRP& item, extendedGrammar& grammar);

int main()
{
    string file = "grammar3.txt"; //input file
    vector<char> terminal;
    vector<char> nonTerminal;
    map<char, string> production;
    map<char, set<char> > firstMap;
    map<char, set<char> > followMap;
    int itemid = -1;
    extendedGrammar AugGrammar;
    vector<singleRP> singleRPs = { singleRP() };
    gotoMap globalGoto;

    string line;
    string lhs, rhs;
    string delim = "->";

    fstream input;
    input.open(file.c_str(), ios::in);
    if (!input)
    {
        cout << "File not found" << endl;
        return 9;
    }

    cout << "LR(0) tables for grammars: Strings of 0's and 1's of the form xx"  << endl;

    getline(input, line);
    while (!input.eof())
    {
        char value = line[0];
        if (islower(value) || isalpha(value))
        {
            terminal.push_back(value);
        }
        if (isupper(value))
        {
            AugGrammar['\''].push_back(value + "");
            string temp = ".";
            temp.push_back(value);
            singleRPs[0].push(new extendedProduction('\'', temp));
            nonTerminal.push_back(value);
        }
        else if (!(find(nonTerminal.begin(), nonTerminal.end(), value) != nonTerminal.end()))
        {
            nonTerminal.push_back(value);
        }
        auto pos = line.find(delim);
        rhs = line.substr(pos + 2, std::string::npos);


        if (production[value] == "")
        {
            if (line.length() > 3) {
                production[value] = line.substr(3, line.length());
            }
            AugGrammar[value].push_back(rhs);
            cout << value << rhs << endl;
            if (!isTerminal(rhs[0], terminal))
            {
                singleRPs[0].push(new extendedProduction(value, "." + rhs));
            }
        }
        else
        {
            production[value] += "|" + line.substr(3, line.length());
            AugGrammar[value].push_back(rhs);
            printf("%c->%s\n", value, rhs.c_str());
            if (isTerminal(rhs[0], terminal) == false)
            {
                singleRPs[0].push(new extendedProduction(value, "." + rhs));
            }
        }
        getline(input, line);
    }
    cout << "Closures and gotos " << endl;
    while (++itemid < int(singleRPs.size()))
    {
        getSingleRPs(singleRPs, AugGrammar, itemid, globalGoto);
    }

}

bool isTerminal(char input, vector<char> terminal)
{
    if (input == 'e')
        return true;
    if (find(terminal.begin(), terminal.end(), input) != terminal.end())
        return true;
    else
        return false;
}

void getSingleRPs(vector<singleRP>& singleRPs, extendedGrammar& grammar, int& itemid, gotoMap& GlobalGoTo)
{
    printf("I%d:\n", itemid);

    for (int i = 0; i < singleRPs[itemid].Size(); i++)
    {
        string rhs = singleRPs[itemid][i]->rhs;
        char lookahead = rhs[rhs.find('.') + 1];
        add_closure(lookahead, singleRPs[itemid], grammar);
    }

    char lookahead;
    char lhs;
    string rhs;

    for (int i = 0; i < singleRPs[itemid].Size(); i++)
    {
        lhs = singleRPs[itemid][i]->lhs;
        rhs = singleRPs[itemid][i]->rhs;
        string production = string(&lhs, 1) + "->" + rhs;

        lookahead = rhs[rhs.find('.') + 1];
        if (lookahead == '\0')
        {
            printf("\t%-20s\n", &production[0]);
            continue;
        }

        if (singleRPs[itemid].gotos.find(lookahead) == singleRPs[itemid].gotos.end())
        {
            if (GlobalGoTo.find(production) == GlobalGoTo.end())
            {
                singleRPs.push_back(singleRP());
                string newRhs = rhs;
                int atpos = newRhs.find('.');
                swap(newRhs[atpos], newRhs[atpos + 1]);
                singleRPs.back().push(new extendedProduction(lhs, newRhs));
                singleRPs[itemid].gotos[lookahead] = char(singleRPs.size() - 1); 
                GlobalGoTo[production] = singleRPs.size() - 1;
            }
            else
            {
                singleRPs[itemid].gotos[lookahead] = GlobalGoTo[production];
            }
            printf("\t%-20s goto(I%d,%c)\n", &production[0], GlobalGoTo[production], lookahead);
        }
        else
        {
            int index = rhs.find('.');
            swap(rhs[index], rhs[index + 1]);
            int nextItem = singleRPs[itemid].gotos[lookahead];
            if (!singleRPs[nextItem].Contains(string(&lhs, 1) + "->" + rhs))
                singleRPs[nextItem].push(new extendedProduction(lhs, rhs));
            swap(rhs[index], rhs[index + 1]);
            printf("\t%-20s\n", &production[0]);
        }
    }
}
void add_closure(char lookahead, singleRP& item, extendedGrammar& grammar)
{
    if (!isupper(lookahead))
    {
        return;
    }

    string lhs = string(&lookahead, 1);

    for (int i = 0; i < int(grammar[lookahead].size()); i++)    
    {
        string rhs = "." + grammar[lookahead][i];
        if (!item.Contains(lhs + "->" + rhs))
            item.push(new extendedProduction(lookahead, rhs));
    }
}