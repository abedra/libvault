# PKI Example

Start the Vault docker container by running

```shell
make vault
```

Next, setup the environment by running:

```shell
script/setup
```

Source the environment variables created by the setup script:

```shell
source .env
```

Finally, compile and run the example:

```shell
make
./example
```