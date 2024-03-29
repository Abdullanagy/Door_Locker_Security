 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Abdulla Nagy
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"
#include"util/delay.h"

uint8 EEPROM_writeArray(uint16 u16addr, uint8 *u8data ,uint8 array_size)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;


    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
    for ( int i=0 ; i<array_size ; i++ ) {
    /* write byte to eeprom */
    TWI_writeByte(*u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
    u8data++;
    _delay_ms(10);
}
    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

uint8 EEPROM_readArray(uint16 u16addr, uint8 *u8data,uint8 array_size)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    for ( int i=1 ; i<array_size ; i++ ) {

    	/* Read Byte from Memory with send ACK */
    	    *u8data = TWI_readByteWithACK();
    	    if (TWI_getStatus() != TWI_MR_DATA_ACK)
    	        return ERROR;
    	    u8data++;
    	    _delay_ms(10);

    }


    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
