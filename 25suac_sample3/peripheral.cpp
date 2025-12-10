/* ========================================
 *  peripheral.h
 *
 *	Copyright(c) 2025- Masahiko Hasebe at Kigakudoh
 *  This software is released under the MIT License, see LICENSE.txt
 * ========================================
*/
#include	"Arduino.h"
#include  <Wire.h>
#include  <avr/pgmspace.h>

#include	"peripheral.h"

//---------------------------------------------------------
//		Initialize I2C Device
//---------------------------------------------------------
void wireBegin( void )
{
  Wire.setClock(400000);
  Wire.setSDA(4);
  Wire.setSCL(5);
	Wire.begin();
}
//---------------------------------------------------------
//		Write I2C Device
//    Err Code
//      0:success
//      1:data too long to fit in transmit buffer
//      2:received NACK on transmit of address
//      3:received NACK on transmit of data
//      4:other error
//---------------------------------------------------------
int write_i2cDevice( unsigned char adrs, unsigned char* buf, int count )
{
	Wire.beginTransmission(adrs);
  Wire.write(buf,count);
	return Wire.endTransmission();
}
//---------------------------------------------------------
//		Read 1byte I2C Device
//---------------------------------------------------------
int read1byte_i2cDevice( unsigned char adrs, unsigned char* wrBuf, unsigned char* rdBuf, int wrCount )
{
	unsigned char err;

	Wire.beginTransmission(adrs);
  Wire.write(wrBuf,wrCount);
	err = Wire.endTransmission(false);
	if ( err != 0 ){ return err; }

	err = Wire.requestFrom(adrs,(uint8_t)1,(uint8_t)0);
	while(Wire.available()) {
		*rdBuf = Wire.read();
	}

	//err = Wire.endTransmission(true);
	//return err;
  return 0;
}
//---------------------------------------------------------
//		Read N byte I2C Device
//---------------------------------------------------------
//送信結果 (byte) 
//0: 成功 
//1: 送ろうとしたデータが送信バッファのサイズを超えた 
//2: スレーブ・アドレスを送信し、NACKを受信した 
//3: データ・バイトを送信し、NACKを受信した 
//4: その他のエラー 
//
int read_nbyte_i2cDevice( unsigned char adrs, unsigned char* wrBuf, unsigned char* rdBuf, int wrCount, int rdCount )
{
	unsigned char err;

	Wire.beginTransmission(adrs);
  Wire.write(wrBuf,wrCount);
	err = Wire.endTransmission(false);
	if ( err != 0 ){ return err; }

	err = Wire.requestFrom(adrs,static_cast<uint8_t>(rdCount),(uint8_t)0);
	int rdAv = 0;
	while((rdAv = Wire.available()) != 0) {
		*(rdBuf+rdCount-rdAv) = Wire.read();
	}

	//err = Wire.endTransmission(true);
	//return err;

	return 0;
}
// ハングしてはいけない本番用
int read_nbyte_i2cDeviceX( unsigned char adrs, unsigned char* wrBuf, unsigned char* rdBuf, int wrCount, int rdCount )
{
	unsigned char err;
  volatile int cnt=0;

	Wire.beginTransmission(adrs);
  Wire.write(wrBuf,wrCount);
	err = Wire.endTransmission(false);
	if ( err != 0 ){ return err; }

	err = Wire.requestFrom(adrs,static_cast<uint8_t>(rdCount),(uint8_t)0);
	int rdAv = 0;
	while(((rdAv = Wire.available()) != 0) && (cnt < 10)){
		*(rdBuf+rdCount-rdAv) = Wire.read();
    cnt += 1;
	}

	return 0;
}
//---------------------------------------------------------
//    Read Only N byte I2C Device
//    Err Code
//      0:success
//      1:data too long to fit in transmit buffer
//      2:received NACK on transmit of address
//      3:received NACK on transmit of data
//      4:other error
//---------------------------------------------------------
int read_only_nbyte_i2cDevice( unsigned char adrs, unsigned char* rdBuf, int rdCount )
{
  unsigned char err;

  err = Wire.requestFrom(adrs,static_cast<uint8_t>(rdCount),static_cast<uint8_t>(false));
  int rdAv = Wire.available();
  while( rdAv ) {
    *(rdBuf+rdCount-rdAv) = Wire1.read();
    rdAv--;
  }

  err = Wire.endTransmission(true);
  return err;
}

//-------------------------------------------------------------------------
//			AT42QT1070
//-------------------------------------------------------------------------
//#ifdef USE_AT42QT1070
static const uint8_t AT42QT_I2C_ADRS = 0x1B;
// read
static const uint8_t AT42QT_STATUS = 2;

static const uint8_t AT42QT_LP_MODE = 54;
static const uint8_t AT42QT_MAX_DUR = 55;

void AT42QT_init( void )
{
  uint8_t i2cdata[2] = {AT42QT_LP_MODE, 0};
  int err = write_i2cDevice(AT42QT_I2C_ADRS, i2cdata,2);
  if ( err != 0 ){ return; }

  i2cdata[0] = AT42QT_MAX_DUR;
  err = write_i2cDevice(AT42QT_I2C_ADRS, i2cdata, 2);
  if ( err != 0 ){ return; }
}

int AT42QT_read( size_t key, uint8_t (&rdraw)[2], bool ref )
{
  uint8_t wd = 0;
  if (ref) {
    wd = 18 + key*2;
  } else {
    wd = 4 + key*2;  //  0-6
  }  
  return read_nbyte_i2cDeviceX(AT42QT_I2C_ADRS, &wd, rdraw, 1, 2);
}
uint8_t AT42QT_read_bit()
{
  uint8_t wd = 3;
  uint8_t rdraw;
  read1byte_i2cDevice(AT42QT_I2C_ADRS, &wd, &rdraw, 1);
  return rdraw;
}
//#endif  // USE_AT42QT1070
/* [] END OF FILE */