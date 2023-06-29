#include "data_packet.h"

Data_Packet::Data_Packet(QString s_ip, QString d_ip, int teid)
{
    src_ip=s_ip;
    dst_ip=d_ip;
    TEID=teid;
}

void Data_Packet::print_data_packet(){
    qDebug()<<"*****PRINT PACKET HEADER*****"<<endl;
    qDebug()<<"SRC IP: "<<src_ip;
    qDebug()<<"DST IP: "<<dst_ip;
    qDebug()<<"SRC Interface: "<<src_interface;
    qDebug()<<"TEID: "<<TEID;
    qDebug()<<"Network Instance: "<<network_instance<<endl;
}
