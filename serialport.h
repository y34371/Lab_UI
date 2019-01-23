#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include "settingsdialog.h"

#define HEADER_FIRST_BYTE 85
#define HEADER_SECOND_BYTE -86

#define PACKET_INT16_NUM 8
#define PACKET_FLOAT_NUM 4
#define PACKET_BYTE_SIZE (4 + 4*PACKET_FLOAT_NUM + 2*PACKET_INT16_NUM)
#define PAYLOAD_BYTE_SIZE (4*PACKET_FLOAT_NUM + 2*PACKET_INT16_NUM)

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    SerialPort(QWidget *);
    ~SerialPort();
    void openSerialPort();
    void closeSerialPort();
    void settingSerialPort();

    void sendCmd(quint16 cmd, qint16 arg_1, float arg_2, float arg_3);


    bool getFloat(float *,quint16 index);
    bool getInt16(qint16 *, quint16 index);

signals:
    void PacketReceived();

private slots:
    void readData();

private:
    struct SerialPacket {
        qint16 data_int16[PACKET_INT16_NUM];
        float data_float[PACKET_FLOAT_NUM];
    };
    union PacketAssembly
    {
        struct SerialPacket packet;
        char bytes[PACKET_BYTE_SIZE];
//        char bytes[12];
    } packet_assembly;

    struct CmdPacket {
        quint16 header;
        quint16 cmd_word;
        float arg_2;
        float arg_3;
        qint16 arg_1;
        quint16 checksum;
    };
    union CmdAssembly
    {
        struct CmdPacket packet;
        quint16 word[8];
        char bytes[16];
    } cmd_assembly;

    void decodeByte(char read_byte);

    int decoder_state;
    quint16 payload_byte_counter;
    quint16 packet_count;

    SettingsDialog *settings;
    QByteArray packet_buffer;
};

#endif // LIBRASERIALPORT_H
