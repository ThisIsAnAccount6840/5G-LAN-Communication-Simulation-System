#include "interface.h"

Interface::Interface(QObject *parent, QString ip, QString name, Ui::MainWindow *u) : QObject(parent)
{
    Interface_IP=ip;
    Interface_Name=name;
    Interface_TEID_List=nullptr;
    ui=u;
}

gNB_N3::gNB_N3(QObject *parent, QString ip, QString name, Ui::MainWindow *u): Interface(parent, ip, name, u){

}

UPF_N3::UPF_N3(QObject *parent, QString ip, QString name, Ui::MainWindow *u): Interface(parent, ip, name, u){

}

UPF_N6::UPF_N6(QObject *parent, QString ip, QString name, Ui::MainWindow *u): Interface(parent, ip, name, u){

}

UPF_N19::UPF_N19(QObject *parent, QString ip, QString name, Ui::MainWindow *u): Interface(parent, ip, name, u){

}

_5G_VN_INTERNAL::_5G_VN_INTERNAL(QObject *parent, QString ip, QString name, Ui::MainWindow *u): Interface(parent, ip, name, u){

}

void gNB_N3::Recev_packet(Data_Packet & data, QString pdu_sid){
    if(data.src_interface == -1 && data.src_ip!="12.1.1.6"){//UE发出的包(UE5除外)，此时的接口是gNB_N3
        qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from UE"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from UE\n");
        data.src_interface=0;//access side
        //寻找与PDU会话ID对应的TEID
        int teid_upf=-1;
        NODE<PDU_Session_Related_Info> *pdu_info;
        for(pdu_info=PDU_Info_List->head->link; pdu_info!=nullptr; pdu_info=pdu_info->link){
            if(pdu_info->info.PDU_Session_ID==pdu_sid) {
                teid_upf=pdu_info->info.TEID;
                break;
            }
        }
        if(teid_upf==-1){
            qDebug()<<"INTERFACE ERROR: PDU SESSION ID NOT FOUND"<<endl;
            ui->Output_Window->append("INTERFACE ERROR: PDU SESSION ID NOT FOUND\n");
            return;
        }
        data.TEID=teid_upf;
        //qDebug()<<"[gNB_N3] uplink packet:"<<endl;
        //data.print_data_packet();
        qDebug()<<"["<<Interface_Name<<"]"<<"send a packet to core network..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to core network...\n");
        delay();
        emit uplink_data(data);//gNB发出的上行数据包src_int=0
        return;
    }
    if(data.src_interface==0){//access side，UPF发来的数据包
        if(Interface_TEID_List->find_node(data.TEID)){//actually useless
            qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from UPF"<<endl;
            ui->Output_Window->append("["+Interface_Name+"] received a packet from UPF\n");
            qDebug()<<"["<<Interface_Name<<"]"<<"send a packet to UE..."<<endl;
            ui->Output_Window->append("["+Interface_Name+"] send a packet to UE...\n");
            delay();
            emit downlink_data(data);
        }
    }
}

void UPF_N3::Recev_packet(Data_Packet &data){
    if(data.src_interface==0){//gNB发出的上行数据包
        if(Interface_TEID_List->find_node(data.TEID)){//actually useless
            if(data.dst_ip=="255.255.255.255" || data.dst_ip=="244.1.1.1"){//接收到广播/组播数据包
                emit broadcast_packet_receive(data.src_ip, 3, data.dst_ip, data.network_instance);
            }
            qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from gNB"<<endl;
            ui->Output_Window->append("["+Interface_Name+"] received a packet from gNB\n");
            qDebug()<<"["<<Interface_Name<<"]"<<"send a packet to core network..."<<endl;
            ui->Output_Window->append("["+Interface_Name+"] send a packet to core network...\n");
            delay();
            emit uplink_data(data, Interface_IP);//UPF_N3将数据包发给UPF内部
        }
        return;
    }
    if(data.src_interface==4){//5G VN internal，从UPF内部发来的包
        qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from UPF"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from UPF\n");
        data.src_interface=0;//access side
        qDebug()<<"["<<Interface_Name<<"]"<<" send a packet to gNB..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to gNB...\n");
        //data.print_data_packet();
        delay();
        emit downlink_data(data, "");
        return;
    }
}

void UPF_N6::Recev_packet(Data_Packet &data, QString sid){
    if(data.src_interface==-1){//UE5发来的数据包
        if(data.dst_ip=="255.255.255.255" || data.dst_ip=="244.1.1.1"){//接收到广播/组播数据包
            emit broadcast_packet_receive(data.src_ip, 6, data.dst_ip, data.network_instance);
        }
        qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from UE"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from UE\n");
        data.src_interface=1;//core side
        qDebug()<<"["<<Interface_Name<<"]"<<"send a packet to core network..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to core network...\n");
        delay();
        emit uplink_data(data);//UPF_N6将数据包发给UPF内部
        return;
    }
    if(data.src_interface==4){//5G VN Internal,从UPF发来的数据包
        qDebug()<<"["<<Interface_Name<<"]"<<" received a packet from UPF"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from UPF\n");
        qDebug()<<"["<<Interface_Name<<"]"<<"send a packet to UE..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to UE\n");
        delay();
        emit downlink_data(data);//UPF_N6将数据包发送给UE5
    }
}

void UPF_N19::Recev_packet(Data_Packet &data){
    if(data.src_interface==4){//5G VN internal,接收到本地UPF发来的包
        qDebug()<<"["<<Interface_Name<<"]"<<"received a packet from local UPF"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from local UPF\n");
        data.src_interface=1;//core side
        qDebug()<<"["<<Interface_Name<<"]"<<" send a packet to another UPF through N19 tunnel..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to another UPF through N19 tunnel...\n");
        delay();
        emit send_data(data);//将包发送给另一个UPF
        return;
    }
    if(data.src_interface==1){//core side，通过N19隧道收到了从另一个UPF发来的包
        if(data.dst_ip=="255.255.255.255" || data.dst_ip=="244.1.1.1"){//接收到广播/组播数据包
            emit broadcast_packet_receive(data.src_ip, 19, data.dst_ip, data.network_instance);
        }
        qDebug()<<"["<<Interface_Name<<"]"<<" received a packet from another UPF through N19 tunnel"<<endl;
        ui->Output_Window->append("["+Interface_Name+"] received a packet from another UPF through N19 tunnel\n");
        qDebug()<<"["<<Interface_Name<<"]"<<" send a packet to local UPF..."<<endl;
        ui->Output_Window->append("["+Interface_Name+"] send a packet to local UPF...\n");
        //data.print_data_packet();
        delay();
        emit recv_data(data, Interface_IP);//通知本地UPF收到了包
    }
}

void _5G_VN_INTERNAL::Recev_packet(Data_Packet &data){
    qDebug()<<"5G VN Internal received a packet"<<endl;
    ui->Output_Window->append("5G VN Internal received a packet\n");
    data.src_interface=4;//5G VN Internal
}
