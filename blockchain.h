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
void httpServer();

