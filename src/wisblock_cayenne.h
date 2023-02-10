/**
 * @file wisblock_cayenne.h
 * @author Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * @brief Extend CayenneLPP class with custom channels
 * @version 0.1
 * @date 2022-01-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef WISBLOCK_CAYENNE_H
#define WISBLOCK_CAYENNE_H

#include <Arduino.h>
// #include <ArduinoJson.h>
#include <CayenneLPP.h>

// Additional value ID's
#define LPP_GPS4 136	  // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter (Cayenne LPP default)
#define LPP_GPS6 137	  // 4 byte lon/lat 0.000001 °, 3 bytes alt 0.01 meter (Customized Cayenne LPP)
#define LPP_VOC 138		  // 2 byte VOC index
#define SH_WIND_SP 190	  // 2 bytes wind speed from SensorHub RK900-09 0.01 m/s, in range 0~40m/s
#define SH_WIND_DIR 191	  // 2 bytes wind direction from SensorHub RK900-09 1°, in 0~359°
#define SH_SOIL_MOIST 188 // 2 bytes soil moisture from SensorHub RK500-02 0.1 % in 0~100% (m3/m3)
#define SH_SOIL_TEMP 103  // 2 bytes soil moisture from SensorHub RK500-02 0.1 % in 0~100% (m3/m3)
#define SH_EC 192		  // 2 bytes EC from SensorHub RK500-02 0.001, mS/cm
#define SH_PH_H 193		  // 2 bytes pH high precision from SensorHub RK500-02 0.01 pH
#define SH_PH_L 194		  // 2 bytes pH low precision from SensorHub RK500-02 0.1 pH
#define SH_PYRANO 195	  // 2 bytes pyrano value from SensorHub RK200-03 1 unsigned MSB (W/m2)

// Only Data Size
#define LPP_GPS4_SIZE 9
#define LPP_GPS6_SIZE 11
#define LPP_GPSH_SIZE 14
#define LPP_GPST_SIZE 10
#define LPP_VOC_SIZE 2
#define SH_SIZE_2 2

// Cayenne LPP Channel numbers per sensor value used in WisBlock API examples
#define LPP_CHANNEL_BATT 1			   // Base Board
#define LPP_CHANNEL_HUMID 2			   // RAK1901
#define LPP_CHANNEL_TEMP 3			   // RAK1901
#define LPP_CHANNEL_PRESS 4			   // RAK1902
#define LPP_CHANNEL_LIGHT 5			   // RAK1903
#define LPP_CHANNEL_HUMID_2 6		   // RAK1906
#define LPP_CHANNEL_TEMP_2 7		   // RAK1906
#define LPP_CHANNEL_PRESS_2 8		   // RAK1906
#define LPP_CHANNEL_GAS_2 9			   // RAK1906
#define LPP_CHANNEL_GPS 10			   // RAK1910/RAK12500
#define LPP_CHANNEL_SOIL_TEMP 11	   // RAK12035
#define LPP_CHANNEL_SOIL_HUMID 12	   // RAK12035
#define LPP_CHANNEL_SOIL_HUMID_RAW 13  // RAK12035
#define LPP_CHANNEL_SOIL_VALID 14	   // RAK12035
#define LPP_CHANNEL_LIGHT2 15		   // RAK12010
#define LPP_CHANNEL_VOC 16			   // RAK12047
#define LPP_CHANNEL_GAS 17			   // RAK12004
#define LPP_CHANNEL_GAS_PERC 18		   // RAK12004
#define LPP_CHANNEL_CO2 19			   // RAK12008
#define LPP_CHANNEL_CO2_PERC 20		   // RAK12008
#define LPP_CHANNEL_ALC 21			   // RAK12009
#define LPP_CHANNEL_ALC_PERC 22		   // RAK12009
#define LPP_CHANNEL_TOF 23			   // RAK12014
#define LPP_CHANNEL_TOF_VALID 24	   // RAK12014
#define LPP_CHANNEL_GYRO 25			   // RAK12025
#define LPP_CHANNEL_GESTURE 26		   // RAK14008
#define LPP_CHANNEL_UVI 27			   // RAK12019
#define LPP_CHANNEL_UVS 28			   // RAK12019
#define LPP_CHANNEL_CURRENT_CURRENT 29 // RAK16000
#define LPP_CHANNEL_CURRENT_VOLTAGE 30 // RAK16000
#define LPP_CHANNEL_CURRENT_POWER 31   // RAK16000
#define LPP_CHANNEL_TOUCH_1 32		   // RAK14002
#define LPP_CHANNEL_TOUCH_2 33		   // RAK14002
#define LPP_CHANNEL_TOUCH_3 34		   // RAK14002
#define LPP_CHANNEL_CO2_2 35		   // RAK12037
#define LPP_CHANNEL_CO2_Temp_2 36	   // RAK12037
#define LPP_CHANNEL_CO2_HUMID_2 37	   // RAK12037
#define LPP_CHANNEL_TEMP_3 38		   // RAK12003
#define LPP_CHANNEL_TEMP_4 39		   // RAK12003
#define LPP_CHANNEL_PM_1_0 40		   // RAK12039
#define LPP_CHANNEL_PM_2_5 41		   // RAK12039
#define LPP_CHANNEL_PM_10_0 42		   // RAK12039
#define LPP_CHANNEL_EQ_EVENT 43		   // RAK12027
#define LPP_CHANNEL_EQ_SI 44		   // RAK12027
#define LPP_CHANNEL_EQ_PGA 45		   // RAK12027
#define LPP_CHANNEL_EQ_SHUTOFF 46	   // RAK12027
#define LPP_CHANNEL_EQ_COLLAPSE 47	   // RAK12027
#define LPP_CHANNEL_SWITCH 48		   // RAK13011
#define LPP_SH_WS 49				   // SensorHub RK900-09
#define LPP_SH_WD 50				   // SensorHub RK900-09
#define LPP_AUDIO_L_A 51			   // WisBlock Audio
#define LPP_AUDIO_L_C 52			   // WisBlock Audio
#define LPP_AUDIO_ALARM 53			   // WisBlock Audio
#define LPP_AUDIO_TYPE 54			   // WisBlock Audio
#define LPP_SH_SOIL_MOIST 55		   // SensorHub RK500-02
#define LPP_SH_EC 56				   // SensorHub RK500-02
#define LPP_SH_PH_H 57				   // SensorHub RK500-02
#define LPP_SH_PH_L 58				   // SensorHub RK500-02
#define LPP_SH_PYRANO 59			   // SensorHub RK200-03

class WisCayenne : public CayenneLPP
{
public:
	WisCayenne(uint8_t size) : CayenneLPP(size) {}

	uint8_t addGNSS_4(uint8_t channel, int32_t latitude, int32_t longitude, int32_t altitude);
	uint8_t addGNSS_6(uint8_t channel, int32_t latitude, int32_t longitude, int32_t altitude);
	uint8_t addGNSS_H(int32_t latitude, int32_t longitude, int16_t altitude, int16_t accuracy, int16_t battery);
	uint8_t addGNSS_T(int32_t latitude, int32_t longitude, int16_t altitude, float accuracy, int8_t sats);
	uint8_t addVoc_index(uint8_t channel, uint32_t voc_index);
	uint8_t addSH_2_value(uint8_t channel, uint8_t data_type, float value);

private:
};
#endif