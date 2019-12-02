#include "blockchain.h"
#include <thread>

Block generateNewBlock(string hashedPrevious, string userData, int previousIndex) {
    struct Block block; 
    block.Index = previousIndex + 1;
    block.Timestamp = time(0);
    block.PreviousBlockHash = hashedPrevious;
    block.Data = userData;

    return block;
}

Block genesisBlock() {
    struct Block genesisBlock;
    genesisBlock.Index = 0;
    genesisBlock.Timestamp = time(0);
    genesisBlock.PreviousBlockHash = "";
    genesisBlock.Data = "";

    return genesisBlock;
}

void printCurrentChain(vector<Block> *chain) {
    for (vector<Block>::iterator i = (*chain).begin(); i != (*chain).end(); ++i) {
        cout << "Block number " <<(*i).Index << "\n";
        if ((*i).Data.empty())
            cout << "\tThis is the Genesis block!\n";
        else
            cout << "\tTransaction data: " <<(*i).Data << "\n";
    }
}

bool isBlockValid(vector<Block> *chain, Block previousBlock, Block currentBlock) {
    if (sha256(previousBlock) == currentBlock.PreviousBlockHash)
        return true;
    else if (previousBlock.Index + 1 == currentBlock.Index)
        return true;
    else if (not(currentBlock.Data).empty())
        return true;

    return false;
}

void addToChain(vector<Block> *chain, Block block) {
    (*chain).push_back(block);
}

int main() {
    vector<Block> chain;
    struct Block unhashedGenesisBlock = genesisBlock();
    chain.insert(chain.begin(), unhashedGenesisBlock);
    string hashedGenesisBlock = sha256(unhashedGenesisBlock);
    int previousIndex;
    string hashedPrevious;
    previousIndex = unhashedGenesisBlock.Index;
    hashedPrevious = hashedGenesisBlock;

    // rest API used for user interaction with node
    thread http_server (httpServer);

    while (true) {
        cout << "Ready to process next transaction\n";
        cout << "Insert the data for the new transaction (insert LIST to show the current status of the chain): ";
        string userData;
        getline (cin, userData);
        if (userData == "LIST") {
            printCurrentChain(&chain);
        } else {
            struct Block newBlock = generateNewBlock(hashedPrevious, userData, previousIndex);
            struct Block previousBlock = unhashedGenesisBlock;
            if (isBlockValid(&chain, previousBlock, newBlock)) {
                addToChain(&chain, newBlock);
                previousIndex = newBlock.Index;
                hashedPrevious = sha256(newBlock);
            } else {
                cout << "New block is invalid!\n";
            }
        }
    }

    return 0;
}

