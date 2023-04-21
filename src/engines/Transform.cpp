#include "VaultClient.h"

std::optional<std::string>
Vault::Transform::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string>
Vault::Transform::updateRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Transform::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Transform::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::Transform::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::Transform::createTransformation(const Path &path,
                                       const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"transformation/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::updateTransformation(const Path &path,
                                       const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"transformation/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::createFPETransformation(const Path &path,
                                          const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"transformation/fpe/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::updateFPETransformation(const Path &path,
                                          const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"transformation/fpe/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::createMaskingTransformation(const Path &path,
                                              const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"transformation/masking/" + path}), parameters);
}

std::optional<std::string>
Vault::Transform::updateMaskingTransformation(const Path &path,
                                              const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"transformation/masking/" + path}), parameters);
}

std::optional<std::string> Vault::Transform::createTokenizationTransformation(
    const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"transformation/tokenization/" + path}), parameters);
}

std::optional<std::string> Vault::Transform::updateTokenizationTransformation(
    const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"transformation/tokenization/" + path}), parameters);
}

std::optional<std::string>
Vault::Transform::readTransformation(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"transformation/" + path}));
}

std::optional<std::string> Vault::Transform::listTransformations() {
  return HttpConsumer::list(client_, getUrl(Path{"transformation"}));
}

std::optional<std::string>
Vault::Transform::deleteTransformation(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"transformation/" + path}));
}

std::optional<std::string>
Vault::Transform::createTemplate(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"template/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::updateTemplate(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"template/" + path}),
                            parameters);
}

std::optional<std::string> Vault::Transform::readTemplate(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"template/" + path}));
}

std::optional<std::string> Vault::Transform::listTemplates() {
  return HttpConsumer::list(client_, getUrl(Path{"template"}));
}

std::optional<std::string> Vault::Transform::deleteTemplate(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"template/" + path}));
}

std::optional<std::string>
Vault::Transform::createAlphabet(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"alphabet/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::updateAlphabet(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"alphabet/" + path}),
                            parameters);
}

std::optional<std::string> Vault::Transform::readAlphabet(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"alphabet/" + path}));
}

std::optional<std::string> Vault::Transform::listAlphabets() {
  return HttpConsumer::list(client_, getUrl(Path{"alphabet"}));
}

std::optional<std::string> Vault::Transform::deleteAlphabet(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"alphabet/" + path}));
}

std::optional<std::string>
Vault::Transform::createTokenizationStore(const Path &path,
                                          const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"store/" + path}), parameters);
}

std::optional<std::string>
Vault::Transform::updateTokenizationStore(const Path &path,
                                          const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"store/" + path}), parameters);
}

std::optional<std::string>
Vault::Transform::createStoreSchema(const Path &path,
                                    const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"store/" + path + "/schema"}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::updateStoreSchema(const Path &path,
                                    const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"store/" + path + "/schema"}),
                            parameters);
}

std::optional<std::string> Vault::Transform::readStore(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"store/" + path}));
}

std::optional<std::string> Vault::Transform::listStores() {
  return HttpConsumer::list(client_, getUrl(Path{"store"}));
}

std::optional<std::string> Vault::Transform::deleteStore(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"store/" + path}));
}

std::optional<std::string>
Vault::Transform::encode(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"encode/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::decode(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"decode/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::validateToken(const Path &path,
                                const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"validate/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::checkTokenization(const Path &path,
                                    const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"tokenized/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::Transform::retrieveTokenMetadata(const Path &path,
                                        const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"metadata/" + path}),
                            parameters);
}

Vault::Url Vault::Transform::getUrl(const Path &path) {
  return client_.getUrl("/v1/transform/", path);
}
