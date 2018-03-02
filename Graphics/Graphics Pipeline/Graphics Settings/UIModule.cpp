#include "UIModule.h"

#include "../../GraphicsCommon.h"
#include "../Resource Management/Database/Database.h"
#include "UserInterfaceDisplay.h"
#include <iterator>

UIModule::UIModule(const std::string identifier, const Vector2 resolution,
	Database* database) : GraphicsModule(identifier, resolution)
{
	font = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	tex = SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	UITextShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"UITextFrag.glsl");

	this->database = database;
}

UIModule::~UIModule()
{
}

void UIModule::initialise()
{
}

void UIModule::apply()
{
	UIObjects = UserInterfaceDisplay::getInterface()->getAllButtonsInMenu();

	setCurrentShader(UIShader);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	viewMatrix.toIdentity();
	textureMatrix.toIdentity();

	updateShaderMatrices();
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&CommonGraphicsData::SHARED_ORTHOGRAPHIC_MATRIX);

	for each (Button* button in *UIObjects)
	{
		glUniform4fv(glGetUniformLocation(UIShader->GetProgram(), "colour"), 1, (float*)&button->colour);

		button->UIMesh->Draw(*currentShader, Matrix4::translation(button->position) * Matrix4::scale(button->scale));

		if (button->childrenEnabled)
		{
			renderButtons(button->childButtons);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	setCurrentShader(UITextShader);
	updateShaderMatrices();
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&CommonGraphicsData::SHARED_ORTHOGRAPHIC_MATRIX);

	Vector3 colour(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "colour"), 1, (float*)&colour);

	for each (Button* button in *UIObjects)
	{
		Vector2 buttonSize = button->scale / Vector2(4.0f, 1.5f);
		Vector2 offset(button->scale.x, 0.0f);
		button->textMesh->Draw(*currentShader, Matrix4::translation(button->position - offset) * Matrix4::scale(Vector3(buttonSize.x, buttonSize.y, 1)));

		if (button->childrenEnabled)
		{
			renderButtonsText(button->childButtons);
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void UIModule::linkShaders()
{
	UIShader->LinkProgram();
	UITextShader->LinkProgram();
}

void UIModule::regenerateShaders()
{
	UIShader->Regenerate();
}

void UIModule::locateUniforms()
{
}

void UIModule::renderButtons(std::vector<Button>& buttons)
{
	for each (Button button in buttons)
	{
		glUniform4fv(glGetUniformLocation(UIShader->GetProgram(), "colour"), 1, (float*)&button.colour);

		button.UIMesh->Draw(*currentShader, Matrix4::translation(button.position) * Matrix4::scale(button.scale));

		if (button.childrenEnabled)
		{
			renderButtons(button.childButtons);
		}
	}
}

void UIModule::renderButtonsText(std::vector<Button>& buttons)
{
	for each (Button button in buttons)
	{
		Vector2 buttonSize = button.scale / Vector2(4.0f, 1.5f);
		Vector2 offset(button.scale.x, 0.0f);
		button.textMesh->Draw(*currentShader, Matrix4::translation(button.position - offset) * Matrix4::scale(Vector3(buttonSize.x, buttonSize.y, 1)));

		if (button.childrenEnabled)
		{
			renderButtonsText(button.childButtons);
		}
	}
}
