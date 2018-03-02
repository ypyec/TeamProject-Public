#pragma once

#include "../GraphicsModule.h"
#include "../../Shaders/ComputeShader.h"
#include "../../Meshes/TextMesh.h"

class Database;

class ScoreCounter : public GraphicsModule
{
public:
	ScoreCounter(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, Database* database);
	~ScoreCounter();

	void bufferScoreHolder(std::string scoreHoldername);

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint* paintTrailTexture;
private:
	void calculateScores();
	void displayScores();
	void locateUniforms() override;

	ComputeShader* computeShader;
	Shader* textShader;
	Font* font;
	Database* database;

	GLuint playerScoresSSBO;
	GLuint coloursSSBO;
	GLuint redCounter;
	GLuint yellowCounter;

	std::vector<int> scores;
	std::vector<Vector4> coloursToCount;
	std::vector<std::string> scoreHolders;
};
