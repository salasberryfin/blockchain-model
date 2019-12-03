#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/** 
 * should be stored in a local DB
 * I use an extern to simplify the process
**/
extern vector<string> nodes;

struct Transaction {
    string SourceId;
    string TargetId;
    string Data;
} ;

struct Block {
    int Index;
    time_t Timestamp;
    string PreviousBlockHash;
    Transaction TransData;
} ;

string sha256(Block);
string printCurrentChain(vector<Block> *);
void httpServer(vector<Block> *);
bool isBlockValid(vector<Block> *, Block, Transaction);

