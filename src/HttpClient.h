#ifndef LIBVAULT_HTTPCLIENT_H
#define LIBVAULT_HTTPCLIENT_H

class HttpClient {
private:
    bool debug;
public:
    HttpClient();
    HttpClient(bool debug);
    std::string get(std::string url, std::string string);
    int post(std::string url, std::string token, std::string value);
};

#endif //LIBVAULT_HTTPCLIENT_H
