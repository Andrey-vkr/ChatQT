#ifndef CLIENTQT_H
#define CLIENTQT_H

#include <QTcpSocket>


class ClientQt: public QTcpSocket
{
  Q_OBJECT

  public:
    ClientQt() = delete;
    explicit ClientQt(QString hostIP, int port);

    void requestIsPasswordCorrect(const QString& login,
                                  const QString& passwordHash);
    void requestIsUserRegistered(const QString& login);
    void requestAddUser(const QString& login,
                        const QString& passwordHash);
    void requestUserLogins();
    void requestCommonMessage();
    void requestPrivateMessage(const QString& receiver);
    void sendCommonMessage(const QString& sender,
                           const QString& message);
    void sendPrivateMessage(const QString& sender,
                            const QString& receiver,
                            const QString& message);
    void requestRemoveUser(const QString& login);
    QString getResponse();

  signals:
    void dataReceived();
    void failConnection();

  private:
    void sendRequest(int commandType, const QString& args);
    void getData();

    QString response_;
    QTcpSocket* socket_;
    QByteArray data_;
    quint16 nextBlockSize_;
};


#endif
