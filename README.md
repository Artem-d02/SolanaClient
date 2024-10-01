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

Let's start with the method. It contains classes that implement the abstraction of Solana methods in the project. They do not call anything directly, but serve to combine the interface of all possible GET methods, create a request body, and verify request parameters. The ``TMethodBase`` class is a basic interface. In order to create an implementation of a specific method, you need to inherit from the ``TMethosBase`` class and redefine your checks for request parameters, as well as the method name. This is necessary to improve performance, since the ability to check the request parameters before sending it leads to a decrease in network load and a reduction in program operation time. An example of the implementation of the ``GetAccountInfo`` and ``GetBalance`` methods can be found in the ``get_methods.hpp`` file. The `nlohman/json` library was used to represent json objects in the code, because it provides an extremely user-friendly interface for reading and serializing objects.

Next, let's look at the part responsible for the HTTP client. The ``THttpclient`` class provides an interface for connecting to the required host on the required port. It has a set of synchronous and asynchronous (non-blocking methods), but at the moment there is no clear advantage in using the asynchronous API, since asynchrony cannot give performance gains when sending single requests, and when sending packaged requests it is very easy to encounter ratelimit on the server. The synchronous request method blocks the thread in which it is called until a response or error message is received, the asynchronous method accepts a callback as input, which will be called upon receipt of the request. The popular `boost::asio` library was chosen as the basis for networking, as it provides a user-friendly interface and high performance.

In the `events` part, it is worth paying attention to the `T Simple Scheduler` class. Since solana has a rate limit for sending requests from a single IP address, you can't just send requests in a row (for example, in a program loop). To easily solve this problem, a scheduler class was developed, which is arranged as follows: when calling the `start()` method of the class, it creates a new `MainThread` thread, in which, with a certain (constant) period, it launches another task from the queue (in a new thread!) contained inside the class. Thus, in order to execute a number of requests in Solana, it is enough to add this entire series of requests to the scheduler queue and wait for answers to them.

In the same section, the `TEventHandler` class was created, which can accept various types of events as input and process them. For extensibility of the code, the following architecture was chosen: in order for the class to handle an event of a certain type, it is necessary to pre-register a handler for this event, which must be a derived class from the base interface `TBaseHandlerFunctor`. As an example, 3 functors are implemented in the code:
- `TInvokeHandlerFunctor` which makes a request to Solana in a new thread (using the scheduler described above) and saves the responses in chronological order
- `TNothingHandlerFunctor` that does nothing
- `TErrorHandlerFunctor`, which outputs the specified error message 

Note: only those responses whose value of the `slot` field differs from the previously received values are saved to the data structure.

And finally, the `TStats` class is designed to effectively track the normal deviation of the latency value in receiving a response to requests (the difference between the time of receiving a response and the time of sending a request) in a sliding time window of fixed width T. The calculation algorithm works in O(1) and recalculates this value for each new answer.

Since the unit testing system is not connected in this project, in the file main.cpp There are several functions for testing individual program components that can be used as an example of using classes. For direct operation, only the `testEventHandler` function is used, which accepts the following parameters as input:
- `host` - hostname
- `port` - port number
- `solanaDelayMs` - minimum delay between requests in ms (Solana allows no more than 100 requests per 10 seconds for a single IP, therefore, the minimum delay should be `10/100 = 0.1 s = 100 ms`)
- `statsWindowSizeMs` - the size of the window for collecting statistics in ms
- `requestsCount` - window size for collecting statistics on request delays