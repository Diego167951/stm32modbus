/***************************************************************
*�ٵ�ѧԺ ����ʵ����
*���繤��ϵ NERV
*30-Apr-2012
*���� 13589226458
*QQ 357017598 
FreeModbus ͨ�� 
eMBRegInputCB eMBRegHoldingCB 
eMBRegCoilsCB	eMBRegDiscreteCB �ĸ��ӿں���������ݵĶ�д���� 
**************************************************************/
#include "stm32f10x.h"
#include "mb.h" 
void ENTER_CRITICAL_SECTION(void)//���볬�ٽ� �����ж�
{
	__set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION(void)//�˳����ٽ� �����ж�
{
	__set_PRIMASK(0);
}


u16 usRegInputBuf[10]={0x0000,0xfe02,0x1203,0x1304,0x1405,0x1506,0x1607,0x1708,0x1809};
u16 *usRegHoldingBuf=usRegInputBuf;	//һ�������õ� �Ĵ������� ��ַ0-7

u8 REG_INPUT_START=0,REG_HOLDING_START=0;
u8 REG_INPUT_NREGS=8,REG_HOLDING_NREGS=8;
u8 usRegInputStart=0,usRegHoldingStart=0;

//�����ּĴ��� ������0x04

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );

        while( usNRegs > 0 )
         {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );//PDU�������reg�ĸ�λ��
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );//�������λ��
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}
// �Ĵ����Ķ�д���� ֧�ֵ�����Ϊ�� 0x03 ��д0x06

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
            *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
	    iRegIndex++;
             usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
             usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
             usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
             iRegIndex++;
             usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

//��/д���ؼĴ���  0x01  x05

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

//�����ؼĴ��� 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
