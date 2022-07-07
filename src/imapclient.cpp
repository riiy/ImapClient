#include "imapclient.h"

ImapClient::ImapClient(const QString & host, int port)
{
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
