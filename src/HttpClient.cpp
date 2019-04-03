#include <iostream>

#include "VaultClient.h"

static size_t
writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

HttpClient::HttpClient() :
  debug_(false),
  errorCallback_([&](std::string err){})
{}

HttpClient::HttpClient(bool debug) :
  debug_(debug),
  errorCallback_([&](std::string err){})
{}

HttpClient::HttpClient(HttpErrorCallback errorCallback) :
  debug_(false),
  errorCallback_(errorCallback)
{}

HttpClient::HttpClient(HttpErrorCallback errorCallback, bool debug) :
  debug_(debug),
  errorCallback_(errorCallback)
{}

std::experimental::optional<HttpResponse>
HttpClient::get(std::string url, std::string token, std::string ns) {
  return executeRequest(url, token, ns, [&](CURL *curl) {});
}

std::experimental::optional<HttpResponse>
HttpClient::post(std::string url, std::string token, std::string ns, std::string value) {
  return executeRequest(url, token, ns, [&](CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
  });
}

std::experimental::optional<HttpResponse>
HttpClient::del(std::string url, std::string token, std::string ns) {
  return executeRequest(url, token, ns, [&](CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  });
}

std::experimental::optional<HttpResponse>
HttpClient::list(std::string url, std::string token, std::string ns) {
  return executeRequest(url, token, ns, [&](CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST");
  });
}

std::experimental::optional<HttpResponse>
HttpClient::executeRequest(std::string url,
                           std::string token,
			   std::string ns,
                           CurlSetupCallback setupCallback) {
  CURL *curl;
  CURLcode res = CURLE_SEND_ERROR;
  std::string buffer;
  long response_code = 0;

  curl = curl_easy_init();
  if (curl) {
    struct curl_slist *chunk = nullptr;

    if (!token.empty()) {
      chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());
    }

    if (!ns.empty()) {
      chunk = curl_slist_append(chunk, ("X-Vault-Namespace: " + ns).c_str());
    }

    chunk = curl_slist_append(chunk, "Content-Type: application/json");

    // TODO: SSL verify host and peer
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    if (debug_) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    setupCallback(curl);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      errorCallback_(curl_easy_strerror(res));

      curl_easy_cleanup(curl);
      curl_slist_free_all(chunk);

      return std::experimental::nullopt;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);
  }

  return std::experimental::optional<HttpResponse>({response_code, buffer});
}
