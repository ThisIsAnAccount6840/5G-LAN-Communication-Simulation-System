#ifndef UE_SUB_DATA_NODE_H
#define UE_SUB_DATA_NODE_H

#include "UE_Sub_Data.h"

class UE_Sub_Data_Node
{
public:
    UE_Sub_Data_Node();
    UE_Sub_Data ue_sub_data;
    UE_Sub_Data_Node *next;
};

#endif // UE_SUB_DATA_NODE_H
