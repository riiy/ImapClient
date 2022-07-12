#include "imapclient.h"

ImapClient::ImapClient(const QString & host, int port, auth_method_t method):host(host), port(port), method(method)
{
    switch (method)
    {
    case ImapClient::LOGIN:
        socket = new QTcpSocket(this);
        break;
    case ImapClient::START_TLS:
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        break;
    }
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
    waitForEvent(3000, SIGNAL(readyConnected()));
}

void ImapClient::authenticate(const QString& username, const QString& password){

        // Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
    qDebug() << username;
        sendMessage("AUTH PLAIN " + QByteArray().append((char) 0).append(username.toUtf8())
                    .append((char) 0).append(password.toUtf8()).toBase64());
}

QTcpSocket* ImapClient::getSocket(){
    return this->socket;
}
void ImapClient::sendMessage(const QString &text)
{

#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] OUT:" << text;
#endif

    socket->flush();
    socket->write(text.toUtf8() + "\r\n");
}

void ImapClient::socketReadyRead()
{
    QString responseLine;

    if (!socket->isOpen()) {
    qDebug() << "[Socket] error";
        return;
    }

    while (socket->canReadLine()) {
        // Save the server's response
        responseLine = socket->readLine();
        tempResponse += responseLine;

#ifndef QT_NO_DEBUG
        qDebug() << "[Socket] IN: " << responseLine;
#endif
    }
 // Is this the last line of the response
    if (responseLine[3] == ' ') {
        responseText = tempResponse;
        tempResponse = "";

        // Extract the respose code from the server's responce (first 3 digits)
        responseCode = responseLine.left(3).toInt();

        // Check for server error
        if (responseCode / 100 == 4) {
        qDebug() << "[Socket] IN: " << responseCode;
            return;
        }

        // Check for client error
        if (responseCode / 100 == 5) {
        qDebug() << "[Socket] IN: " << responseCode;
            return;
        }

        qDebug() << responseText;
    }
}
void ImapClient::socketStateChanged(QAbstractSocket::SocketState state) {

#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] State:" << state;
#endif
    qDebug() << state;
    switch (state)
    {
    case QAbstractSocket::ConnectedState:
        sendMessage("EHLO localhost");

        break;
    case QAbstractSocket::UnconnectedState:
        break;
    default:
        sendMessage("EHLO localhost");
        ;
    }
}

void ImapClient::socketError(QAbstractSocket::SocketError socketError) {
#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] ERROR:" << socketError;
#else
    Q_UNUSED(socketError);
#endif
    qDebug() << "[Socket] ERROR:" << socketError;
}

void ImapClient::socketEncrypted() {
        sendMessage("EHLO localhosts");
}

void ImapClient::waitForEvent(int msec, const char *successSignal)
{
    QEventLoop loop;
    QObject::connect(this, successSignal, &loop, SLOT(quit()));
    QObject::connect(this, SIGNAL(error(SmtpClient::SmtpError)), &loop, SLOT(quit()));

    if(msec > 0)
    {
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start(msec);
    }

    loop.exec();
}
