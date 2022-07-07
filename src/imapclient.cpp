#include "imapclient.h"

ImapClient::ImapClient(const QString & host, int port, auth_method_t method):host(host), port(port), method(method)
{
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));

}
QString ImapClient::getHost() const
{
    return this->host;
}

/**
 * @brief Return the port.
 */
int ImapClient::getPort() const
{
    return this->port;
}

ImapClient::~ImapClient(){

}

void ImapClient::connect_host(){
        switch (method)
        {
        case LOGIN:
            socket->connectToHost(host, port);
            break;
        case START_TLS:
            ((QSslSocket*) socket)->connectToHostEncrypted(host, port);
            break;
        }
}

void ImapClient::authenticate(const QString& username, const QString& password){

        // Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
        sendMessage("AUTH PLAIN " + QByteArray().append((char) 0).append(username.toUtf8())
                    .append((char) 0).append(password.toUtf8()).toBase64());
}

void ImapClient::sendMessage(const QString &text)
{

#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] OUT:" << text;
#endif

    socket->flush();
    socket->write(text.toUtf8() + "\r\n");
}
