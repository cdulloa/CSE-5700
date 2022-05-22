/*
Name: Corbin Ulloa
Student ID: 006973376
Class: CSE 5700 SPRING 2022
Programming Excerise 1: finite automata
*******/

#include <iostream>
#include <string>

using namespace std;

int DFA(const int [4][2]);
int check_state(char);
void reject(char);
int change_sate(int, char, const int [4][2]);
bool verify(const int [1], const int size, int);

// Checks to see if input string is valid or invalid
// Valid characters are 'a' and 'b'
int check_state(char c)
{
    switch (c)
    {
    case 'a':
        return 0;
    case 'b':
        return 1;
    default:
        return -1;
    }
}

// If an invalid character is found, print a message and terminate the program
void reject(char c) 
{
      cout << " String rejected" << endl;

    exit(1);
}

// Change the state of the DFA and returns new state
int change_state(int s, char c, const int table[4][2])
{
    int next_state = check_state(c);
    // If the next state is valid, update the state of DFA using the table
    if (next_state != -1)
        s = table[s][next_state];
    else
        reject(c);
    
    return s;
}

// Simulate the DFA
int DFA(const int table [4][2])
{
    int s = 0; // Initial state of the DFA

    cout << "Enter a string from the following alphabet: (a|b)*abb" << endl << endl;
    cout << "Enter: ";
    char c = getchar();

    // Read each character of the input string. Stop when the terminating character is reached
    while(c != '\n')
    {
        s = change_state(s, c, table); 
        c = getchar(); 
    } 

    return s;
}

// Check if the DFA is in an acceptable halt state
bool verify(const int end [1], const int size, int s)
{
    // Check if the last state of the DFA is one of the accepted end states
    for (int i = 0; i < size; i++)
        if(end[i] == s)
            return true;

    return false;
}

int main() 
{
    // Tranisition table for the DFA
    const int tranistion_table [4][2] = {
        {1, 0},
        {1, 2},
        {1, 3},
        {1, 0}
    };

    const int end_states [1] = {3}; 
    const int end_states_size = 1; 

    // Simulate the DFA
    const int state = DFA(tranistion_table);

    if (verify(end_states, end_states_size, state))
        cout << "String accepted" << endl;
    else
        cout << "String rejected" << endl;

    return 0;
}


