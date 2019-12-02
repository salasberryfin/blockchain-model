#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Block {
    int Index;
    time_t Timestamp;
    string PreviousBlockHash;
    string Data;
} ;

string sha256(Block);
string printCurrentChain(vector<Block> *);
void httpServer(vector<Block> *);
bool isBlockValid(vector<Block> *, Block, string);

