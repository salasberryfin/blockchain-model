#include "blockchain.h"

Block generateNewBlock(string hashedPrevious) {
    struct Block block; 
    block.PreviousBlockHash = hashedPrevious;
    block.Timestamp = time(0);

    return block;
}

int main() {
    const string myName = "previous_block";
    string hashedPrevious = sha256(myName);

    struct Block newBlock = generateNewBlock(hashedPrevious); 
    cout << "Hashed string is: " << newBlock.PreviousBlockHash;
    cout << "Timestamp: " << newBlock.Timestamp;

    return 0;
}
