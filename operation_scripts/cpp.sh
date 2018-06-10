cleos set contract team ../contracts/noteip/ -p team

cleos push action team create '["author","末世皇帝","www.moshi.com","末世币","MSP","100000 MSP",30000,"1 EOW"]' -p author

cleos get table team team ip

cleos push action eosio.token create '[ "eosio.token", "1000000000.0000 EOW", 0, 0, 0]' -p eosio.token

cleos push action eosio.token create '[ "eosio.token", "100000.0000 MSP", 0, 0, 0]' -p eosio.token

cleos push action eosio.token issue '[ "reader", "1000.0000 EOW", "memo" ]' -p eosio.token

cleos push action eosio.token issue '[ "author", "100000.0000 MSP", "memo" ]' -p eosio.token

cleos get currency balance eosio.token reader

cleos set account permission reader active '{"threshold": 1,"keys": [{"key": "EOS5BisJQyMeeQMfxA2sULSB82qXvDK6czZSggBT6oCjc8m75vkEZ","weight": 1}],"accounts": [{"permission":{"actor":"team","permission":"eosio.code"},"weight":1}]}' owner -p reader

cleos set account permission author active '{"threshold": 1,"keys": [{"key": "EOS5BisJQyMeeQMfxA2sULSB82qXvDK6czZSggBT6oCjc8m75vkEZ","weight": 1}],"accounts": [{"permission":{"actor":"team","permission":"eosio.code"},"weight":1}]}' owner -p author

cleos push action team buyip '[0,"reader","10.0000 MSP","10.0000 EOW"]' -p reader

cleos get currency balance eosio.token reader

cleos get currency balance eosio.token author

cleos push action eosio.token issue '[ "gamecom", "5000000.0000 EOW", "memo" ]' -p eosio.token

cleos set account permission gamecom active '{"threshold": 1,"keys": [{"key": "EOS5BisJQyMeeQMfxA2sULSB82qXvDK6czZSggBT6oCjc8m75vkEZ","weight": 1}],"accounts": [{"permission":{"actor":"team","permission":"eosio.code"},"weight":1}]}' owner -p gamecom

cleos push action team dividend '[0,"video_adaptation_right","gamecom","30000.0000 EOW"]' -p gamecom

cleos get currency balance eosio.token gamecom

cleos get currency balance eosio.token author