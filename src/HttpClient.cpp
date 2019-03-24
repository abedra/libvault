#include <curl/curl.h>
#include <iostream>

#include "HttpClient.h"

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

HttpClient::HttpClient() : HttpClient(false) {}

HttpClient::HttpClient(bool debug) {
    this->debug = debug;
}

std::string HttpClient::get(std::string url, std::string token) {
    // curl                                                        \
    //    -H "X-Vault-Token: f3b09679-3001-009d-2b80-9c306ab81aa6" \
    //    -X GET                                                   \
    //    http://localhost:8200/v1/secret/data/hello

    CURL *curl;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = nullptr;
        chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());

        // TODO: SSL verify host and peer
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (debug) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cout << "GET " << url << " failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }

    return readBuffer;
}

int HttpClient::post(std::string url, std::string token, std::string value) {
    // curl                                                        \
    //    -H "X-Vault-Token: f3b09679-3001-009d-2b80-9c306ab81aa6" \
    //    -H "Content-Type: application/json"                      \
    //    -X POST                                                  \
    //    -d '{"data": {"foo":"bar"}}'                                   \
    //    http://localhost:8200/v1/secret/data/hello

    CURL *curl;
    CURLcode res = CURLE_SEND_ERROR;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = nullptr;
        chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());
        chunk = curl_slist_append(chunk, "Content-Type: application/json");

        // TODO: SSL verify host and peer
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (debug) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cout << "POST " << url << " failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }

    return res;
}

int HttpClient::del(std::string url, std::string token) {
    // curl                                                        \
    //    -H "X-Vault-Token: f3b09679-3001-009d-2b80-9c306ab81aa6" \
    //    -H "Content-Type: application/json"                      \
    //    -X DELETE                                                \
    //    http://localhost:8200/v1/secret/data/hello

    CURL *curl;
    CURLcode res = CURLE_SEND_ERROR;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = nullptr;
        chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());
        chunk = curl_slist_append(chunk, "Content-Type: application/json");

        // TODO: SSL verify host and peer
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (debug) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cout << "DELETE " << url << " failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }

    return res;
}
