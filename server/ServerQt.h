#ifndef SERVERQT_H
#define SERVERQT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include "CommandHandler.h"
#include "Database.h"


class ServerQt: public QTcpServer
{
  Q_OBJECT

  public:
    ServerQt() = delete;
    ServerQt(uint16_t port, Database* database);
    ~ServerQt();

  public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readData();

  private:
    void sendToClient(QTcpSocket* socketClient, const QString& string);

    CommandHandler* handler_;

    QTcpSocket* socket_;
    QVector<QTcpSocket*> clientSockets_;
    QByteArray data_;
    quint16 incomingDataSize_;
};

#endif
