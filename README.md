# Transfer filter contract
EOS has a method to check incoming transfers and This contract makes token transfers to it fail, if memo in those transfers does not
satisfy certain conditions. Currently set conditions are:
* Memo has to be exactly 8 symbols long;
* Contain only digits;

You add different asserts in ontransfer action to modify these conditions.

Only transfers wher contract is receiver are currently filtered.

### Build
```commandline
eosiocpp -o transferfilter.wast transferfilter.cpp
```

### Deploy
```commandline
cleos set code <your-account-name> transferfilter.wast
```

### Test
#### Without memo
```commandline
$ cleos transfer eosio <your-account-name> "10.0000 SYS"
Error 3050003: eosio_assert_message assertion failure
```

#### With non digit
```commandline
$ cleos transfer eosio <your-account-name> "10.0000 SYS" "1234567a"
Error 3050003: eosio_assert_message assertion failure
```

#### Valid
```commandline
$ cleos transfer eosio <your-account-name> "10.0000 SYS" "00345670"
executed transaction: 747ceb78a0359ffd426df96d3ce4c844bfe7402e1634638a14ebc2d91c676a91  136 bytes  607 us
#   eosio.token <= eosio.token::transfer        {"from":"eosio","to":"<your-account-name>","quantity":"10.0000 SYS","memo":"00345670"}
#         eosio <= eosio.token::transfer        {"from":"eosio","to":"<your-account-name>","quantity":"10.0000 SYS","memo":"00345670"}
#      <your-account-name> <= eosio.token::transfer        {"from":"eosio","to":"<your-account-name>","quantity":"10.0000 SYS","memo":"00345670"}
warning: transaction executed locally, but may not be confirmed by the network yet    ]
```
