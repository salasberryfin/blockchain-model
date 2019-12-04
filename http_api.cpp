#include "blockchain.h"
#include "crow_all.h"

void httpServer() {
    crow::SimpleApp app;

    // GET list current chain
    CROW_ROUTE(app, "/get_blockchain")([](){
        string resp = printCurrentChain();
        if (not resp.empty())
            return resp;
    });

    // POST create transaction
    CROW_ROUTE(app, "/new_transaction")
    .methods("POST"_method)
([](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x)
        return crow::response(400);
    struct Block previousBlock;
    previousBlock = getLastBlock();
    string unhashedPreviousBlock = to_string(previousBlock.Index) +
                                   previousBlock.Timestamp +
                                   previousBlock.PreviousBlockHash +
                                   to_string(previousBlock.TransactionId);
    try {
        int transId = previousBlock.TransactionId + 1;
        int blockId = previousBlock.Index + 1;
        string hashedPreviousBlock = sha256(unhashedPreviousBlock);
        Transaction trans = generateNewTransaction(transId, sha256(x["source"].s()), sha256(x["target"].s()), x["data"].s());
        Block block = generateNewBlock(blockId, to_string(time(0)), hashedPreviousBlock, trans.Index);
        if (not(isValidBlock(block, hashedPreviousBlock)))
            return crow::response(400);
        std::ostringstream os;
        os << trans.Data;
        if (addBlock(block, trans))
            return crow::response{os.str()};
        else
            return crow::response(400);
    } catch (const std::exception& e) {
        return crow::response(400);
    }
});

    //// POST add node to network
    //CROW_ROUTE(app, "/add_node")
    //    .methods("POST"_method)
    //([chain](const crow::request& req){
    //    auto x = crow::json::load(req.body);
    //    if (!x)
    //        return crow::response(400);
    //    try {
    //        struct Node newNode;
    //        newNode.HashedId = sha256(x["node_id"].s());
    //        std::ostringstream os;
    //        os << "New node ID: ";
    //        os << newNode.HashedId;
    //        return crow::response{os.str()};
    //    } catch (const std::exception& e) {
    //        return crow::response(400);
    //    }
    //});

    app.port(18080).multithreaded().run();
}
