#include "GameObject.h"

#include "../Physics/PhysicsNode.h"

GameObject::GameObject()
{
	setSize(sizeof(*this));
}

GameObject::~GameObject()
{
}

void GameObject::setSceneNode(SceneNode * sceneNode)
{
	this->sceneNode = sceneNode;
}

void GameObject::setPhysicsNode(PhysicsNode * physicsNode)
{
	this->physicsNode = physicsNode;
}

SceneNode * GameObject::getSceneNode()
{
	return sceneNode;
}

PhysicsNode * GameObject::getPhysicsNode()
{
	return physicsNode;
}

void GameObject::updatePosition()
{
}

void GameObject::setPosition(Vector3 position)
{
	this->sceneNode->SetTransform(position);
	//set physics position
}

void GameObject::setRotation()
{
}

void GameObject::setScale(Vector3 scale)
{
	this->sceneNode->SetModelScale(scale);
	//set physics scale
}


