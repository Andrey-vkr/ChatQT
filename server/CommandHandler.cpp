#include "CommandHandler.h"


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

CommandHandler::CommandHandler(Database* database): database_(database){}

QString CommandHandler::handle(int commandType, const QString& args)
{
  QString response;
  switch(commandType)
  {
    case IS_PASSWORD_CORRECT:
      response = isPasswordCorrect(args);
      break;
    case IS_USER_REGISTERED:
      response = isUserRegistered(args);
      break;
    case ADD_USER:
      response = addUser(args);
      break;
    case ADD_COMMON_MESSAGE:
      response = addCommonMessage(args);
      break;
    case ADD_PRIVATE_MESSAGE:
      response = addPrivateMessage(args);
      break;
    case GET_COMMON_MESSAGE:
      response = getCommonMessage();
      break;
    case GET_PRIVATE_MESSAGE:
      response = getPrivateMessage(args);
      break;
    case GET_USER_LOGINS:
      response = getUserLogins();
      break;
    case REMOVE_USER:
      response = removeUser(args);
      break;
    default:
      break;
  }
  return response;
}

QString CommandHandler::isPasswordCorrect(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->isPasswordCorrect(login, passwordHash);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}

QString CommandHandler::isUserRegistered(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  auto isOk = database_->isUserRegistered(login);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}

QString CommandHandler::addUser(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->addUser(login, passwordHash);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}

QString CommandHandler::addCommonMessage(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string message = listArgs.at(1).toStdString();

  auto isOk = database_->addCommonMessage(sender, message);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}

QString CommandHandler::addPrivateMessage(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string receiver = listArgs.at(1).toStdString();
  const std::string message = listArgs.at(2).toStdString();

  auto isOk = database_->addPrivateMessage(sender, receiver, message);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}

QString CommandHandler::getCommonMessage()
{
  auto messages = database_->getCommonMessages();
  auto response = QString::fromStdString(messages);
  return response;
}

QString CommandHandler::getPrivateMessage(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string receiver = listArgs.at(0).toStdString();
  auto messages = database_->getPrivateMessages(receiver);
  auto response = QString::fromStdString(messages);
  return response;
}

QString CommandHandler::getUserLogins()
{
  auto users = database_->getLogins();
  QString response;
  for (const auto& user : users)
  {
    response += QString::fromStdString(user) + "|";
  }
  return response;
}

QString CommandHandler::removeUser(const QString& args)
{
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  auto isOk = database_->removeUser(login);
  QString response = "false";
  if (isOk)
  {
    response = "true";
  }
  return response;
}
