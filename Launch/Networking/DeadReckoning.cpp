#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

//KinematicState DeadReckoning::predictState(PhysicsNode* node, float deltaTime)
//{
//	//KinematicState newState = oldState;
//	
//	//newState.position = oldState.position + (oldState.linearVelocity * deltaTime)
//	//	+ ((oldState.linearAcceleration * 0.5f) * (deltaTime * deltaTime));
//}
//
//KinematicState DeadReckoning::blendStates(KinematicState oldState, KinematicState newState, float blendFactor)
//{
//}
void DeadReckoning::predictPosition(PhysicsNode* node, float deltaTime)
{
	float dt = 1.0f / 60.f;
		node->setPosition(node->getPosition() + (node->getLinearVelocity() * dt)
			+ ((node->getAcceleration() * 0.5f) * (dt * dt)));
}