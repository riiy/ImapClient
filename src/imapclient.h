#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QtNetwork/QSslSocket>
#include <QFileInfo>
#include <QByteArray>
#include <QTimer>
#include <QEventLoop>
#include <QMetaEnum>
#include <QObject>
#include "imap_global.h"

class IMAPCLIENT_EXPORT ImapClient : public QObject
{

    Q_OBJECT
    Q_ENUMS (auth_method_t)

public:
    enum auth_method_t{LOGIN, START_TLS};
    ImapClient(const QString & host = "localhost", int port = 25, auth_method_t method = auth_method_t::LOGIN);
    QString getHost() const;
    int getPort() const;
    QTcpSocket* getSocket();
    /**
    Sending the logout command and closing the connection.
    **/
    virtual ~ImapClient();

    ImapClient(const ImapClient&) = delete;

    ImapClient(ImapClient&&) = delete;

    void operator=(const ImapClient&) = delete;

    void operator=(ImapClient&&) = delete;

    /**
    Authenticating with the given credentials.
    The method should be called only once on an existing object - it is not possible to authenticate again within the same connection.
    @param username Username to authenticate.
    @param password Password to authenticate.
    @param method   Authentication method to use.
    @return         The server greeting message.
    @throw *        `connect()`, `auth_login(const string&, const string&)`.
    **/
    void authenticate(const QString& username, const QString& password);

protected:
    QTcpSocket *socket;
    const QString host;
    const int port;
    auth_method_t method;
    /**
    Initiating a session to the server.
    @return           The server greeting message.
    @throw imap_error Connection to server failure.
    @throw imap_error Parsing failure.
    @throw *          `parse_tag_result(const string&)`, `dialog::receive()`.
    @todo             Add server error messages to exceptions.
    **/
    void connect_host();

    /**
    Performing an authentication by using the login method.
    @param username   Username to authenticate.
    @param password   Password to authenticate.
    @throw imap_error Authentication failure.
    @throw imap_error Parsing failure.
    @throw *          `parse_tag_result(const string&)`, `dialog::send(const string&)`, `dialog::receive()`.
    @todo             Add server error messages to exceptions.
    **/
    void auth_login(const QString& username, const QString& password);
    void sendMessage(const QString &text);
};

#endif // IMAPCLIENT_H
