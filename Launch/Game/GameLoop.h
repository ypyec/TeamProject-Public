#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"




#include <memory>
#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"
#include "../Input/InputManager.h"
#include "GameplaySystem.h"

#include "../../Audio/AudioSystem.h"
#include "../../Physics/PhysicsEngine.h"

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:
	Database* database;
	SceneManager* scene;

	RenderingSystem* rendering;
	InputManager* inputManager;
	GameplaySystem* gameplay;
	AudioSystem* audio;
	PhysicsEngine* physics;

	System engine;
	GameTimer loopTimer;
	Window* window;

	Camera* camera;


	float pitch = 0.0f;
	float yaw   = 0.0f;
};

