#include "blockchain.h"
#include <stdlib.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define DBNAME "blockchain_db"
#define HOST "tcp://127.0.0.1:3306"
#define DBUSER "blockchain"
#define DBPWD "password"

sql::Connection* createConn(string sqlQuery) {
    sql::Driver *driver;
    sql::Connection *con;

    driver = get_driver_instance();
    con = driver->connect(HOST, DBUSER, DBPWD);
    con->setSchema(DBNAME);

    return con;
}

bool addNewTransaction(Transaction trans) {
    const string tableName = "transactions";
    string transValues = "( '" + to_string(trans.Index) +
                         "', '" + trans.SourceId +
                         "', '" + trans.TargetId +
                         "', '" + trans.Data + "' )";
    string sqlQuery = "INSERT INTO " + tableName + 
                      " (id, source_id, target_id, data) " +
                      "VALUES " + transValues + ";";
    cout << sqlQuery;
    sql::Connection* con = createConn(sqlQuery);
    sql::Statement *stmt;
    bool sqlResult;
    try {
        stmt = con->createStatement();
        sqlResult = stmt->execute(sqlQuery);
	} catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        return false;
    }
    delete stmt;
    delete con;

    return true;
}

bool addNewBlock(Block block, int transId) {
    const string tableName = "blockchain";
    string blockValues = "( '" + to_string(block.Timestamp) +
                         "', '" + block.PreviousBlockHash +
                         "', '" + to_string(transId) + "' )";
    string sqlQuery = "INSERT INTO " + tableName + 
                      " (timestamp, previous_block_hash, transaction_id) " +
                      "VALUES " + blockValues + ";";
    cout << "Add new block query\n " << sqlQuery << "\n";
    sql::Connection* con = createConn(sqlQuery);
    sql::Statement *stmt;
    bool sqlResult;
    try {
        stmt = con->createStatement();
        sqlResult = stmt->execute(sqlQuery);
	} catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        return false;
    }
    delete stmt;
    delete con;

    return true;
}

bool readCurrentChain() {
    const string tableName = "blockchain";
    //sqlConn = createConn();
    //
    //try {
    //    while (sqlRes->next()) {
    //        cout << "Data: " << sqlRes->getString("data") << endl;
    //        cout << "Source: " << sqlRes->getString("source_id") << endl;
    //    }
    //    delete sqlRes;
    //} catch (sql::SQLException &e) {
    //    cout << "# ERR: SQLException in " << __FILE__;
    //    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    //    cout << "# ERR: " << e.what();
    //    cout << " (MySQL error code: " << e.getErrorCode();
    //    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    //}


    return false;
}

//int main() {
//    Transaction newTrans;
//    newTrans.Index = 1;
//    newTrans.SourceId = "Source Carlos";
//    newTrans.TargetId = "Target Carlos";
//    newTrans.Data = "this is some test data";
//    addNewTransaction(newTrans);
//    Block newBlock;
//    newBlock.Timestamp = time(0);
//    newBlock.PreviousBlockHash = "imagine that this is a hash";
//    addNewBlock(newBlock, newTrans.Index);
//
//    return 0;
//}

