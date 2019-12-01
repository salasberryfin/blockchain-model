#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>

using namespace std;

string sha256(const string str);

struct Block {
    time_t Timestamp;
    string PreviousBlockHash;
    string RootHash;
    string Goal;
    string Nonce;
} ;

#endif
