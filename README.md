# BuzingaServ: A Custom HTTP Sever

BuzingaServ is a custom web server application implemented in C++ that's designed to understand and process HTTP requests and responses.

## URL Understanding: 
BuzingaServ is capable of recognizing and processing Universal Resource Locators (URLs), ensuring accurate request routing.

## HTTP Protocol:
The server handles plain text messages that conform to the Hypertext Transfer Protocol (HTTP) standards.

## Transport Layer Integration:
All messages are transmitted through a transport layer.
BuzingaServ makes use of the Transmission Control Protocol (TCP), ensuring reliable message delivery.

## Protocol Suite:
Both HTTP and TCP are part of the internet protocol suite.
It's worth noting that while our server is custom-built,
these protocols' foundational support is provided by the operating system.

## Definitions

### HTTP:

A language for communicating between two computers. It specifies how to transfer documents that are interconnected by hyperlinks.
A message is constructed as a request or a response.

### TCP:

Transmission Control Protocol
Used to establish host-to-host data transfer channels.
It maintains communications between application processes between hosts (client and server), and they use port numbers to track sessions.

### Internet Protocol Suite || TCP/IP:

A conceptual model to specify how data should be packetized,addressed, transmitted routed and received.
It made up of 4 abstraction layers: application, transport, internet and link layers.
An application like a web browser needs to receive data over the internet it communicates with the Trancport layer of the CP/IP stack using a specific port number.
HTTP usually uses port 80.

### Request / Response

##### HTTP Request

1. A request line,
2. Headers,
3. Empty line,
4. optional message body.

#### HTTP Response

1. Status line,
2. Headers,
3. Empty line,
4. optional message body.

When a request is made by the application layer, the message passes through the layerson one side, and back up through layer on the other side.
Logically each layer talks to the corresponding layer on the other side.

### man 4 tcp

SOCKET -> BIND -> LISTEN -> ACCEPT -> READ/WRITE

We need to implement a ‘socket’ on which we can ‘listen’ for incoming connections,
Then we need to ‘bind’ the socket to a local address, and port.
Then put the socket in a ‘listen’ state.
After that we’re able to ‘accept’ incoming connections,
for each accepted connection a new socket will be created, 
and we will be able to read and write to this socket.
The following diagram gives a bit of an overview of what we need to implement.

### man 4 socket
socket() creates an endpoint for communication and returns a descriptor.


## Tools

- [RFC Navigator](https://rfc.fyi/?collection=HTTP)
- [Postman](https://www.postman.com/)
- [Postman community](https://www.community.postman.com/)

## References

- [42 Evaluation Sheet](https://rphlr.github.io/42-Evals/Rank05/webserv/)
- [HTTP Server: Everything you need to know to Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa) (requires Medium Premium)
- [Making a simple HTTP webserver in C](https://bruinsslot.jp/post/simple-http-webserver-in-c/)
- [Thread pools](https://stackoverflow.com/questions/15752659/thread-pooling-in-c11)

## Required Reading

- [Linux Programming Interface](https://sciencesoftcode.files.wordpress.com/2018/12/the-linux-programming-interface-michael-kerrisk-1.pdfh): Chapter 56-61
- [RFC:  9112](https://www.rfc-editor.org/rfc/rfc9112.pdf) (HTTP/1.1) (could be obsoletes)
