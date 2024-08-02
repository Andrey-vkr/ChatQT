#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QString>
#include "Database.h"

class CommandHandler
{
  public:
    CommandHandler() = delete;
    explicit CommandHandler(Database* database);
    QString handle(int commandType, const QString& args);

  private:
    QString isPasswordCorrect(const QString& args);
    QString isUserRegistered(const QString& args);
    QString addUser(const QString& args);
    QString addCommonMessage(const QString& args);
    QString addPrivateMessage(const QString& args);
    QString getCommonMessage();
    QString getPrivateMessage(const QString& args);
    QString getUserLogins();
    QString removeUser(const QString& args);
    Database* database_;
};

#endif
