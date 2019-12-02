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

string printCurrentChain(vector<Block> *chain) {
    string httpResponse = "";
    for (vector<Block>::iterator i = (*chain).begin(); i != (*chain).end(); ++i) {
        httpResponse.append("Block number " + to_string((*i).Index) + "\n");
        if ((*i).Data.empty())
            httpResponse.append("\tThis is the Genesis block!\n");
        httpResponse.append(("\tTransaction data: " + (*i).Data) + "\n");
        httpResponse.append(("\tTimestamp: " + to_string((*i).Timestamp)) + "\n");
        httpResponse.append(("\tPrevious block: " + (*i).PreviousBlockHash) + "\n");
    }

    return httpResponse;
}

void addToChain(vector<Block> *chain, Block block) {
    (*chain).push_back(block);
}

bool isBlockValid(vector<Block> *chain, Block previousBlock, string userData) {
    bool isValid = false;
    const int MAX_DATA_LENGTH = 100;
    struct Block currentBlock = generateNewBlock(sha256(previousBlock), userData, ((*chain).back()).Index);
    if (sha256(previousBlock) == currentBlock.PreviousBlockHash)
        isValid = true;
    else if (previousBlock.Index + 1 == currentBlock.Index)
        isValid = true;
    else if (not(currentBlock.Data).empty())
        isValid = true;

    if (isValid) {
        addToChain(chain, currentBlock);

        return true;
    }

    return false;
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
    httpServer(&chain);

    return 0;
}

