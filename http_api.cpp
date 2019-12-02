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
        string hashedPreviousBlock = sha256((*chain).back());
        string userData = x["data"].s();
        std::ostringstream os;
        os << userData;
        if (isBlockValid(chain, (*chain).back(), userData)) {
            return crow::response{os.str()};
        } else {
            cout << "New block is invalid!\n";
            return crow::response(400);
        }
    });

    app.port(18080).multithreaded().run();
}
