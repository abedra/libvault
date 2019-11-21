#include <nlohmann/json.hpp>
#include "VaultClient.h"

Ldap::Ldap(std::string uname, std::string pwd) :
	username(uname), password(pwd) {}

std::string Ldap::getUrl(const VaultClient& client, std::string path) {
	return client.getUrl("/v1/auth/ldap/login/", path);
}

optional<std::string> Ldap::authenticate(const VaultClient& client) {
	nlohmann::json j;
	j = nlohmann::json::object();
	j["password"] = password;

	auto response = client.getHttpClient()
		.post(getUrl(client, this->username),
			client.getToken(),
			client.getNamespace(),
			j.dump());

	if (HttpClient::is_success(response)) {
		return nlohmann::json::parse(response.value().body)["auth"]["client_token"];
	}
	else {
		return std::experimental::nullopt;
	}
}
