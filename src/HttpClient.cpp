#include <iostream>

#include "HttpClient.h"

static size_t
writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

HttpClient::HttpClient() : HttpClient(false) {}

HttpClient::HttpClient(bool debug) {
  this->debug = debug;
}

std::string
HttpClient::get(std::string url, std::string token) {
  auto result = executeRequest(url, token, [&](CURL *curl) {});

  if (result.first != CURLE_OK) {
    std::cout
      << "GET " << url << " failed: " << curl_easy_strerror(result.first)
      << std::endl;

    return "";
  }

  return result.second;
}

std::string
HttpClient::post(std::string url, std::string token, std::string value) {
  auto result = executeRequest(url, token, [&](CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
  });

  if (result.first != CURLE_OK) {
    std::cout
      << "POST " << url << " failed: " << curl_easy_strerror(result.first)
      << std::endl;

    return "";
  }

  return result.second;
}

std::string
HttpClient::del(std::string url, std::string token) {
  auto result = executeRequest(url, token, [&](CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  });

  if (result.first != CURLE_OK) {
    std::cout
      << "DELETE " << url << " failed: " << curl_easy_strerror(result.first)
      << std::endl;

    return "";
  }

  return result.second;
}

std::pair<CURLcode, std::string>
HttpClient::executeRequest(std::string url,
                           std::string token,
                           std::function<void(CURL *curl)> callback) {
  CURL *curl;
  CURLcode res = CURLE_SEND_ERROR;
  std::string buffer;

  curl = curl_easy_init();
  if (curl) {
    struct curl_slist *chunk = nullptr;
    if (!token.empty()) {
      chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());
    }
    chunk = curl_slist_append(chunk, "Content-Type: application/json");

    // TODO: SSL verify host and peer
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    if (debug) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    callback(curl);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);
  }

  return std::make_pair(res, buffer);
}
