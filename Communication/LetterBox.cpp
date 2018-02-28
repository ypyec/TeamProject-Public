#include "LetterBox.h"
#include "DeliverySystem.h"

LetterBox::LetterBox()
{
	messageStorage = MessageStorage();
}

LetterBox::~LetterBox()
{
}

void LetterBox::addDeliveryPoint(const std::string& bufferName)
{
	messageStorage.addMessageBuffer(bufferName);
}

std::queue<Message*>* LetterBox::getDeliveryPoint(const std::string& bufferName)
{
	return messageStorage.getMessageBufferByName(bufferName);
}

void LetterBox::insertMessage(Message message)
{
	messageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(PlayerInputMessage message)
{
	playerInputMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(TextMessage message)
{
	textMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(RelativeTransformMessage message)
{
	relativeTransformMessageBuffer.insertOutgoingMessage(message);
}
void LetterBox::insertMessage(PlaySoundMessage message)
{
	playSoundMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(StopSoundMessage message)
{
	stopSoundMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(ToggleGraphicsModuleMessage message)
{
	toggleGraphicsModuleMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(ApplyForceMessage message)
{
	applyForceMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(MoveCameraRelativeToGameObjectMessage message)
{
	moveCameraBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(ApplyImpulseMessage message)
{
	applyImpulseMessageBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(CollisionMessage message)
{
	collisionBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(PreparePaintSurfaceMessage message)
{
	preparePaintSurfaceBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(PaintTrailForGameObjectMessage message)
{
	paintTrailForGameObjectBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(UpdatePositionMessage message)
{
	updatePositionBuffer.insertOutgoingMessage(message);
}

void LetterBox::insertMessage(TextMeshMessage message)
{
	textMeshBuffer.insertOutgoingMessage(message);
}

void LetterBox::deliverAllMessages()
{
	messageBuffer.sendMessages(messageStorage);
	playerInputMessageBuffer.sendMessages(messageStorage);
	textMessageBuffer.sendMessages(messageStorage);
	relativeTransformMessageBuffer.sendMessages(messageStorage);
	playSoundMessageBuffer.sendMessages(messageStorage);
	stopSoundMessageBuffer.sendMessages(messageStorage);
	toggleGraphicsModuleMessageBuffer.sendMessages(messageStorage);
	applyForceMessageBuffer.sendMessages(messageStorage);
	moveCameraBuffer.sendMessages(messageStorage);
	collisionBuffer.sendMessages(messageStorage);
	preparePaintSurfaceBuffer.sendMessages(messageStorage);
	applyImpulseMessageBuffer.sendMessages(messageStorage);
	paintTrailForGameObjectBuffer.sendMessages(messageStorage);
	updatePositionBuffer.sendMessages(messageStorage);
	textMeshBuffer.sendMessages(messageStorage);
}

void LetterBox::clearAllMessages()
{
	messageStorage.clearMessageStorage();

	messageBuffer.clearSentMessages();
	playerInputMessageBuffer.clearSentMessages();
	textMessageBuffer.clearSentMessages();
	relativeTransformMessageBuffer.clearSentMessages();
	playSoundMessageBuffer.clearSentMessages();
	stopSoundMessageBuffer.clearSentMessages();
	toggleGraphicsModuleMessageBuffer.clearSentMessages();
	applyForceMessageBuffer.clearSentMessages();
	applyImpulseMessageBuffer.clearSentMessages();
	moveCameraBuffer.clearSentMessages();
	collisionBuffer.clearSentMessages();
	preparePaintSurfaceBuffer.clearSentMessages();
	paintTrailForGameObjectBuffer.clearSentMessages();
	updatePositionBuffer.clearSentMessages();
	textMeshBuffer.clearSentMessages();
}
