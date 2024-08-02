#include "RegistrationWindow.h"
#include "ui_RegistrationWindow.h"
#include <QMessageBox>
#include "WrapperSHA1.h"
#include "InputValidator.h"


RegistrationWindow::RegistrationWindow(QWidget* parent) :
  QDialog(parent), ui(new Ui::RegistrationWindow)
{
  ui->setupUi(this);
}

RegistrationWindow::~RegistrationWindow()
{
  delete ui;
}

void RegistrationWindow::setClient(ClientQt* client)
{
  client_ = client;
}

void RegistrationWindow::handleServerResponse_IsUserRegistered()
{
  auto isUserRegistered = client_->getResponse();
  disconnectNetworkSignal_IsUserRegistered();

  if (isUserRegistered == "true")
  {
    QString message = "This user is already registered";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  connectNetworkSignal_IsUserAdded();

  auto hashPassword = cipher::sha1Hash(password_.toStdString());
  connectNetworkSignal_IsUserAdded();
  client_->requestAddUser(login_, QString::fromStdString(hashPassword));
}

void RegistrationWindow::handleServerResponse_IsUserAdded()
{
  auto isUserAdded = client_->getResponse();
  disconnectNetworkSignal_IsUserAdded();

  if (isUserAdded == "true")
  {
    auto login = ui->lineEdit_Login->text().toStdString();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword->clear();
    emit toChat(login);
  }
  else
  {
    QString message = "Failed to add user";
    QMessageBox::critical(this, "Error", message);
    return;
  }
}

void RegistrationWindow::on_pushButton_toLogin_clicked()
{
  ui->lineEdit_Login->clear();
  ui->lineEdit_Password->clear();
  ui->lineEdit_ConfirmPassword->clear();
  emit toLogin();
}

void RegistrationWindow::on_buttonBox_accepted()
{
  login_.clear();
  password_.clear();

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
  if (InputValidator::isTooShort(login)){
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

  auto confirmPassword = ui->lineEdit_ConfirmPassword->text().toStdString();
  if (confirmPassword.empty())
  {
    QString message = "Confirm password!";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (password != confirmPassword)
  {
    QString message = "The passwords entered do not match";
    QMessageBox::critical(this, "Error", "The passwords entered do not match");
    return;
  }

  login_ = QString::fromStdString(login);
  password_ = QString::fromStdString(password);

  checkIsUserRegistered(login_);
}

void RegistrationWindow::on_buttonBox_rejected()
{
  emit close();
}

void RegistrationWindow::connectNetworkSignal_IsUserRegistered()
{
  connect(client_, &ClientQt::dataReceived,
          this, &RegistrationWindow::handleServerResponse_IsUserRegistered);
}

void RegistrationWindow::disconnectNetworkSignal_IsUserRegistered()
{
  disconnect(client_, &ClientQt::dataReceived,
              this, &RegistrationWindow::handleServerResponse_IsUserRegistered);
}

void RegistrationWindow::connectNetworkSignal_IsUserAdded()
{
  connect(client_, &ClientQt::dataReceived,
          this, &RegistrationWindow::handleServerResponse_IsUserAdded);
}

void RegistrationWindow::disconnectNetworkSignal_IsUserAdded()
{
  disconnect(client_, &ClientQt::dataReceived,
              this, &RegistrationWindow::handleServerResponse_IsUserAdded);
}

void RegistrationWindow::checkIsUserRegistered(const QString& login)
{
  connectNetworkSignal_IsUserRegistered();
  client_->requestIsUserRegistered(login);
}

