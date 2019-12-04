#include "blockchain.h"

bool addTransaction(Transaction trans) {
    bool result = addNewTransaction(trans);

    return result;
}

bool addBlock(Block block, Transaction trans) {
    // add to blockchain_db.blockchain and blockchain_db.transactions
    bool result = false;
    if (addTransaction(trans)) {
        cout << "Transaction was added\n";
        result = addNewBlock(block);
        if (result)
            cout << "Block was added\n";
    }
    else {
        cout << "If you're waiting for the Genesis block, it's already there!\n";
        cout << "If you're adding a new transaction -> something went wrong...\n";
        return result;
    }

    return result;
}

Transaction generateNewTransaction(int id, string sourceId, string targetId, string userData) {
    Transaction trans;
    trans.Index = id;
    trans.SourceId = sourceId;
    trans.TargetId = targetId;
    trans.Data = userData;

    return trans;
}

bool isValidBlock(Block block, string previousHash) {
    if (block.PreviousBlockHash != previousHash)
        return false;

    return true;
}

Block generateNewBlock(int index, string timestamp, string previousBlockHash, int transactionId) {
    Block block;
    block.Index = index;
    block.Timestamp = timestamp;
    block.PreviousBlockHash = previousBlockHash;
    block.TransactionId = transactionId;

    return block;
}

int main() {
    Transaction genesisTransaction = generateNewTransaction(0, sha256("MasterNode"), sha256("Broadcast"), "This is the Genesis Transaction.");
    Block unhashedGenesisBlock = generateNewBlock(0, to_string(time(0)), "", genesisTransaction.Index);
    addBlock(unhashedGenesisBlock, genesisTransaction);

    // rest API used for user interaction with the network
    httpServer();

    return 0;
}

