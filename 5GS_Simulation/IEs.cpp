#include "IEs.h"

bool PDI::operator==(PDI & pdi){
    if(pdi.Local_FTEID.IP_Address==Local_FTEID.IP_Address && pdi.Local_FTEID.TEID==Local_FTEID.TEID && pdi.Network_Instance==Network_Instance && pdi.Source_Interface==Source_Interface && pdi.UE_IP_Address==UE_IP_Address) return true;
    else return false;
}

void PDI::print_pdi(){
    qDebug()<<"Source Interface:"<<Source_Interface<<endl;
    qDebug()<<"F-TEID: "<<Local_FTEID.TEID<<","<<Local_FTEID.IP_Address<<endl;
    qDebug()<<"UE IP Address: "<<UE_IP_Address<<endl;
    qDebug()<<"Network Instance: "<<Network_Instance<<endl;
}

void Create_FAR::print_far(){
    qDebug()<<"FAR ID: "<<FAR_ID<<endl;
    qDebug()<<"Apply Action: "<<Apply_Action<<endl;
    qDebug()<<"Forwarding Parameter: "<<endl;
    qDebug()<<" Dest Interface in forwarding parameter: "<<forwarding_parameters.Destination_Interface<<endl;
    qDebug()<<" Outer Header Creation in forwarding parameter: "<<forwarding_parameters.Outer_Header_Creation.TEID<<","<<forwarding_parameters.Outer_Header_Creation.IP_Address<<endl;
    qDebug()<<"Duplicating Parameter: "<<endl;
    qDebug()<<" Dest Interface in duplicating parameter: "<<duplicating_parameters.Destination_Interface<<endl;
    qDebug()<<" Outer Header Creation in duplicating parameter: "<<duplicating_parameters.Outer_Header_Creation.TEID<<","<<duplicating_parameters.Outer_Header_Creation.IP_Address<<endl;
}

void Create_PDR::print_PDR(){
    qDebug()<<"PRIUEAI:"<<rep_detc_info.PRIUEAI<<", PRIN6I:"<<rep_detc_info.PRIN6I<<", PRIN19I:"<<rep_detc_info.PRIN19I<<endl;
}

Create_FAR::Create_FAR(){
    FAR_ID=-1;
    Apply_Action="00";
    forwarding_parameters.Destination_Interface=-1;
    forwarding_parameters.Outer_Header_Creation.IP_Address="null";
    forwarding_parameters.Outer_Header_Creation.TEID=-1;
    duplicating_parameters.Destination_Interface=-1;
    duplicating_parameters.Outer_Header_Creation.IP_Address="null";
    duplicating_parameters.Outer_Header_Creation.TEID=-1;
}

Create_PDR::Create_PDR(){
    PDR_ID=-1;
    FAR_ID=-1;
}

PDI::PDI(){
    Source_Interface=-1;
    Local_FTEID.IP_Address="null";
    Local_FTEID.TEID=-1;
    UE_IP_Address="null";
    Network_Instance="null";
}

Packet_Replication_and_Detection_CarryOn_Info::Packet_Replication_and_Detection_CarryOn_Info(){
    PRIUEAI=false;
    PRIN19I=false;
    PRIN6I=false;
    DCARONI=true;
}
