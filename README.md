# blockchain-model
Trying to understand blockchain playing around with block generation. Let's see how far I can get!

This is an extremely simplified centralized toy implementation, focused on understanding how the each block of the chain is created and potential problems a developer may faced.
* I used a centralized HTTP API [Crow](https://github.com/ipkn/crow) to send new transactions and update the chain, which is hold by a master node.

... Using this solution I don't need to focus on creating the TCP connections to establish the peer-to-peer network and distribute the current status of the chain.

* Each new transaction is created by sending a POST request to the API, which only validates that the source node of the transaction is part of the network and then creates the new block (if hashes match).
* Since validation of transactions is performed by the master node, no proof-of-work was implemented.

## My goal is to fix these assumptions over time...
