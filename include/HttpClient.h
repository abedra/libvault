#pragma once

#include <curl/curl.h>
#include <functional>

class HttpClient {
private:
  bool debug;
  std::pair<CURLcode, std::string> executeRequest(std::string &url, std::string &token, std::function<void(CURL *curl)> callback);
public:
  HttpClient();
  HttpClient(bool debug);
  std::string get(std::string url, std::string string);
  int post(std::string url, std::string token, std::string value);
  int del(std::string url, std::string token);
};
