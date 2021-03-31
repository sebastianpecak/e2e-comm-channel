#include "Storage.h"

void Storage::NewMessage(const TargetMessage &msg)
{
    // Make message object to store.
    // Message class generates unique identifier for message.
    const Message newMsg = Message(msg);
    _storage[msg.targetid()].push_back(newMsg);
    return;
}

std::vector<Message> Storage::GetAllMessages(const std::string &recipient)
{
    return _storage[recipient];
}

void Storage::ConfirmDelivery(const std::string &recipient, const std::string &msgId)
{
    // Find message and mark it as delivered.
    for (auto &msg : _storage[recipient])
    {
        if (msg.GetId() == msgId)
        {
            msg.SetDelivered(true);
            return;
        }
    }
}

void Storage::RemoveMessage(const std::string &recipient, const std::string &msgId)
{
    for (auto &msg : _storage[recipient])
    {
        if (msg.GetId() == msgId)
        {
            msg.SetInvalid(true);
            return;
        }
    }
}
