#pragma once

#include "../Communication/Message.h"
#include "../Audio/Sound.h"
#include "../Audio/SoundNode.h"

class PlaySoundMessage : public Message
{
public:
	PlaySoundMessage(const std::string& desinationName, MessageType type, Vector3 position,
		 SoundPriority priority, float volume, float radius, float pitch, bool isLooping,
		bool isGlobal, std::string soundID, std::string soundNodeIdentifier);
	~PlaySoundMessage();

	Vector3 position;
	SoundPriority priority;
	float volume;
	float radius;
	float pitch;
	bool isLooping;
	bool isGlobal;
	std::string soundID;
	std::string soundNodeIdentifier;
};