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

map<string, string> initMasterNodes() {
    map<string, string> initializeMasterNodes;
    initializeMasterNodes.insert(pair<string, string>(sha256("GenesisMasterNode"), "0.0.0.0:18080"));

    return initializeMasterNodes;
}

map<string, string> masterNodes = initMasterNodes();

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
    const int id = 1;
    const string sourceId = sha256("GenesisMasterNode");
    const string targetId = "Broadcast";
    const string userData = "This is the Genesis transaction.";
    genesisTransaction.Index = id;
    genesisTransaction.SourceId = sourceId;
    genesisTransaction.TargetId = targetId;
    genesisTransaction.Data = userData;
    genesisBlock.Index = 0;
    genesisBlock.Timestamp = time(0);
    genesisBlock.PreviousBlockHash = "";
    genesisBlock.TransData = genesisTransaction ;

    return genesisBlock;
}

string printCurrentChain() {
    string httpResponse = "";
    //for (vector<Block>::iterator i = (*chain).begin(); i != (*chain).end(); ++i) {
    //    httpResponse.append("Block number " + to_string((*i).Index) + "\n");
    //    if ((*i).TransData.Data.empty())
    //        httpResponse.append("\tThis is the Genesis block!\n");
    //    httpResponse.append(("\tSource node: " + (*i).TransData.SourceId) + "\n");
    //    httpResponse.append(("\tTransaction data: " + (*i).TransData.Data) + "\n");
    //    httpResponse.append(("\tTimestamp: " + to_string((*i).Timestamp)) + "\n");
    //    httpResponse.append(("\tPrevious block: " + (*i).PreviousBlockHash) + "\n");
    //}

    return httpResponse;
}

bool addTransaction(Transaction trans) {
    bool result = addNewTransaction(trans);

    return result;
}

bool addBlock(Block block) {
    // add to blockchain_db.blockchain and blockchain_db.transactions
    bool result = false;
    if (addTransaction(block.TransData)) {
        cout << "Transaction was added\n";
        result = addNewBlock(block, block.TransData.Index);
    }
    else
        cout << "Something went wrong...\n";

    return result;
}

bool isBlockValid(vector<Block> *chain, Block previousBlock, Transaction newTransaction) {
    const int MAX_DATA_LENGTH = 100;
    if (isValidTransaction(newTransaction)) {
        string unhashedPreviousBlock = to_string(previousBlock.Index) +
                                       to_string(previousBlock.Timestamp) +
                                       previousBlock.PreviousBlockHash +
                                       previousBlock.TransData.Data;
        string unhashedExpectedBlock = to_string((*chain).back().Index) +
                                       to_string((*chain).back().Timestamp) +
                                       (*chain).back().PreviousBlockHash +
                                       (*chain).back().TransData.Data;
        struct Block currentBlock = generateNewBlock(sha256(unhashedPreviousBlock), newTransaction, ((*chain).back()).Index);
        if (sha256(unhashedExpectedBlock) != currentBlock.PreviousBlockHash)
            return false;
        else if (previousBlock.Index + 1 != currentBlock.Index)
            return false;
        else if ((currentBlock.TransData.Data).empty() && sizeof(currentBlock.TransData.Data) >= MAX_DATA_LENGTH)
            return false;
        addBlock(currentBlock);

        return true;
    }

    return false;
}

bool isGenesisNode() {
    if (masterNodes.size() == 1)
        return true;

    return false;
}

int main() {
    if (isGenesisNode()) {
        Block unhashedGenesisBlock = genesisBlock();
        addBlock(unhashedGenesisBlock);
    } else
        cout << "New node\n";

    // rest API used for user interaction with the network
    httpServer();

    return 0;
}

