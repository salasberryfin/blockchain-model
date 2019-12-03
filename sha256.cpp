#include "blockchain.h"
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

string sha256(Block block) {
    string unhashedBlock = to_string(block.Index) +
                           to_string(block.Timestamp) +
                           block.PreviousBlockHash +
                           block.TransData.Data;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, unhashedBlock.c_str(), unhashedBlock.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}
