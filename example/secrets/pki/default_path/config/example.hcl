path "pki/*" {
  capabilities = ["read", "update", "list", "delete", "create"]
}

path "auth/approle/login" {
  capabilities = ["create", "read"]
}

path "auth/approle/role/*" {
  capabilities =  ["create", "read", "update", "delete", "list"]
}
