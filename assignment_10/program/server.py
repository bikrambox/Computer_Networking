import socket
import select
import sys
from thread import *

#create an INET, Streaming socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#set SO_REUSEADDR on a socket
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 
if len(sys.argv) != 3:
    print "Correct usage: script, IP address, port number"
    exit()
 
IP_address = str(sys.argv[1])

Port = int(sys.argv[2])
 
#bind the ip and port
server.bind((IP_address, Port))

#listening
server.listen(5)
 
#for storing the number of the client
list_of_clients = []
 

def clientthread(conn, addr):
 
    # sends a message to the client whose user object is conn
    conn.send("Multithreaded chat program")
 
    while True:
                message = conn.recv(2048)
                if message:

                    print "<" + addr[0] + "> " + message

                    # Calls broadcast function to send message to all
                    message_to_send = "<" + addr[0] + "> " + message
                    broadcast(message_to_send, conn)
 
def broadcast(message, connection):
    for clients in list_of_clients:
        if clients!=connection:
                clients.send(message)
            
while True:
 
    #addr which contains the IP address of the client that justconnected
    conn, addr = server.accept()
 
    #Maintains a list of clients for ease of broadcasting
    list_of_clients.append(conn)
 
    # prints the address of the user that just connected
    print addr[0] + " connected"
 
    # creates and individual thread for every user that connects
    start_new_thread(clientthread,(conn,addr))    
 
conn.close()
server.close()
