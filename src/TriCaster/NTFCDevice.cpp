#include "NTFCDevice.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>

NTFCDevice::NTFCDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      port(port), connected(false), address(address)
{
    this->socket = new QTcpSocket(this);

    //QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

NTFCDevice::~NTFCDevice()
{
}

void NTFCDevice::connectDevice()
{
    if (this->connected)
        return;

    this->socket->connectToHost(this->address, this->port);

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void NTFCDevice::disconnectDevice()
{
    this->socket->blockSignals(true);
    this->socket->disconnectFromHost();
    this->socket->blockSignals(false);

    this->connected = false;

    sendNotification();
}

void NTFCDevice::setConnected()
{
    this->connected = true;

    sendNotification();
}

void NTFCDevice::setDisconnected()
{
    this->connected = false;

    sendNotification();

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

bool NTFCDevice::isConnected() const
{
    return this->connected;
}

int NTFCDevice::getPort() const
{
    return this->port;
}

const QString &NTFCDevice::getAddress() const
{
    return this->address;
}

void NTFCDevice::writeMessage(const QString& message)
{
    if (this->connected)
    {
        std::wstring destination = QString("UI|shortcuts:response").toStdWString();
        std::wstring messageData = message.toStdWString();

        const int destinationSize = (destination.length() + 1) * sizeof(wchar_t);
        const int messageDataSize = (messageData.length() + 1) * sizeof(wchar_t);

        // Setup the transission header.
        const TcpMessageHeader tcpMessageHeader = { 1, 0, destinationSize, sizeof(MessageHeader) + messageDataSize };

        // Setup the message header.
        const MessageHeader messageHeader = { 2, messageDataSize };

        this->socket->write((char*)&tcpMessageHeader, sizeof(tcpMessageHeader)); // Send tcp header.
        this->socket->write((char*)destination.c_str(), destinationSize);        // Send destination.
        this->socket->write((char*)&messageHeader, sizeof(messageHeader));       // Send message header.
        this->socket->write((char*)messageData.c_str(), messageDataSize);        // Send message.
    }
}

/*
void NTFCDevice::readMessage()
{
    while (this->socket->bytesAvailable() && this->socket->canReadLine())
    {
        this->line += QString::fromUtf8(this->socket->readLine());
        if (this->line.endsWith("\r\n"))
        {
            if (this->line == "\r\n")
                this->line.remove(QRegExp("\r\n"));

            QStringList lines = this->line.split(QRegExp("\r\n"));
            foreach (QString line, lines)
                parseLine(line);

            this->line.clear();
        }
    }
}

void NTFCDevice::parseHeader(const QString& line)
{
}

void NTFCDevice::parseMultiline(const QString& line)
{
    sendNotification();
}

void NTFCDevice::resetDevice()
{
    this->response.clear();
    this->state = NTFCDevice::ExpectingHeader;
}
*/
