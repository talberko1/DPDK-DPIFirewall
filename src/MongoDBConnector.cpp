//
// Created by tal on 02/12/2020.
//

#include "MongoDBConnector.h"

MongoDBConnector::MongoDBConnector(const char *uri) {
    mongocxx::instance instance{};
    m_Client = mongocxx::client(mongocxx::uri(uri));
    //"mongodb+srv://<username>:<password>@<cluster-url>/<database>?retryWrites=true&w=majority" <-- atlas
}

mongocxx::database MongoDBConnector::getDatabase(const char *databaseName) {
    return m_Client[databaseName];
}



