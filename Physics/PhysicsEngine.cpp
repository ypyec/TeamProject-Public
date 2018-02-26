#include "PhysicsEngine.h"
#include "CollisionDetectionSAT.h"

#include "OctreePartitioning.h"

#include "../Communication/Messages/ApplyForceMessage.h"
#include "../Communication/Messages/CollisionMessage.h"

PhysicsEngine::PhysicsEngine(Database* database) : Subsystem("Physics")
{
	this->database = database;

	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::RELATIVE_TRANSFORM, MessageType::APPLY_FORCE, MessageType::APPLY_IMPULSE };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("Physics"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::APPLY_FORCE, [database, &incomingMessages = incomingMessages](Message* message)
	{
		MessageProcessor* m = &incomingMessages;

		ApplyForceMessage* applyForceMessage = static_cast<ApplyForceMessage*>(message);

		GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(applyForceMessage->gameObjectID));

		gObj->getPhysicsNode()->setAppliedForce(applyForceMessage->force);
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::APPLY_IMPULSE, [database](Message* message)
	{
		ApplyImpulseMessage* applyImpulseMessage = static_cast<ApplyImpulseMessage*>(message);

		GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(applyImpulseMessage->gameObjectID));

		gObj->getPhysicsNode()->applyImpulse(applyImpulseMessage->impulse);
	});

	updateTimestep = 1.0f / 60.f;
	updateRealTimeAccum = 0.0f;
}


PhysicsEngine::~PhysicsEngine()
{
	removeAllPhysicsObjects();
}

void PhysicsEngine::addPhysicsObject(PhysicsNode * obj)
{
	if (octreeInitialised)
	{
		octreeChanged = true;
		obj->movedSinceLastBroadPhase = true;
		octree->AddNode(obj);
	}

	physicsNodes.push_back(obj);

	if (obj->transmitCollision)
	{
		obj->setOnCollisionCallback([](PhysicsNode* this_obj, PhysicsNode* colliding_obj, CollisionData collisionData)
		{
			if (!this_obj->hasTransmittedCollision)
			{
				DeliverySystem::getPostman()->insertMessage(CollisionMessage("Gameplay", collisionData,
					this_obj->getParent()->getName(), colliding_obj->getParent()->getName()));
				this_obj->hasTransmittedCollision = true;
			}
			return true;
		});
	}

	obj->setOnUpdateCallback(std::bind(
		&PhysicsEngine::OctreeChanged,
		this,
		std::placeholders::_1));
}

void PhysicsEngine::removePhysicsObject(PhysicsNode * obj)
{
	//Lookup the object in question
	auto found_loc = std::find(physicsNodes.begin(), physicsNodes.end(), obj);

	//If found, remove it from the list
	if (found_loc != physicsNodes.end())
	{
		physicsNodes.erase(found_loc);
	}
}

void PhysicsEngine::removeAllPhysicsObjects()
{
	for (Constraint* c : constraints)
	{
		delete c;
	}
	constraints.clear();

	for (Manifold* m : manifolds)
	{
		delete m;
	}
	manifolds.clear();


	for (PhysicsNode* obj : physicsNodes)
	{
		if (obj->getParent())
			obj->getParent()->setPhysicsNode(nullptr);
		delete obj;
	}
	physicsNodes.clear();
}

void PhysicsEngine::updateSubsystem(const float& deltaTime)
{
	const int maxUpdatesPerFrame = 5;
	
	updateRealTimeAccum += deltaTime;
	for (int i = 0; (updateRealTimeAccum >= updateTimestep) && i < maxUpdatesPerFrame; ++i)
	{
		updateRealTimeAccum -= updateTimestep;

		updatePhysics();
	}

	if (updateRealTimeAccum >= updateTimestep)
	{
		updateRealTimeAccum = 0.0f;
	}

	for each (PhysicsNode* physicsNode in physicsNodes)
	{
		physicsNode->hasTransmittedCollision = false;
	}
}

void PhysicsEngine::updatePhysics()
{
	for (Manifold* m : manifolds)
	{
		delete m;
	}
	manifolds.clear();

	
	//A whole physics engine in 6 simple steps =D

	//-- Using positions from last frame --
	//1. Broadphase Collision Detection (Fast and dirty)
	broadPhaseCollisions();
	
	//2. Narrowphase Collision Detection (Accurate but slow)
	narrowPhaseCollisions();
	
	//3. Initialize Constraint Params (precompute elasticity/baumgarte factor etc)	
	for (Manifold* m : manifolds)
	{
		m->preSolverStep(updateTimestep);
	}
	for (Constraint* c : constraints)
	{
		c->preSolverStep(updateTimestep);
	}


	//4. Update Velocities
	for (PhysicsNode* obj : physicsNodes) 
	{
		obj->integrateForVelocity(updateTimestep);
	}
	
	//5. Constraint Solver
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i) 
	{
		for (Manifold* m : manifolds) m->applyImpulse();
		for (Constraint* c : constraints) c->applyImpulse(updateTimestep);
	}
	
	//6. Update Positions (with final 'real' velocities)
	for (PhysicsNode* obj : physicsNodes)
	{
		obj->integrateForPosition(updateTimestep);
	}
	
}

void PhysicsEngine::broadPhaseCollisions()
{
	if (physicsNodes.size() > 0)
	{
		if (octreeChanged)
		{
			octree->UpdateTree();
			octreeChanged = false;
			broadphaseColPairs = octree->GetAllCollisionPairs();
		}
	}

	//broadphaseColPairs.clear();

	//PhysicsNode* nodeA;
	//PhysicsNode* nodeB;

	//if (physicsNodes.size() > 0)
	//{
	//	for (size_t i = 0; i < physicsNodes.size() - 1; ++i)
	//	{
	//		for (size_t j = i + 1; j < physicsNodes.size(); j++)
	//		{
	//			nodeA = physicsNodes[i];
	//			nodeB = physicsNodes[j];

	//			if (nodeA->getCollisionShape() && nodeB->getCollisionShape())
	//			{
	//				CollisionPair pair;
	//				pair.pObjectA = nodeA;
	//				pair.pObjectB = nodeB;

	//				if (pair.pObjectA->getIsCollision() && pair.pObjectB->getIsCollision())
	//				{
	//					broadphaseColPairs.push_back(pair);
	//				}
	//			}
	//		}
	//	}
	//}
}

void PhysicsEngine::narrowPhaseCollisions()
{
	if (broadphaseColPairs.size() > 0)
	{
		CollisionData colData;

		CollisionDetectionSAT colDetect;

		for (size_t i = 0; i < broadphaseColPairs.size(); ++i)
		{
			CollisionPair& cp = broadphaseColPairs[i];

			CollisionShape *shapeA = cp.pObjectA->getCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->getCollisionShape();

			colDetect.beginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->getCollisionShape(),
				cp.pObjectB->getCollisionShape());

			
			if (colDetect.areColliding(&colData))
			{
				bool okA = cp.pObjectA->fireOnCollisionEvent(cp.pObjectA, cp.pObjectB, colData);
				bool okB = cp.pObjectB->fireOnCollisionEvent(cp.pObjectB, cp.pObjectA, colData);

				if (okA && okB)
				{
					Manifold* manifold = new Manifold();
					manifold->initiate(cp.pObjectA, cp.pObjectB);

					colDetect.genContactPoints(manifold);

					if (manifold->contactPoints.size() > 0) 
					{
						manifolds.push_back(manifold);
					}
					else {
						delete manifold;
					}
				}
			}
		}

	}
}

void PhysicsEngine::InitialiseOctrees(int entityLimit)
{
	octree = new OctreePartitioning(physicsNodes, Vector3(600, 400, 600), Vector3(0, 0, 0));
	octree->ENTITY_PER_PARTITION_THRESHOLD = entityLimit;

	if (physicsNodes.size() > 0)
	{
		octree->BuildInitialTree();
	}

	octreeChanged = false;
	octreeInitialised = true;

	for each (PhysicsNode* node in physicsNodes)
	{
		node->movedSinceLastBroadPhase = false;
	}

	broadphaseColPairs = octree->GetAllCollisionPairs();
}