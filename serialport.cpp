#include "serialport.h"
#include <QDebug>

SerialPort::SerialPort(QWidget *parent) :
    QSerialPort(parent)
{
    settings = new SettingsDialog;
    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
    decoder_state = 0;
    packet_count = 0;

    cmd_assembly.packet.header = 0x55aa;
}

SerialPort::~SerialPort()
{
    delete settings;
}

void SerialPort::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    this->setPortName(p.name);
    this->setBaudRate(p.baudRate);
    this->setDataBits(p.dataBits);
    this->setParity(p.parity);
    this->setStopBits(p.stopBits);
    this->setFlowControl(p.flowControl);

    if(!this->open(QIODevice::ReadWrite)) {
        QMessageBox msgBox;
        msgBox.setText(this->errorString());
        msgBox.exec();
    }
}

void SerialPort::closeSerialPort()
{
    if(this->isOpen())
        this->close();
}

void SerialPort::settingSerialPort()
{
    settings->show();
}

void SerialPort::sendCmd(quint16 cmd, qint16 arg_1, float arg_2, float arg_3)
{
    cmd_assembly.packet.cmd_word = cmd;
    cmd_assembly.packet.arg_1 = arg_1;
    cmd_assembly.packet.arg_2 = arg_2;
    cmd_assembly.packet.arg_3 = arg_3;
    quint16 checksum = 0;
    for(int i=0;i<7;i++)
        checksum += cmd_assembly.word[i];
    cmd_assembly.packet.checksum = checksum;
    this->write(cmd_assembly.bytes,16);
}

bool SerialPort::getFloat(float *p, quint16 index)
{
    if(index < PACKET_FLOAT_NUM)
    {
        *p = packet_assembly.packet.data_float[index];
        return true;
    }
    else
        return false;
}

bool SerialPort::getInt16(qint16 *p, quint16 index)
{
    if(index < PACKET_INT16_NUM)
    {
        *p = packet_assembly.packet.data_int16[index];
        return true;
    }
    else
        return false;
}

void SerialPort::readData()
{
    char read_byte[256];
    qint64 bytes_count;
    bytes_count = this->read(read_byte,256);
//    qDebug() << bytes_count;
    for(qint64 i=0;i<bytes_count;i++)
        decodeByte(char(read_byte[i]));

}

void SerialPort::decodeByte(char read_byte)
{
//    qDebug() << decoder_state << read_byte;
    switch(decoder_state){
    case 0: // Standby. Waiting for header's first byte
    {
        if(read_byte == HEADER_FIRST_BYTE)
        {
            decoder_state = 1;
        }
        break;
    }
    case 1: // Waiting for header's second byte. If not received, return to standby state.
    {
        if(read_byte == HEADER_SECOND_BYTE)
        {
            decoder_state = 2;
            payload_byte_counter = 0;
        }
        else
            decoder_state = 0;
        break;
    }
    case 2: // Counter received. Start to receive payload. Switch to next state at the last bytes of payload.
    {
        if(payload_byte_counter<(PAYLOAD_BYTE_SIZE-1))
        {
            packet_assembly.bytes[payload_byte_counter] = read_byte;
            payload_byte_counter++;
        }
        else if(payload_byte_counter==(PAYLOAD_BYTE_SIZE-1)) // Last byte of data, need to change to the next state.
        {
            packet_assembly.bytes[payload_byte_counter] = read_byte;
            decoder_state = 3;
        }
        else
            decoder_state = 0;
        break;
    }
    case 3: // Waiting for checksum's first byte.
    {
        decoder_state = 4;
        break;
    }
    case 4: // Waiting for checksum's second byte.
    {
        decoder_state = 0;
//        qDebug() << packet_count++;
        emit PacketReceived();
        break;
    }
    default:
    {
        decoder_state = 0;
        break;
    }
    }
}
