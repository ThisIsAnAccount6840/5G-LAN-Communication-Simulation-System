#include "sendpacket_window.h"

SendPacket_Window::SendPacket_Window(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Send Packet");
    this->setFixedSize(600,550);

    unicast.setParent(this);
    unicast.setText("Unicast");
    unicast.move(20,0);

    multicast.setParent(this);
    multicast.setText("Multicast");
    multicast.move(220,0);

    broadcast.setParent(this);
    broadcast.setText("Broadcast");
    broadcast.move(420,0);

    content_label.setParent(this);
    content_label.setText("Packet Content:");
    content_label.move(20,200);
    content_label.hide();

    content.setParent(this);
    content.setPlaceholderText("Please input the content to be transmitted.");
    content.move(20, 225);
    content.hide();

    start.setParent(this);
    start.setText("Start to send packet");
    start.move(30,500);

    //unicast buttons
    unicast_buttons[0]=&ue0_grp1;
    unicast_buttons[1]=&ue1_grp1;
    unicast_buttons[2]=&ue2_grp1;
    unicast_buttons[3]=&ue0_grp2;
    unicast_buttons[4]=&ue1_grp2;
    int i;
    for(i=0;i<5;i++) member_ips[i]="null";
    ue0_grp1.setParent(this);
    ue0_grp1.setText("null");
    ue0_grp1.hide();
    ue1_grp1.setParent(this);
    ue1_grp1.setText("null");
    ue1_grp1.hide();
    ue2_grp1.setParent(this);
    ue2_grp1.setText("null");
    ue2_grp1.hide();
    ue0_grp2.setParent(this);
    ue0_grp2.setText("null");
    ue0_grp2.hide();
    ue1_grp2.setParent(this);
    ue1_grp2.setText("null");
    ue1_grp2.hide();

    //broadcast buttons
    group1_select.setParent(this);
    group1_select.setText("VN_Group1");
    group1_select.move(180,100);
    group1_select.hide();
    group2_select.setParent(this);
    group2_select.setText("VN_Group2");
    group2_select.move(180,150);
    group2_select.hide();

    //Multicast texts
    prompt.setParent(this);
    prompt.setText("UEs in the multicast group:");
    prompt.move(200,80);
    prompt.hide();
    member1.setParent(this);
    member1.setText("UE1");
    member1.move(200,130);
    member1.hide();
    member2.setParent(this);
    member2.setText("UE2");
    member2.move(250,130);
    member2.hide();
    member3.setParent(this);
    member3.setText("UE5");
    member3.move(300,130);
    member3.hide();

}
