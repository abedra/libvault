path "secret/*" {
  capabilities = ["read", "update", "list", "delete", "create"]
}

path "secret/metadata/*" {
  capabilities = ["read", "update", "list", "delete", "create"]
}

path "legacy/*" {
  capabilities = ["read", "update", "list", "delete", "create"]
}

path "sys/wrapping/unwrap" {
  capabilities = ["read", "create", "update"]
}

path "sys/wrapping/rewrap" {
  capabilities = ["read", "create", "update"]
}

path "auth/approle/login" {
  capabilities = ["create", "read"]
}

path "auth/approle/role/*" {
  capabilities =  ["create", "read", "update", "delete", "list"]
}

path "transit/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}

path "totp/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}

path "cubbyhole/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}

path "pki/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}

path "rabbitmq/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}

path "database/*" {
  capabilities = ["create", "read", "update", "delete", "list"]
}
