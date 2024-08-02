#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>
#include "WrapperSHA1.h"
#include "InputValidator.h"


LoginWindow::LoginWindow(QWidget* parent) :
  QDialog(parent), ui(new Ui::LoginWindow)
{
  ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
  delete ui;
}

void LoginWindow::setClient(ClientQt* client)
{
  client_ = client;
}

void LoginWindow::handleServerResponse_IsPasswordCorrect()
{
  auto isCorrectPassword = client_->getResponse();
  disconnectNetworkSignal_IsPasswordCorrect();

  if (isCorrectPassword == "true")
  {
    auto login = ui->lineEdit_Login->text().toStdString();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Password->clear();

    emit toChat(login);
  }
  else
  {
    QMessageBox::critical(this, "Error", "Incorrect password, try again");
    return;
  }
}

void LoginWindow::on_pushButton_Registration_clicked()
{
  ui->lineEdit_Login->clear();
  ui->lineEdit_Password->clear();

  emit toRegistration();
}

void LoginWindow::on_buttonBox_accepted()
{
  auto login = ui->lineEdit_Login->text().toStdString();
  if (login.empty())
  {
    QString message = "Enter login!";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputValidator::isContainsForbidenChars(login))
  {
    QString message = "Login contains invalid characters. Valid characters: 'a'-'z', 'A'-'Z', '0'-'9'";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputValidator::isStartsByNumber(login))
  {
    QString message = "Login cannot start with a number";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputValidator::isTooShort(login))
  {
    QString message = "Login too short. Min length: " +
                      QString::number(InputValidator::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto password = ui->lineEdit_Password->text().toStdString();

  if (password.empty())
  {
    QString message = "Enter password!";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputValidator::isTooShort(password))
  {
    QString message = "Password too short. Min length: " +
                      QString::number(InputValidator::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto hashPassword = cipher::sha1Hash(password);
  connectNetworkSignal_IsPasswordCorrect();
  client_->requestIsPasswordCorrect(QString::fromStdString(login),
                                    QString::fromStdString(hashPassword));
}

void LoginWindow::on_buttonBox_rejected()
{
  emit close();
}

void LoginWindow::connectNetworkSignal_IsPasswordCorrect()
{
  connect(client_, &ClientQt::dataReceived,
          this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}

void LoginWindow::disconnectNetworkSignal_IsPasswordCorrect()
{
  disconnect(client_, &ClientQt::dataReceived,
              this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}
