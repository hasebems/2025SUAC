/* ========================================
 *  peripheral.h
 *
 *	Copyright(c) 2025- Masahiko Hasebe at Kigakudoh
 *  This software is released under the MIT License, see LICENSE.txt
 * ========================================
*/
#ifndef I2CDEVICE_H
#define I2CDEVICE_H

void wireBegin( void );
void initHardware( void );

// AT42QT
void AT42QT_init( void );
int AT42QT_read( size_t key, uint8_t (&rdraw)[2], bool ref );
uint8_t AT42QT_read_bit();

#endif
