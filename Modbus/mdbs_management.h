#ifndef __MDBS_MANAGEMENT_H__
#define __MDBS_MANAGEMENT_H__
#include "Modbus/mdbs.h"

class MDBS_Management{
public:
    MDBS_Management();
    ~MDBS_Management();
public:
    void Initialzation();
    void Close();
    void DeleteSendBody();
    void DeleteRecvBody();
public:
    MDBS_HDR shdr;
    MDBS_HDR rhdr;
    void *pSendBody;
    void *pRecvBody;
    BOOL CRC;
private:
    void *SendDataID;
public:
    void SetUnitID(BYTE);
    void SetFunctionCode(BYTE);
    DWORD SetSendQuantity(WORD);
        void SetSendQuantityCR(WORD);
        void SetSendQuantityDIR(WORD);
        void SetSendQuantityHRR(WORD);
        void SetSendQuantityIRR(WORD);
        void SetSendQuantityMCS(WORD);
        void SetSendQuantityMRS(WORD);
    DWORD SetSendAddress(WORD);
        void SetSendAddressCR(WORD);
        void SetSendAddressDIR(WORD);
        void SetSendAddressHRR(WORD);
        void SetSendAddressIRR(WORD);
        void SetSendAddressSCW(WORD);
        void SetSendAddressSRW(WORD);
        void SetSendAddressMCW(WORD);
        void SetSendAddressMRW(WORD);
    DWORD SetData(WORD);
        void SetSendDataSCW(WORD);
        void SetSendDataSRW(WORD);
    DWORD SetData(int,BYTE);
        void SetSendDataMCW(int,BYTE);
        void SetSendDataMRW(int,BYTE);
    DWORD SetData(int,WORD);
        void SetSendDataMCW(int,WORD);
        void SetSendDataMRW(int,WORD);
    DWORD SetData(int,DWORD);
        void SetSendDataMCW(int,DWORD);
        void SetSendDataMRW(int,DWORD);
    DWORD SetData(int,char);
        void SetSendDataMCW(int,char);
        void SetSendDataMRW(int,char);
    DWORD SetData(int,short);
        void SetSendDataMCW(int,short);
        void SetSendDataMRW(int,short);
    DWORD SetData(int,int);
        void SetSendDataMCW(int,int);
        void SetSendDataMRW(int,int);
    void *Send();
private:
    int SendLen;
private:
    BYTE* SetSendHDR(BYTE*);
    void SetSendCR(BYTE*);
    void SetSendDIR(BYTE*);
    void SetSendHRR(BYTE*);
    void SetSendIRR(BYTE*);
    void SetSendSCW(BYTE*);
    void SetSendSRW(BYTE*);
    void SetSendMCW(BYTE*);
    void SetSendMRW(BYTE*);
public:
    int LenSend();
public:
    BYTE* RecvDataID;
    int RecvDataCnt;
public:
    WORD RecvHDR(char*);
    WORD GetHDRLenth();
    WORD RecvData(char*);
    void SetRecvDataCR(char*);
    void SetRecvDataDIR(char*);
    void SetRecvDataHRR(char*);
    void SetRecvDataIRR(char*);
    void SetRecvDataSCW(char*);
    void SetRecvDataSRW(char*);
    void SetRecvDataMCW(char*);
    void SetRecvDataMRW(char*);
    char GetChar();
    short GetShort();
    int GetInt();
    BYTE GetByte();
    WORD GetWord();
    DWORD GetDword();
};
#endif