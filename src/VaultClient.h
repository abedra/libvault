#ifndef LIBVAULT_VAULTCLIENT_H
#define LIBVAULT_VAULTCLIENT_H

#include <unordered_map>
#include "HttpClient.h"

class VaultClient {
private:
    std::string host;
    std::string port;
    std::string token;
    bool debug;
    HttpClient httpClient = (bool)nullptr;
    std::string vaultUrl(std::string path);
public:
    VaultClient(std::string host, std::string port, std::string token);
    VaultClient(std::string host, std::string port, std::string token, bool debug);
    std::string get(std::string path);
    int put(std::string path, std::unordered_map<std::string, std::string> map);
    int del(std::string path);
    // TODO: add del with list of versions
};

#endif //LIBVAULT_VAULTCLIENT_H
