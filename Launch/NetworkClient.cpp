#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"

NetworkClient::NetworkClient(InputRecorder* keyboardAndMouse,
	PlayerBase* playerbase, GameplaySystem* gameplay) : Subsystem("NetworkClient")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT }, 
		DeliverySystem::getPostman()->getDeliveryPoint("NetworkClient"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	{
	});

	if (network.Initialize(0))
	{
		serverConnection = network.ConnectPeer(10, 70, 33, 11, 1234);
	}

	clientID = serverConnection->incomingPeerID;
	playerbase->addNewPlayer(keyboardAndMouse, clientID);
	gameplay->connectPlayerbase(playerbase);
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::updateSubsystem(const float& deltaTime)
{
	network.ServiceNetwork(0, [&serverConnection = serverConnection](const ENetEvent& evnt)
	{
		switch (evnt.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			if (evnt.peer == serverConnection)
			{
				//Send a 'hello' packet
				char* text_data = "Connected to server";
				ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
				enet_peer_send(serverConnection, 0, packet);
			}
		}
		break;

		case ENET_EVENT_TYPE_RECEIVE:
		{
			printf("\t Server %d says: %s\n", evnt.peer->incomingPeerID, evnt.packet->data);
			enet_packet_destroy(evnt.packet);
		}
		break;
		}
	});
}
