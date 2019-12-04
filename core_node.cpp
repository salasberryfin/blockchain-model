#include "blockchain.h"

/** 
 * should be stored in a local DB
 * I use an extern to simplify the process
**/
vector<string> initNodes() {
    vector<string> initializeNodes;
    initializeNodes.push_back("GenesisMasterNode");

    return initializeNodes;
}

/** 
 * initialize the nodes in the network
 * so I can keep track of the masters
 * that the new nodes connect to to 
 * retrieve the data (current chain, nodes, etc)
**/
vector<string> nodes = initNodes();

bool isValidTransaction(Transaction newTransaction) {
    for (vector<string>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        if (*i == newTransaction.SourceId)
            return true;
    }

    return false;
}

Block generateNewBlock(string hashedPrevious, Transaction transaction, int previousIndex) {
    struct Block block; 
    block.Index = previousIndex + 1;
    block.Timestamp = time(0);
    block.PreviousBlockHash = hashedPrevious;
    block.TransData = transaction;

    return block;
}

Block genesisBlock() {
    struct Block genesisBlock;
    struct Transaction genesisTransaction;
    const string sourceId = "GenesisMasterNode";
    const string targetId = "Broadcast";
    const string userData = "This is the Genesis transaction.";
    genesisTransaction.SourceId = sourceId;
    genesisTransaction.TargetId = targetId;
    genesisTransaction.Data = userData;
    genesisBlock.Index = 0;
    genesisBlock.Timestamp = time(0);
    genesisBlock.PreviousBlockHash = "";
    genesisBlock.TransData = genesisTransaction ;

    return genesisBlock;
}

string printCurrentChain(vector<Block> *chain) {
    string httpResponse = "";
    for (vector<Block>::iterator i = (*chain).begin(); i != (*chain).end(); ++i) {
        httpResponse.append("Block number " + to_string((*i).Index) + "\n");
        if ((*i).TransData.Data.empty())
            httpResponse.append("\tThis is the Genesis block!\n");
        httpResponse.append(("\tSource node: " + (*i).TransData.SourceId) + "\n");
        httpResponse.append(("\tTransaction data: " + (*i).TransData.Data) + "\n");
        httpResponse.append(("\tTimestamp: " + to_string((*i).Timestamp)) + "\n");
        httpResponse.append(("\tPrevious block: " + (*i).PreviousBlockHash) + "\n");
    }

    return httpResponse;
}

vector<Block> getCurrentChain() {
    vector<Block> chain;
    return chain;
}

void addToNodes(string sourceId) {
    nodes.push_back(sourceId);
}

void addToChain(vector<Block> *chain, Block block) {
    (*chain).push_back(block);
}

bool isBlockValid(vector<Block> *chain, Block previousBlock, Transaction newTransaction) {
    bool isValid = false;
    //const int MAX_DATA_LENGTH = 100;
    if (isValidTransaction(newTransaction)) {
        struct Block currentBlock = generateNewBlock(sha256(previousBlock), newTransaction, ((*chain).back()).Index);
        if (sha256(previousBlock) == currentBlock.PreviousBlockHash)
            isValid = true;
        else if (previousBlock.Index + 1 == currentBlock.Index)
            isValid = true;
        else if (not(currentBlock.TransData.Data).empty())
            isValid = true;

        if (isValid) {
            addToChain(chain, currentBlock);

            return true;
        }
    }

    return false;
}

bool isGenesisNode() {
    /**
     * get vector of nodes before from nodes[0]
     * the entry for the new node should be added
     * before running the program so that it is
     * already present in the vector
    **/
    if (nodes.size() == 1)
        return true;

    return false;
}

int main() {
    vector<Block> chain;
    if (isGenesisNode()) {
        Block unhashedGenesisBlock = genesisBlock();
        chain.insert(chain.begin(), unhashedGenesisBlock);
    } else
        chain = getCurrentChain();

    // rest API used for user interaction with the network
    httpServer(&chain);

    return 0;
}

