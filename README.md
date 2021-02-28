# Image_sender_reciver_UDP

Selecting the client-server role through the application launch parameter. 

## Server part
1. Opens a connection on the specified UDP port.
2. Waits for the client to send a JSON packet containing the "start" command.
3. Reads a picture from the bmp disk (at least 2MB in size) and sends it to the client.
4. It is necessary to provide protection against possible mixing of packets (since UDP protocol).

## Client side
1. Sends the "start" command in a JSON format packet to the server on the specified port.
2. Receives a bmp picture from the server, checks that all content is received correctly and renders it using OpenGL. 
