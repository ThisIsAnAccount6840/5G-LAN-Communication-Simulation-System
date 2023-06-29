#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <QString>
#include <QDebug>

class Data_Packet
{
public:
    Data_Packet(QString="null", QString="null", int=-1);
    //IP Header
    QString src_ip;
    QString dst_ip;
    //UDP header?
    int src_interface;
    //GTP-U Header
    int TEID;

    QString T_PDU;//User data
    QString network_instance;//VN组名

    void print_data_packet();
};

#endif // DATA_PACKET_H
