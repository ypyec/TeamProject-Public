#include "ButtonActionCreator.h"

#include "UserInterfaceBuilder.h"
#include "../Communication/DeliverySystem.h"
#include "../Resource Management/XMLParser.h"

ButtonActionCreator::ButtonActionCreator()
{
	actions.insert({ "None", []()
	{
	} });

	actions.insert({"PrintText", []()
	{
		std::cout << "Button pressed" << std::endl;
	} });

	actions.insert({ "Quit", []()
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "Quit"));
	} });

	actions.insert({ "DisableSSAO", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "SSAO", false));
	} });

	actions.insert({ "EnableSSAO", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "SSAO", true));
	} });

	actions.insert({ "EnableSkybox", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Skybox", true));
	} });

	actions.insert({ "DisableSkybox", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Skybox", false));
	} });

	actions.insert({ "EnableShadows", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Shadows", true));
	} });

	actions.insert({ "DisableShadows", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Shadows", false));
	} });
}

ButtonActionCreator::~ButtonActionCreator()
{
}

ButtonAction ButtonActionCreator::createButtonAction(Node* actionNode)
{
	if (actionNode->name == "Start")
	{
		return [node = actionNode]()
		{
			DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "Start " + node->children[0]->value + " " + node->children[1]->value));
			DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "UIModule", false));
		};
	}
	else 
	{
		return actions.at(actionNode->value);
	}
}