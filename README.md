# Solana client
The purpose of this project was to solve three main tasks:
- Implementation of a simple version of the HTTP client capable of making requests using rpc2.0 (in particular, in solana ecosystem) 
- Modeling the operation of an asynchronous event handler from a deterministic set capable of executing requests in various threads using the client described in the paragraph above
- Creating an efficient real-time statistics collection class that calculates the normal deviation for a set of query delays executed during a time window of a given length
## Additional dependencies
To successful build you need to have next points in you system:
- ``cmake``
- ``boost``
- ``nlohman/json``
- ``g++-13``
## Installation
1. Clone this repository:
    ``git clone https://github.com/Artem-d02/SolanaClient``
2. Go to the project folder
3. Create build directory and navigate to it:
    ``mkdir build``
    ``cd build``
4. Launch cmake:
    ``cmake ..``
5. Make the project:
    ``make``
6. Run application:
    ``./app``
## Detailed description
The entire project code is divided into 4 main blocks - client, method, events and stats (the code of which is contained in the corresponding folders). Let's take a closer look at the principles of operation of each of them. 

Let's start with the method. It contains classes that implement the abstraction of Solana methods in the project. They do not call anything directly, but serve to combine the interface of all possible GET methods, create a request body, and verify request parameters. The ``TMethodBase`` class is a basic interface. In order to create an implementation of a specific method, you need to inherit from the ``TMethosBase`` class and redefine your checks for request parameters, as well as the method name. This is necessary to improve performance, since the ability to check the request parameters before sending it leads to a decrease in network load and a reduction in program operation time. An example of the implementation of the ``GetAccountInfo`` and ``GetBalance`` methods can be found in the ``get_methods.hpp`` file.
