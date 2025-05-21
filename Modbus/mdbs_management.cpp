#include "pch.h"
#include "modbus/mdbs_management.h"
#include "math.h"
MDBS_Management::MDBS_Management(){
    Initialzation();
}
MDBS_Management::~MDBS_Management(){
    Close();
}
void MDBS_Management::Initialzation(){
    memset(&shdr,0,sizeof(MDBS_HDR));
    memset(&rhdr,0,sizeof(MDBS_HDR));
    pSendBody = NULL;
    pRecvBody = NULL;
    SendDataID = NULL;
    CRC = false;
}
void MDBS_Management::Close(){
    DeleteSendBody();
    DeleteRecvBody();
    memset(&shdr,0,sizeof(MDBS_HDR));
    memset(&rhdr,0,sizeof(MDBS_HDR));
    
    SendDataID = NULL;
    RecvDataID = NULL;
    CRC = false;
}
void MDBS_Management::DeleteSendBody()
{
    if (this->pSendBody == NULL)return;
    if(shdr.FUNC_CODE == MDBS_MULTY_COIL_WIRTE) {
        MDBS_MC_WRITE_BODY* p = (MDBS_MC_WRITE_BODY*)pSendBody;
        if (p->DT)delete[](p->DT);
        p->DT = NULL;
    }
    else if(shdr.FUNC_CODE == MDBS_MULTY_REGISTERS_WIRTE){
        MDBS_MR_WRITE_BODY* p = (MDBS_MR_WRITE_BODY*)pSendBody;
        if (p->DT)delete[](p->DT);
        p->DT = NULL;
    }
    if (pSendBody)delete[](pSendBody);
    pSendBody = NULL;
}
void MDBS_Management::DeleteRecvBody()
{
    if (this->pRecvBody == NULL)return;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if(Body->DT)delete[](Body->DT);
        Body->DT = NULL;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (Body->DT)delete[](Body->DT);
        Body->DT = NULL;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (Body->DT)delete[](Body->DT);
        Body->DT = NULL;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (Body->DT)delete[](Body->DT);
        Body->DT = NULL;
    }
    delete(pRecvBody);
    pRecvBody = NULL;
}
void MDBS_Management::SetUnitID(BYTE ID){
    shdr.UNIT_ID = ID;
}

void MDBS_Management::SetFunctionCode(BYTE Code){
    
    shdr.FUNC_CODE = Code;
    switch(Code){
        case MDBS_COIL_READ :
            pSendBody = new MDBS_C_READ_BODY();
            pRecvBody = new MDBS_C_READ_RECV_BODY();
        break;
        case MDBS_DISCRETE_INPUTS_READ :
            pSendBody = new MDBS_DI_READ_BODY();
            pRecvBody = new MDBS_DI_READ_RECV_BODY();
        break;
        case MDBS_HOLDING_REGISTERS_READ :
            pSendBody = new MDBS_HR_READ_BODY();
            pRecvBody = new MDBS_HR_READ_RECV_BODY();
        break;
        case MDBS_INPUT_REGISTERS_READ:
            pSendBody = new MDBS_IR_READ_BODY();
            pRecvBody = new MDBS_IR_READ_RECV_BODY();
        break;
        case MDBS_SINGLE_COIL_WIRTE :
            pSendBody = new MDBS_SC_WRITE_BODY();
            pRecvBody = new MDBS_SC_WRITE_RECV_BODY();
        break;
        case MDBS_SINGLE_REGISTERS_WIRTE:
            pSendBody = new MDBS_SR_WRITE_BODY();
            pRecvBody = new MDBS_SR_WRITE_RECV_BODY();
        break;
        case MDBS_MULTY_COIL_WIRTE :
            pSendBody = new MDBS_MC_WRITE_BODY();
            pRecvBody = new MDBS_MC_WRITE_RECV_BODY();
        break;
        case MDBS_MULTY_REGISTERS_WIRTE :
            pSendBody = new MDBS_MR_WRITE_BODY();
            pRecvBody = new MDBS_MR_WRITE_RECV_BODY();
        break;
    }
}

DWORD MDBS_Management::SetSendQuantity(WORD Quantity){
    BYTE Data[4] = {0,0,0,0};
    DWORD Out;
    switch(shdr.FUNC_CODE){
        case MDBS_COIL_READ :
            SetSendQuantityCR(Quantity);
            return 0;
        break;
        case MDBS_DISCRETE_INPUTS_READ :
            SetSendQuantityDIR(Quantity);
            return 0;
        break;
        case MDBS_HOLDING_REGISTERS_READ :
            SetSendQuantityHRR(Quantity);
            return 0;
        break;
        case MDBS_INPUT_REGISTERS_READ:
            SetSendQuantityIRR(Quantity);
            return 0;
        break;
        case MDBS_SINGLE_COIL_WIRTE :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_SINGLE_COIL_WIRTE ;
        break;
        case MDBS_SINGLE_REGISTERS_WIRTE :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
        case MDBS_MULTY_COIL_WIRTE :
            SetSendQuantityMCS(Quantity);
            return 0;
        break;
        case MDBS_MULTY_REGISTERS_WIRTE:
           SetSendQuantityMRS(Quantity);
           return 0;
        break;
        default:
            Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
            Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out,Data,4);
    return Out;
}
void MDBS_Management::SetSendQuantityCR(WORD Quantity){
    MDBS_C_READ_BODY *body = (MDBS_C_READ_BODY*)pSendBody;
    body->QUANTITY = Quantity;
}
void MDBS_Management::SetSendQuantityDIR(WORD Quantity){
    MDBS_DI_READ_BODY *body = (MDBS_DI_READ_BODY*)pSendBody;
    body->QUANTITY = Quantity;
}
void MDBS_Management::SetSendQuantityHRR(WORD Quantity){
    MDBS_HR_READ_BODY *body = (MDBS_HR_READ_BODY*)pSendBody;
    body->QUANTITY = Quantity;
}
void MDBS_Management::SetSendQuantityIRR(WORD Quantity){
    MDBS_IR_READ_BODY *body = (MDBS_IR_READ_BODY*)pSendBody;
    body->QUANTITY = Quantity;
}
void MDBS_Management::SetSendQuantityMCS(WORD Quantity){
    float size;
    MDBS_MC_WRITE_BODY *bd = (MDBS_MC_WRITE_BODY*)pSendBody;
    bd->QUANTITY = Quantity;
    size = (float)Quantity/8;
    BYTE CNT = (BYTE)(ceil(size));
    shdr.LENGTH = 7+CNT;
    bd->BYTE_CNT = CNT;
    if(bd->BYTE_CNT == 1)   bd->DT = new BYTE;
    else                    bd->DT = new BYTE[bd->BYTE_CNT];
}
void MDBS_Management::SetSendQuantityMRS(WORD Quantity){
    MDBS_MR_WRITE_BODY *bd = (MDBS_MR_WRITE_BODY*)pSendBody;
    shdr.LENGTH = Quantity*2+7;
    bd->QUANTITY = Quantity;
    bd->BYTE_CNT = (BYTE)(Quantity*2);
    if(Quantity == 1)   bd->DT = new WORD;
    else                bd->DT = new WORD[Quantity];
}

DWORD MDBS_Management::SetSendAddress(WORD ID){
    BYTE Data[4] = {0,0,0,0};
    DWORD Out;
    switch(shdr.FUNC_CODE){
        case MDBS_COIL_READ :
            SetSendAddressCR(ID);
            return 0;
        break;
        case MDBS_DISCRETE_INPUTS_READ :
            SetSendAddressDIR(ID);
            return 0;
        break;
        case MDBS_HOLDING_REGISTERS_READ :
            SetSendAddressHRR(ID);
            return 0;
        break;
        case MDBS_INPUT_REGISTERS_READ:
            SetSendAddressIRR(ID);
            return 0;
        break;
        case MDBS_SINGLE_COIL_WIRTE :
            SetSendAddressSCW(ID);
            return 0;
        break;
        case MDBS_SINGLE_REGISTERS_WIRTE :
            SetSendAddressSRW(ID);
            return 0;
        break;
        case MDBS_MULTY_COIL_WIRTE :
            SetSendAddressMCW(ID);
            return 0;
        break;
        case MDBS_MULTY_REGISTERS_WIRTE:
            SetSendAddressMRW(ID);
            return 0;
        break;
        default:
            Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
            Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out,Data,4);
    return Out;
}
void MDBS_Management::SetSendAddressCR(WORD ID){
    MDBS_C_READ_BODY *mcrb = (MDBS_C_READ_BODY*)pSendBody;
    mcrb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressDIR(WORD ID){
    MDBS_DI_READ_BODY *mcrb = (MDBS_DI_READ_BODY*)pSendBody;
    mcrb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressHRR(WORD ID){
    MDBS_HR_READ_BODY *mhrb = (MDBS_HR_READ_BODY*)pSendBody;
    mhrb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressIRR(WORD ID){
    MDBS_IR_READ_BODY *mirb = (MDBS_IR_READ_BODY*)pSendBody;
    mirb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressSCW(WORD ID){
    MDBS_SC_WRITE_BODY *mswb = (MDBS_SC_WRITE_BODY*)pSendBody;
    mswb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressSRW(WORD ID){
    MDBS_SR_WRITE_BODY *mswb = (MDBS_SR_WRITE_BODY*)pSendBody;
    mswb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressMCW(WORD ID){
    MDBS_MC_WRITE_BODY *mmwb = (MDBS_MC_WRITE_BODY*)pSendBody;
    mmwb->ADDRESS = ID;
}
void MDBS_Management::SetSendAddressMRW(WORD ID){
    MDBS_MR_WRITE_BODY *mmwb = (MDBS_MR_WRITE_BODY*)pSendBody;
    mmwb->ADDRESS = ID;
}

DWORD MDBS_Management::SetData(WORD Dt){
    BYTE Data[4] = {0,0,0,0};
    DWORD Out;
    switch(shdr.FUNC_CODE){
        case MDBS_COIL_READ :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_COIL_READ;
        break;
        case MDBS_DISCRETE_INPUTS_READ :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
        case MDBS_HOLDING_REGISTERS_READ :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
        case MDBS_INPUT_REGISTERS_READ:
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
        case MDBS_SINGLE_COIL_WIRTE :
            SetSendDataSCW(Dt);
            return 0;
        break;
        case MDBS_SINGLE_REGISTERS_WIRTE :
            SetSendDataSRW(Dt);
            return 0;
        break;
        case MDBS_MULTY_COIL_WIRTE :
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_MULTY_COIL_WIRTE;
        break;
        case MDBS_MULTY_REGISTERS_WIRTE:
            Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
            Data[3] = MDBS_MULTY_REGISTERS_WIRTE;
        break;
        default:
            Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
            Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out,Data,4);
    return Out;
}
void MDBS_Management::SetSendDataSCW(WORD ID){
    MDBS_SC_WRITE_BODY *mswb = (MDBS_SC_WRITE_BODY*)pSendBody;
    mswb->DT = ID;
}
void MDBS_Management::SetSendDataSRW(WORD ID){
    MDBS_SR_WRITE_BODY* mswb = (MDBS_SR_WRITE_BODY*)pSendBody;
    mswb->DT = ID;
}

DWORD MDBS_Management::SetData(int n, BYTE Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, BYTE Dt)
{
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)Body->DT;
    DATA[n] = Dt;
}
void MDBS_Management::SetSendDataMRW(int n, BYTE Dt)
{
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)Body->DT;
    DATA[n] = Dt;
}

DWORD MDBS_Management::SetData(int n, WORD Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, WORD Dt){
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(WORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
}
void MDBS_Management::SetSendDataMRW(int n, WORD Dt){
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(WORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
}

DWORD MDBS_Management::SetData(int n, DWORD Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, DWORD Dt) {
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(DWORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
    Tep = DATA[2];
    DATA[2] = DATA[3];
    DATA[3] = Tep;
}
void MDBS_Management::SetSendDataMRW(int n, DWORD Dt) {
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(DWORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
    Tep = DATA[2];
    DATA[2] = DATA[3];
    DATA[3] = Tep;
}

DWORD MDBS_Management::SetData(int n, char Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, char Dt)
{
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)Body->DT;
    DATA[n] = Dt;
}
void MDBS_Management::SetSendDataMRW(int n, char Dt)
{
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)Body->DT;
    DATA[n] = Dt;
}

DWORD MDBS_Management::SetData(int n, short Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, short Dt) {
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(WORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
}
void MDBS_Management::SetSendDataMRW(int n, short Dt) {
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(WORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
}

DWORD MDBS_Management::SetData(int n, int Dt)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_COIL_READ;
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_DISCRETE_INPUTS_READ;
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_HOLDING_REGISTERS_READ;
        break;
    case MDBS_INPUT_REGISTERS_READ:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_INPUT_REGISTERS_READ;
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_COIL_WIRTE;
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        Data[2] = MDBS_ERROR_UNAVAILABLE_FUNCTION_CODE;
        Data[3] = MDBS_SINGLE_REGISTERS_WIRTE;
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendDataMCW(n, Dt);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendDataMRW(n, Dt);
        break;
    default:
        Data[2] = MDBS_ERROR_WRONG_FUNCTION_CODE;
        Data[3] = shdr.FUNC_CODE;
        break;
    }
    memmove(&Out, Data, 4);
    return Out;
}
void MDBS_Management::SetSendDataMCW(int n, int Dt) {
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(DWORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
    Tep = DATA[2];
    DATA[2] = DATA[3];
    DATA[3] = Tep;
}
void MDBS_Management::SetSendDataMRW(int n, int Dt) {
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE* DATA = (BYTE*)&Body->DT[n];
    BYTE Tep;
    memmove(DATA, &Dt, sizeof(DWORD));
    Tep = DATA[0];
    DATA[0] = DATA[1];
    DATA[1] = Tep;
    Tep = DATA[2];
    DATA[2] = DATA[3];
    DATA[3] = Tep;
}

void* MDBS_Management::Send(){
    BYTE* DATA = new BYTE[shdr.LENGTH+8];
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    BYTE* p = SetSendHDR(DATA);
    SendLen = shdr.LENGTH + 8;
    switch (shdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        SetSendCR(p);
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        SetSendDIR(p);
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        SetSendHRR(p);
        break;
    case MDBS_INPUT_REGISTERS_READ:
        SetSendIRR(p);
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        SetSendSCW(p);
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        SetSendSRW(p);
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetSendMCW(p);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetSendMRW(p);
        break;
    }
    return DATA;
}
BYTE* MDBS_Management::SetSendHDR(BYTE* p)
{
    BYTE Tep;
    memmove(p, &shdr.TRS_ID, 2);
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &shdr.PRTC_ID, 2);
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &shdr.LENGTH, 2);
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &shdr.UNIT_ID, 2);
    p = &p[1];
    memmove(p, &shdr.FUNC_CODE, 2);
    p = &p[1];
    return p;
}
void MDBS_Management::SetSendCR(BYTE* p) {
    MDBS_C_READ_BODY* Body = (MDBS_C_READ_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, Body, sizeof(MDBS_C_READ_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendDIR(BYTE* p) {
    MDBS_DI_READ_BODY* Body = (MDBS_DI_READ_BODY*)pSendBody;
    BYTE Tep;
    memmove(p,Body, sizeof(MDBS_DI_READ_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendHRR(BYTE* p) {
    MDBS_HR_READ_BODY* Body = (MDBS_HR_READ_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, Body, sizeof(MDBS_HR_READ_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendIRR(BYTE* p) {
    MDBS_IR_READ_BODY* Body = (MDBS_IR_READ_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, Body, sizeof(MDBS_IR_READ_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendSCW(BYTE* p) {
    MDBS_SC_WRITE_BODY* Body = (MDBS_SC_WRITE_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, Body, sizeof(MDBS_SC_WRITE_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendSRW(BYTE* p) {
    MDBS_SR_WRITE_BODY* Body = (MDBS_SR_WRITE_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, Body, sizeof(MDBS_SR_WRITE_BODY));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
}
void MDBS_Management::SetSendMCW(BYTE* p) {
    MDBS_MC_WRITE_BODY* Body = (MDBS_MC_WRITE_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, &Body->ADDRESS, sizeof(WORD));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &Body->QUANTITY, sizeof(WORD));
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &Body->BYTE_CNT, sizeof(BYTE));
    p = &p[1];
    memmove(p, Body->DT, sizeof(BYTE) * Body->BYTE_CNT);
}
void MDBS_Management::SetSendMRW(BYTE* p) {
    MDBS_MR_WRITE_BODY* Body = (MDBS_MR_WRITE_BODY*)pSendBody;
    BYTE Tep;
    memmove(p, &Body->ADDRESS, 2);
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &Body->QUANTITY, 2);
    Tep = p[0];
    p[0] = p[1];
    p[1] = Tep;
    p = &p[2];
    memmove(p, &Body->BYTE_CNT, 1);
    p = &p[1];
    memmove(p, Body->DT, Body->BYTE_CNT);
}

int MDBS_Management::LenSend()
{
    return SendLen;
}

WORD MDBS_Management::RecvHDR(char* buf)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    int i;
    char Tep, * Buf = buf;
    memset(&rhdr, 0, sizeof(MDBS_HDR));
    for (i = 0; i < 3; i++) {
        Tep = Buf[0];
        Buf[0] = Buf[1];
        Buf[1] = Tep;
        Buf = &Buf[2];
    }
    memmove(&rhdr, buf, sizeof(MDBS_HDR));
    if (rhdr.UNIT_ID != shdr.UNIT_ID) {
        Data[1] = MDBS_ERROR_RECV_DIFFERENT_UNIT_ID;
        Data[3] = rhdr.FUNC_CODE;
    }
    if (rhdr.FUNC_CODE <= 0x10){
        if (rhdr.FUNC_CODE != shdr.FUNC_CODE) {
            Data[1] = Data[1] | MDBS_ERROR_RECV_DIFFERENT_FUNCTION_CODE;
            Data[3] = rhdr.FUNC_CODE;
        }
    }
    else {
        Data[1] = MDBS_ERROR_RECV_DATA_ERROR_OCCERRENCE;
        Data[3] = rhdr.FUNC_CODE;
    }
    memmove(&Out, Data, 4);
    return Out;
}

WORD MDBS_Management::GetHDRLenth()
{
    return rhdr.LENGTH-2;
}

WORD MDBS_Management::RecvData(char*p)
{
    BYTE Data[4] = { 0,0,0,0 };
    DWORD Out;
    if (rhdr.FUNC_CODE > 0x10) {
        Data[1] = MDBS_ERROR_RECV_DATA_ERROR_OCCERRENCE;
        Data[3] = rhdr.FUNC_CODE;
        memmove(&Out, Data, 4);
        return Out;
    }
    switch (rhdr.FUNC_CODE) {
    case MDBS_COIL_READ:
        SetRecvDataCR(p);
        break;
    case MDBS_DISCRETE_INPUTS_READ:
        SetRecvDataIRR(p);
        break;
    case MDBS_HOLDING_REGISTERS_READ:
        SetRecvDataHRR(p); 
        break;
    case MDBS_INPUT_REGISTERS_READ:
        SetRecvDataIRR(p);
        break;
    case MDBS_SINGLE_COIL_WIRTE:
        SetRecvDataSCW(p);
        break;
    case MDBS_SINGLE_REGISTERS_WIRTE:
        SetRecvDataSRW(p); 
        break;
    case MDBS_MULTY_COIL_WIRTE:
        SetRecvDataMCW(p);
        break;
    case MDBS_MULTY_REGISTERS_WIRTE:
        SetRecvDataMRW(p);
        break;
    }
    return 0;
}

void MDBS_Management::SetRecvDataCR(char* p)
{
    MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
    Body->BYTE_CNT = p[0];
    Body->DT = new BYTE[Body->BYTE_CNT];
    memmove(Body->DT, &p[1], Body->BYTE_CNT);
}

void MDBS_Management::SetRecvDataDIR(char* p)
{
    MDBS_DI_READ_RECV_BODY *Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
    Body->BYTE_CNT = p[0];
    Body->DT = new BYTE[Body->BYTE_CNT];
    memmove(Body->DT, &p[1], Body->BYTE_CNT);
}

void MDBS_Management::SetRecvDataHRR(char* p)
{
    MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
    int n;
    Body->BYTE_CNT = p[0];
    n = Body->BYTE_CNT / 2;
    Body->DT = new WORD[n];
    memmove(Body->DT, &p[1], Body->BYTE_CNT);
}

void MDBS_Management::SetRecvDataIRR(char* p)
{
    MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
    int n;
    Body->BYTE_CNT = p[0];
    n = Body->BYTE_CNT / 2;
    Body->DT = new WORD[n];
    memmove(Body->DT, &p[1], Body->BYTE_CNT);
}

void MDBS_Management::SetRecvDataSCW(char* p)
{
    MDBS_SC_WRITE_RECV_BODY* Body = (MDBS_SC_WRITE_RECV_BODY*)pRecvBody;
    char Tep = p[0];
    p[0] = p[1];
    p[0] = Tep;
    memmove(&Body->ADDRESS, &p[0],2);
    Tep = p[2];
    p[2] = p[3];
    p[3] = Tep;
    memmove(&Body->DT, &p[2], 2);
}

void MDBS_Management::SetRecvDataSRW(char* p )
{
    MDBS_SR_WRITE_RECV_BODY* Body = (MDBS_SR_WRITE_RECV_BODY*)pRecvBody;
    char Tep = p[0];
    p[0] = p[1];
    p[0] = Tep;
    memmove(&Body->ADDRESS, &p[0], 2);
    Tep = p[2];
    p[2] = p[3];
    p[3] = Tep;
    memmove(&Body->DT, &p[2], 2);
}

void MDBS_Management::SetRecvDataMCW(char* p)
{
    MDBS_MC_WRITE_RECV_BODY* Body = (MDBS_MC_WRITE_RECV_BODY*)pRecvBody;
    char Tep = p[0];
    p[0] = p[1];
    p[0] = Tep;
    memmove(&Body->ADDRESS, &p[0], 2);
    Tep = p[2];
    p[2] = p[3];
    p[3] = Tep;
    memmove(&Body->QUANTITY, &p[2], 2);
}

void MDBS_Management::SetRecvDataMRW(char* p)
{
    MDBS_MR_WRITE_RECV_BODY* Body = (MDBS_MR_WRITE_RECV_BODY*)pRecvBody;
    char Tep = p[0];
    p[0] = p[1];
    p[0] = Tep;
    memmove(&Body->ADDRESS, &p[0], 2);
    Tep = p[2];
    p[2] = p[3];
    p[3] = Tep;
    memmove(&Body->QUANTITY, &p[2], 2);
}

char MDBS_Management::GetChar()
{
    char Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) { 
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0; 
            }
            Out = this->RecvDataID[RecvDataCnt++];
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            Out = RecvDataID[RecvDataCnt++];
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            Out = this->RecvDataID[RecvDataCnt++];
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            Out = RecvDataID[RecvDataCnt++];
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            Out = this->RecvDataID[RecvDataCnt++];
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            Out = RecvDataID[RecvDataCnt++];
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            Out = this->RecvDataID[RecvDataCnt++];
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            Out = RecvDataID[RecvDataCnt++];
        }
        return Out;
    }
    else    return 0;
}
short MDBS_Management::GetShort()
{
    short Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt],2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else    return 0;
}
int MDBS_Management::GetInt()
{
    int Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else    return 0;
}

BYTE MDBS_Management::GetByte()
{
    BYTE Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt+=1;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0; 
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 1);
            RecvDataCnt += 1;
        }
        return Out;
    }
    else    return 0;
}
WORD MDBS_Management::GetWord()
{
    WORD Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }

            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 2);
            RecvDataCnt += 2;
        }
        return Out;
    }
    else    return 0;
}
DWORD MDBS_Management::GetDword()
{
    DWORD Out = 0;
    if (rhdr.FUNC_CODE == MDBS_COIL_READ) {
        MDBS_C_READ_RECV_BODY* Body = (MDBS_C_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        //Body->BYTE_CNT
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_DISCRETE_INPUTS_READ) {
        MDBS_DI_READ_RECV_BODY* Body = (MDBS_DI_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_HOLDING_REGISTERS_READ) {
        MDBS_HR_READ_RECV_BODY* Body = (MDBS_HR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else if (rhdr.FUNC_CODE == MDBS_INPUT_REGISTERS_READ) {
        MDBS_IR_READ_RECV_BODY* Body = (MDBS_IR_READ_RECV_BODY*)pRecvBody;
        if (this->RecvDataID) {
            if (RecvDataCnt >= Body->BYTE_CNT) {
                RecvDataID = NULL;
                RecvDataCnt = 0;
                return 0;
            }
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        else {
            RecvDataID = (BYTE*)Body->DT;
            RecvDataCnt = 0;
            memmove(&Out, &RecvDataID[RecvDataCnt], 4);
            RecvDataCnt += 4;
        }
        return Out;
    }
    else    return 0;
}

