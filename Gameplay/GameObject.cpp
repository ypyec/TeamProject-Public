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
	sceneNode->setParent(this);
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
	NCLMatrix4 newTransform = this->physicsNode->getWorldSpaceTransform();
	newTransform = newTransform * NCLMatrix4::scale(scale);

	this->sceneNode->SetTransform(newTransform);
}

void GameObject::setPosition(NCLVector3 position)
{
	this->position = position;
	this->sceneNode->SetTransform(position);

	if(this->physicsNode != nullptr)
	{
		this->physicsNode->setPosition(position);
	}
}

void GameObject::setRotation(NCLVector4 rotation)
{
	NCLVector3 position = sceneNode->GetTransform().getPositionVector();
	NCLVector3 scale = sceneNode->GetTransform().getScalingVector();
	sceneNode->axisAngleRotation = rotation;

	this->sceneNode->SetTransform(NCLMatrix4::translation(position) * 
		NCLMatrix4::rotation(rotation.w, NCLVector3(rotation.x, rotation.y, rotation.z)) * 
		NCLMatrix4::scale(scale));
	
	if (this->physicsNode != nullptr)
	{
		this->physicsNode->setRotation(rotation);
	}
}

void GameObject::setScale(NCLVector3 scale)
{
	this->scale = scale;
	this->sceneNode->SetModelScale(scale);
}

void GameObject::setEnabled(bool isEnabled)
{
	this->isEnabled = isEnabled;
	this->physicsNode->setEnabled(isEnabled);
	this->sceneNode->setEnabled(isEnabled);
}
