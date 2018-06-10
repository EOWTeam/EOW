# EOW
EOW is a BasS open platform which provides customers with IP tokenization, crowd-sale, investment and management services based on blockchain. 

EOW enables the traditional IP industry with blockchain ability, to reconstruct the relationship among authors, readers and investors which can improve the creativity of authors, invokes the enthusiasm of all participants to make the IP more and more valuable. n.

This part is about eow's smart contract,the rest of the EOW project is in the following places:

EOW Offical: https://github.com/EOWTeam/eow.official

EOW DReading: https://github.com/EOWTeam/DReading

EOW Wallet: https://github.com/EOWTeam/eow.wallet

EOW Js: https://github.com/EOWTeam/eow.js

The next steps will guide you quickly in the command line to deploy and test the smart contract method:
1、Start your EOS node;

2、Put the noteip folder and put it in the eos/build directory

3、cd noteip

eosiocpp -o noteip.wast noteip.cpp

eosiocpp -g noteip.abi noteip.cpp

4、Put the operation_scripts folder and put it in the eos/ directory

5、cd operation_scripts

./swagger.sh

./cpp.sh

6、Ok, until here you'll see the method in noteip contract run successfully!

If you want to see how does these methods called by eosjs, you can see https://github.com/EOWTeam/eow.js