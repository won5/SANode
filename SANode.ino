/*********************************************************************************************
* File: CommandServer
* Project: LG Exec Ed Program
* Copyright: Copyright (c) 2013 Anthony J. Lattanze
* Versions:
*	1.0 May 2013. 
*
* Description:
*
* This program runs on an Arduio processor with a WIFI shield. This program will act as a 
* general server that can be extended to do various other tasks. Currently the server will
* allow a client to connect and query. The response to the query is to return the WIFI shield's
* MAC address. Note that this code should be paired with FindServer.java client applications 
* snince that follows the command conventions. Other commands can easily be added to this and 
* client applications can easily be added to extend the services provided.
*
* Compilation and Execution Instructions: Compiled using Arduino IDE
*
* Parameters: None
*
* Internal Methods: void printConnectionStatus() 
*
************************************************************************************************/

 #include <SPI.h>
 #include <WiFi.h>

 char ssid[] = "Shadyside Inn";           // The network SSID (name) 
 int status = WL_IDLE_STATUS;   // The status of the network connections
 WiFiServer server(50000);        // The WIFI status,.. we are using port 500
 char inChar;                   // This is a character sent from the client
 char command;                  // This is the actual command character
 IPAddress ip;                  // The IP address of the shield
 IPAddress subnet;              // The subnet we are connected to
 long rssi;                     // The WIFI shield signal strength
 byte mac[6];                   // MAC address of the WIFI shield
 boolean done;                  // Loop flag
 
 // for IoT Server Discovery
 #define   PARSING_ADDR 1
 #define   PARSING_PORT 2
 
 int parsingStatus = PARSING_ADDR;
 String serverIPStr;
 int serverPort=0;
 
 void setup() 
 {
   // Initialize serial port. This is used for debug.
   Serial.begin(9600); 
  
   // Attempt to connect to WIfI network indicated by SSID.
   while ( status != WL_CONNECTED) 
   { 
     Serial.print("Attempting to connect to SSID: ");
     Serial.println(ssid);
     status = WiFi.begin(ssid);
   }  
   
   // Print connection information to the debug terminal
   printConnectionStatus();
   
   // Start the server and print a message to the terminial.
   server.begin();
   Serial.println("The Server is started.");
   Serial.println("--------------------------------------\n\n");
   
 } // setup


 void loop() 
 {
   // Listen for a new client.
   WiFiClient client = server.available();

   // Wait until we are collected to the client.
   if (client) 
   {
     Serial.println("Client Service Requested..."); 

     // Here is the command parser. Commands are in the format of
     // commandcharacter + /n. 
     
     done = false;
     
     while ( !done )
     {
       if (client.available())           // Checks to see if data is ready to read
       {
         inChar = client.read();         // Read the first character from the client and
         Serial.print(inChar);
         
         if (inChar == ':' )            // Check to see if we are done.
         {
           serverIPStr += '\0';             // If we are not done, we just read the command.
           parsingStatus = PARSING_PORT;
         }
         else if (inChar == '\n') 
         {
           serverPort = serverPort*10 + inChar - '0';
           done = true;
         }
         else
         {
           switch (parsingStatus)
           {
           case PARSING_ADDR :
             serverIPStr += inChar;             // If we are not done, we just read the command.
             break;
           case PARSING_PORT :
             serverPort = serverPort*10 + inChar - '0';
             break;
           }
         } 
       } // if 
       
     } // while

     Serial.print( "server IP Address : " );      // We print the command to the terminal
     Serial.print( serverIPStr );
     Serial.print( ":" );
     Serial.println(serverPort);
    
     // To finish up the command processing we flush the input, stop the connection, 
     // and print a message to the terminal.
     client.flush();       
     client.stop();
     Serial.println( "Client Disconnected.\n" );
     Serial.println( "........................." );
     
   } // if client is connected
 
 } // loop
 
/************************************************************************************************
* The following method prints out the connection information
************************************************************************************************/

 void printConnectionStatus() 
 {
     // Print the basic connection and network information: Network, IP, and Subnet mask
     ip = WiFi.localIP();
     Serial.print("Connected to ");
     Serial.print(ssid);
     Serial.print(" IP Address:: ");
     Serial.println(ip);
     subnet = WiFi.subnetMask();
     Serial.print("Netmask: ");
     Serial.println(subnet);
   
     // Print our MAC address.
     WiFi.macAddress(mac);
     Serial.print("WiFi Shield MAC address: ");
     Serial.print(mac[5],HEX);
     Serial.print(":");
     Serial.print(mac[4],HEX);
     Serial.print(":");
     Serial.print(mac[3],HEX);
     Serial.print(":");
     Serial.print(mac[2],HEX);
     Serial.print(":");
     Serial.print(mac[1],HEX);
     Serial.print(":");
     Serial.println(mac[0],HEX);
   
     // Print the wireless signal strength:
     rssi = WiFi.RSSI();
     Serial.print("Signal strength (RSSI): ");
     Serial.print(rssi);
     Serial.println(" dBm");

 } // printConnectionStatus

