g++ server.cpp stack.cpp -lpthread -o server
g++ client.cpp -o client
 ./server
 ./client "127.0.0.1"