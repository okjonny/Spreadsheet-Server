//
// Created by jonny on 4/22/21.
//

#include "server_controller.h"
#include "networking.h"
#include <iostream>

using namespace network_util;
namespace spreadsheet_server {
/// <summary>
/// Start accepting Tcp sockets connections from clients
/// </summary>
    void server_controller::start_server() {
        // This begins an "event loop"

//        StartServer(first_contact, 11000);
        networking::start_server(first_contact);
        std::cout << "Server is running. Accepting new Players:" << std::endl;
    }


/// <summary>
/// Method to be invoked by the networking library
/// when a new client connects (see line 43)
/// </summary>
/// <param name="state">The SocketState representing the new client</param>
    void server_controller::first_contact(socket_state state) {
//        if (state.ErrorOccured)
//            return;

        std::cout << "Client " << state.get_socket_id() << " connected" << std::endl;
        state.on_network_action = receive_name;

//        Networking.GetData(state);
    }

/// <summary>
/// Method to be invoked by the networking library
/// when a new client connects (see line 43)
/// </summary>
/// <param name="state">The SocketState representing the new client</param>
    void server_controller::receive_name(socket_state state) {
        // Remove the client if they aren't still connected
//        if (state.ErrorOccured) {
//            RemoveClient(state.ID);
//            return;
//        }

        List <string> clientData = ProcessData(state);

        Tank clientTank = new Tank((int) state.ID, clientData[0].TrimEnd('\n'), new Vector2D(0, 0));

        // change the state's network action to the
        // receive handler so we can process data when something
        // happens on the network
        state.OnNetworkAction = ReceiveCommands;

        // Then send the startup info to the client.Then add the client's socket to a list of all clients.
        Networking.Send(state.TheSocket, state.ID.ToString() + "\n" + GameWorld.Size + "\n");

        // Send walls
        lock(GameWorld.Walls)
        {
            foreach(Wall
            w
                    in
            GameWorld.Walls.Values)
            Networking.Send(state.TheSocket, JsonConvert.SerializeObject(w) + "\n");
        }

        Console.WriteLine("Player ID: " + state.ID.ToString() + "\n" + "World Size: " + GameWorld.Size + "\n" + "SENT");


        // Save the client state
        // Need to lock here because clients can disconnect at any time
        lock(Clients)
        Clients[state.ID] = state;

        lock(GameWorld.Tanks)
        GameWorld.Tanks.Add((int) state.ID, clientTank);

        Networking.GetData(state);
    }


/// <summary>
/// Method to be invoked by the networking library
/// when a network action occurs (see lines 68-70)
/// </summary>
/// <param name="state"></param>
    private

    void ReceiveCommands(SocketState state) {
        // Remove the client if they aren't still connected
        if (state.ErrorOccured) {
            RemoveClient(state.ID);
            return;
        }

        List <string> worldData = ProcessData(state);

        foreach(string
        s
                in
        worldData)
        {

            ControlCommand c = JsonConvert.DeserializeObject<ControlCommand>(s);
            lock(Commands)
            {
                Commands[state.ID] = c;
            }
        }

        // Continue the event loop that receives messages from this client
        Networking.GetData(state);
    }



/// <summary>
/// Process any buffered data separated by '\n'.

/// </summary>
/// <param name="state">The SocketState holding the data to be processed.</param>
/// <returns>A list of data that has been processed (separated by '\n').</returns>
    private

    List <string> ProcessData(SocketState state) {
        string totalData = state.GetData();
        string[]
        parts = Regex.Split(totalData, @"(?<=[\n])");

        // Loop until we have processed all messages.
        // We may have received more than one.
        List <string> newMessages = new List<string>();

        foreach(string
        p
                in
        parts)
        {
            // Ignore empty strings added by the regex splitter
            if (p.Length == 0)
                continue;
            // The regex splitter will include the last string even if it doesn't end with a '\n',
            // So we need to ignore it if this happens.
            if (p[p.Length - 1] != '\n')
                break;

            // build a list of messages to send to the view
            newMessages.Add(p);

            // Then remove it from the SocketState's growable buffer
            state.RemoveData(0, p.Length);
        }
        return newMessages;
    }
}