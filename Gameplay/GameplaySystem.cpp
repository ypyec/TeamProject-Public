#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Players/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Input/InputUtility.h"
#include "../Resource Management/XMLParser.h"

GameplaySystem::GameplaySystem(const int playersInGame)
	: Subsystem("Gameplay")
{
	InputActionMap actions(0);
	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_W"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(0, 0, 1)));
	}); 

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_A"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(-1, 0, 0)));
	});

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_S"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(0, 0, -1)));
	});

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_D"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(1, 0, 0)));
	});

	inputBridge = GameplayInputBridge(playersInGame);
	inputBridge.addInputActionMapForPlayer(actions);

	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT }, 
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	XMLParser xmlParser;
	xmlParser.loadFile("../Resources/Gameplay/gameplay.xml");
	gameLogic = GameLogic(&incomingMessages);
	gameLogic.compileParsedXMLIntoScript(xmlParser.parsedXml);

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [&gameLogic = gameLogic, &inputBridge = inputBridge](Message* message)
	{
		gameLogic.notifyMessageActions("PlayerInputMessage", message);
		inputBridge.processPlayerInputMessage(*static_cast<PlayerInputMessage*>(message));
	});
}

GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
	gameLogic.executeMessageBasedActions();
	gameLogic.executeTimeBasedActions(gameplayTimer.getMillisecondsSinceStart() * 0.001f, deltaTime * 0.001f);
	gameLogic.clearNotifications();
}
