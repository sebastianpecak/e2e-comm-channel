#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

class ServerConnection
{
public:
    bool Open();
    bool Close();
    bool Send();
    bool Recv();
};

#endif // SERVERCONNECTION_H
