#include "ClientQt.h"
#include <QDataStream>

namespace
{
  enum Command
{
    IS_PASSWORD_CORRECT,
    IS_USER_REGISTERED,
    ADD_USER,
    ADD_COMMON_MESSAGE,
    ADD_PRIVATE_MESSAGE,
    GET_COMMON_MESSAGE,
    GET_PRIVATE_MESSAGE,
    GET_USER_LOGINS,
    REMOVE_USER
  };
}

ClientQt::ClientQt(QString hostIP, int port)
{
  socket_ = new QTcpSocket(this);
  connect(socket_, &QTcpSocket::readyRead,
          this, &ClientQt::getData);
  connect(socket_, &QTcpSocket::disconnected,
          socket_, &QTcpSocket::deleteLater);
  nextBlockSize_ = 0;
  response_.clear();
  socket_->connectToHost(hostIP, port);
}

void ClientQt::requestIsPasswordCorrect(const QString& login,
                                      const QString& passwordHash)
{
  QString args = login + "|" + passwordHash;
  sendRequest(IS_PASSWORD_CORRECT, args);
}

void ClientQt::requestIsUserRegistered(const QString& login)
{
  sendRequest(IS_USER_REGISTERED, login);
}

void ClientQt::requestAddUser(const QString& login,
                            const QString& passwordHash)
{
  QString args = login + "|" + passwordHash;
  sendRequest(ADD_USER, args);
}

void ClientQt::requestUserLogins()
{
  sendRequest(GET_USER_LOGINS, "");
}

void ClientQt::requestCommonMessage()
{
  sendRequest(GET_COMMON_MESSAGE, "");
}

void ClientQt::requestPrivateMessage(const QString& receiver)
{
  sendRequest(GET_PRIVATE_MESSAGE, receiver);
}

void ClientQt::sendCommonMessage(const QString& sender,
                               const QString& message)
{
  QString args = sender + "|" + message;
  sendRequest(ADD_COMMON_MESSAGE, args);
}

void ClientQt::sendPrivateMessage(const QString& sender,
                                const QString& receiver,
                                const QString& message)
{
  QString args = sender + "|" + receiver + "|" + message;
  sendRequest(ADD_PRIVATE_MESSAGE, args);
}

void ClientQt::requestRemoveUser(const QString& login)
{
  sendRequest(REMOVE_USER, login);
}

QString ClientQt::getResponse()
{
  auto result = response_;
  response_.clear();
  return result;
}

void ClientQt::sendRequest(int commandType, const QString& args)
{
  if (socket_->state() != QAbstractSocket::ConnectedState)
  {
    emit failConnection();
    return;
  }

  data_.clear();
  QDataStream out(&data_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_5_6);
  out << quint16(0) << commandType << args;
  out.device()->seek(0);
  out << quint16(data_.size() - sizeof(quint16));
  socket_->write(data_);
}

void ClientQt::getData()
{
  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_5_6);
  if (in.status() == QDataStream::Ok)
  {
    for(;;)
    {
      if (nextBlockSize_ == 0)
      {
        if (socket_->bytesAvailable() < 2)
        {
          break;
        }
        in >> nextBlockSize_;
      }
      if (socket_->bytesAvailable() < nextBlockSize_)
      {
        break;
      }
      in >> response_;
      nextBlockSize_ = 0;
    }
  }

  emit dataReceived();
}
