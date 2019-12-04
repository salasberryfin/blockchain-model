#include "blockchain.h"
#include "crow_all.h"

void httpServer(vector<Block> *chain) {
    crow::SimpleApp app;

    // GET list current chain
    CROW_ROUTE(app, "/get_blockchain")([chain](){
        string resp = printCurrentChain(chain);
        if (not resp.empty())
            return resp;
    });

    // POST create transaction
    CROW_ROUTE(app, "/new_transaction")
        .methods("POST"_method)
    ([chain](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        struct Block previousBlock;
        previousBlock = (*chain).back();
        string unhashedPreviousBlock = to_string(previousBlock.Index) +
                                       to_string(previousBlock.Timestamp) +
                                       previousBlock.PreviousBlockHash +
                                       previousBlock.TransData.Data;
        string hashedPreviousBlock = sha256(unhashedPreviousBlock);
        try {
            struct Transaction newTransaction;
            newTransaction.Data = x["data"].s();
            newTransaction.SourceId = x["source"].s();
            newTransaction.TargetId = x["target"].s();
            std::ostringstream os;
            os << newTransaction.Data;
            if (isBlockValid(chain, previousBlock, newTransaction)) {
                return crow::response{os.str()};
            } else {
                cout << "New block is invalid!\n";
                return crow::response(400);
            }
        } catch (const std::exception& e) {
            return crow::response(400);
        }
    });

    // POST add node to network
    CROW_ROUTE(app, "/add_node")
        .methods("POST"_method)
    ([chain](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        try {
            struct Node newNode;
            newNode.HashedId = sha256(x["node_id"].s());
            std::ostringstream os;
            os << "New node ID: ";
            os << newNode.HashedId;
            return crow::response{os.str()};
        } catch (const std::exception& e) {
            return crow::response(400);
        }
    });

    app.port(18080).multithreaded().run();
}
