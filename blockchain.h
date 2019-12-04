#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

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

struct TcpConnections {
    string Outbound[3];
    string Inbound[3];
} ;

struct Node {
    string HashedId;
    TcpConnections Connections;     
    int SelfPort;
    //string SelfIp;
} ;

string sha256(string);
string printCurrentChain(vector<Block> *);
void httpServer(vector<Block> *);
bool isBlockValid(vector<Block> *, Block, Transaction);

