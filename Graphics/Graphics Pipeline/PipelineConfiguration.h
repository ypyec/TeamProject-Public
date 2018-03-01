#pragma once

#include "Graphics Settings/SSAO.h"
#include "Graphics Settings/Shadows.h"
#include "Graphics Settings/BPLighting.h"
#include "Graphics Settings/GBuffer.h"
#include "Graphics Settings/Bloom.h"
#include "Graphics Settings/MotionBlur.h"
#include "../Utilities/Maths/Vector2.h"
#include "Graphics Settings/Skybox.h"
#include "Graphics Settings/Shadows.h"
#include "Effects/ParticleSystem.h"

#include "GraphicsPipeline.h"
#include "../Scene Management/SceneManager.h"

#include <vector>
#include "Graphics Settings/BasicGeometry.h"
#include "Graphics Settings/UIModule.h"
#include "Graphics Settings/PaintTrail.h"
#include "Graphics Settings/GameText.h"
#include "Graphics Settings/ScoreCounter.h"

class Database;
class Camera;
class Window;

class PipelineConfiguration
{
public:
	PipelineConfiguration();
	PipelineConfiguration(SceneManager* sceneManager, Window* window, 
		Camera* camera, Vector2 resolution);
	~PipelineConfiguration();

	void initialiseModules(Matrix4 projmatrix, Matrix4 orthographicMatrix, Database* database);
	void buildPipeline(GraphicsPipeline* pipeline);

private:
	bool initialised;

	Vector2	resolution;
	Camera*	camera;
	Window* window;
	SceneManager* sceneManager;

	BasicGeometry* basicGeom;
	GBuffer *gBuffer;
	Skybox* skybox;
	SSAO* ssao;
	Shadows* shadowTex;
	BPLighting *bpLighting;
	UIModule* uiModule;
	PaintTrail* paintTrail;
	GameText* gameText;
	ScoreCounter* scoreCounter;
};

