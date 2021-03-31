#ifndef STORAGE_H
#define STORAGE_H

#include "Message.h"
#include <unordered_map>
#include <vector>

// template <typename T>
// class Global
// {
// public:
//     /**
//      * Returns global instance of given class.
//      */
//     static T& Get();
// };

// #define GLOBAL(class) Global<class>::Get()

// template <typename T>
// class NamedInstance
// {
// public:
//     static T& Get(const std::string &name);
// };

// #define NAMED_INSTANCE(class, name) NamedInstance<class>::Get(name)

class Storage
{
    /**
     * This is internal storage object.
     * To make it persistent it needs to be implemented using SQLite or other file-storage.
     */
    std::unordered_map<std::string, std::vector<Message>> _storage;

public:
   //Storage() : array() {}
    /**
     * Called on new message received by server - to be relayed to other user.
     */
    void NewMessage(const TargetMessage &msg);
    /**
     * Returns array of all messages intended for given recipient.
     * Each message has its own identity so later it can be marked as delived.
     */
    std::vector<Message> GetAllMessages(const std::string &recipient);
    /**
     * Mark message of given identity as delivered.
     * Message then can be removed from server's storage.
     */
    void ConfirmDelivery(const std::string &recipient, const std::string &msgId);
    /**
     * This function should be called when client responded with
     * MSG_NOK which indicates that message is invalid.
     */
    void RemoveMessage(const std::string &recipient, const std::string &msgId);
};

// void test()
// {
//     GLOBAL(Storage).NewMessage(TargetMessage());
//     NAMED_INSTANCE(Storage, "test_instance").GetAllMessages("wioled");
// }

#endif // STORAGE_H
