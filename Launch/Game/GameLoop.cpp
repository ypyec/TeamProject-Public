#include "GameLoop.h"
#include "../../Input/InputManager.h"
#include "../../Input/GamePadRecorder.h"
#include "../../Input/KeyboardMouseRecorder.h"
#include "../../Resource Management/Level.h"
#include <iostream>
#include "Communication/LetterBox.h"
#include "../../Gameplay/GameObject.h"

GameLoop::GameLoop(System& gameSystem)
{
	engine = gameSystem;
	window = new Window("Game Window", 1280, 720);
	window->lockMouseToWindow(true);

	//MUST BE REMOVED
	camera = new Camera(0, 0, Vector3(0, 0, 0));

	rendering = new RenderingSystem(window, camera, Vector2(1280, 720));

	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	//nodes->push_back(node);
	scene = new SceneManager(camera, nodes);

	rendering->SetSceneToRender(scene);

	InputRecorder* keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	PlayerBase* playerbase = new PlayerBase();
	playerbase->addNewPlayer(keyboardAndMouse);

	std::string seperator = "|";
	std::string keyboard = "KEYBOARD_W|KEYBOARD_A";
	std::string xbox = "XBOX_A|XBOX_B";
	std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

	inputManager = new InputManager(playerbase);
	gameplay = new GameplaySystem();

	engine.addSubsystem(gameplay);
	engine.addSubsystem(inputManager);
	engine.addSubsystem(rendering);
	
	Database database;

	TableCreation tableCreation(&database);

	Level level(&database);
	level.loadLevelFile("TestLevel.txt");

	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("playerBall"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("wall1"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("wall2"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("wall3"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("wall4"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getAllResources()->getResource("floor"))->getSceneNode());
}

GameLoop::~GameLoop()
{
	delete window;
	delete rendering;
	delete inputManager;
}

void GameLoop::executeGameLoop()
{
	int frameCount = 0;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer.getTimeSinceLastRetrieval();

		engine.updateNextSystemFrame(deltaTime);

		DeliverySystem::getPostman()->deliverAllMessages();
		engine.processMessagesForAllSubsystems();

		DeliverySystem::getPostman()->clearAllMessages();

		pitch -= (window->getMouse()->getRelativePosition().y);
		yaw -= (window->getMouse()->getRelativePosition().x);

		if (window->getKeyboard()->keyDown(KEYBOARD_W)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_S)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_A)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(-1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_D)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_SPACE)) {
			camera->setPosition(camera->getPosition() + Vector3(0, 1, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_C)) {
			camera->setPosition(camera->getPosition() + Vector3(0, -1, 0) * 1);
		}

		camera->setPitch(pitch);
		camera->setYaw(yaw);
	}

}
