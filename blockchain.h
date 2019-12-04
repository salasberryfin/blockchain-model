#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Transaction {
    int Index;
    string SourceId;
    string TargetId;
    string Data;
} ;

struct Block {
    int Index;
    string Timestamp;
    string PreviousBlockHash;
    int TransactionId;
} ;

string sha256(string);
void httpServer();
// mysql handler
bool addNewTransaction(Transaction);
bool addNewBlock(Block);
Block getLastBlock();
Transaction generateNewTransaction(int, string, string, string);
Block generateNewBlock(int, string, string, int);
bool addBlock(Block, Transaction);
bool isValidBlock(Block, string);
string printCurrentChain();

