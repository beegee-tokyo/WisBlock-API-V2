/**
 * @file flash-nrf52.cpp
 * @author Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * @brief Initialize, read and write parameters from/to internal flash memory
 * @version 0.1
 * @date 2021-01-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifdef ARDUINO_RAKWIRELESS_RAK11300

#include "WisBlock-API-V2.h"

s_lorawan_settings g_flash_content;
s_loracompat_settings g_flash_content_compat;

#include <FS.h>
#include <LittleFS.h>

#define FILE_O_READ "r"
#define FILE_O_WRITE "w"

const char settings_name[] = "/rak.bin";

File lora_file;

void flash_int_reset(void);

/**
 * @brief Initialize access to nRF52 internal file system
 *
 */
void init_flash(void)
{
	if (init_flash_done)
	{
		return;
	}

	// Initialize Internal File System
	if (!LittleFS.begin())
	{
		API_LOG("FLASH", "LittleFS.begin() failed");
		LittleFS.format();
		if (!LittleFS.begin())
		{
			API_LOG("FLASH", "LittleFS.begin() failed after format");
		}
	}

	// Check if file exists
	lora_file = LittleFS.open(settings_name, FILE_O_READ);
	if (!lora_file)
	{
		API_LOG("FLASH", "File doesn't exist, force format");
		delay(1000);
		flash_reset();
		lora_file = LittleFS.open(settings_name, FILE_O_READ);
	}

	uint8_t markers[2] = {0};
	lora_file.read(markers, 2);
	if ((markers[0] == 0xAA) && (markers[1] == LORAWAN_COMPAT_MARKER))
	{
		API_LOG("FLASH", "File has old structure, merge into new structure");
		// Found old structure
		lora_file.close();
		// Read data into old structure
		s_loracompat_settings old_struct;
		lora_file = LittleFS.open(settings_name, FILE_O_READ);
		lora_file.read((uint8_t *)&old_struct, sizeof(s_loracompat_settings));
		lora_file.close();
		// Merge old structure into new structure
		g_lorawan_settings.adr_enabled = old_struct.adr_enabled;
		g_lorawan_settings.app_port = old_struct.app_port;
		g_lorawan_settings.auto_join = old_struct.auto_join;
		g_lorawan_settings.confirmed_msg_enabled = old_struct.confirmed_msg_enabled;
		g_lorawan_settings.data_rate = old_struct.data_rate;
		g_lorawan_settings.duty_cycle_enabled = old_struct.duty_cycle_enabled;
		g_lorawan_settings.join_trials = old_struct.join_trials;
		g_lorawan_settings.lora_class = old_struct.lora_class;
		g_lorawan_settings.lora_region = old_struct.lora_region;
		memcpy(g_lorawan_settings.node_app_eui, old_struct.node_app_eui, 8);
		memcpy(g_lorawan_settings.node_app_key, old_struct.node_app_key, 16);
		memcpy(g_lorawan_settings.node_apps_key, old_struct.node_apps_key, 16);
		g_lorawan_settings.node_dev_addr = old_struct.node_dev_addr;
		memcpy(g_lorawan_settings.node_device_eui, old_struct.node_device_eui, 8);
		memcpy(g_lorawan_settings.node_nws_key, old_struct.node_nws_key, 16);
		g_lorawan_settings.otaa_enabled = old_struct.otaa_enabled;
		g_lorawan_settings.public_network = old_struct.public_network;
		g_lorawan_settings.send_repeat_time = old_struct.send_repeat_time;
		g_lorawan_settings.subband_channels = old_struct.subband_channels;
		g_lorawan_settings.tx_power = old_struct.tx_power;
		save_settings();
		// delay(1000);
		// sd_nvic_SystemReset();
	}
	else
	{
		// Found new structure
		lora_file.close();
		lora_file = LittleFS.open(settings_name, FILE_O_READ);
		lora_file.read((uint8_t *)&g_lorawan_settings, sizeof(s_lorawan_settings));
		lora_file.close();
		// Check if it is LPWAN settings
		if ((g_lorawan_settings.valid_mark_1 != 0xAA) || (g_lorawan_settings.valid_mark_2 != LORAWAN_DATA_MARKER))
		{
			// Data is not valid, reset to defaults
			API_LOG("FLASH", "Invalid data set, deleting and restart node");
			LittleFS.format();
			delay(1000);
			api_reset();
		}
		log_settings();
		init_flash_done = true;
	}
}

/**
 * @brief Save changed settings if required
 *
 * @return boolean
 * 			result of saving
 */
boolean save_settings(void)
{
	bool result = true;
	// Read saved content
	lora_file = LittleFS.open(settings_name, FILE_O_READ);
	if (!lora_file)
	{
		API_LOG("FLASH", "File doesn't exist, force format");
		delay(100);
		flash_reset();
		lora_file = LittleFS.open(settings_name, FILE_O_READ);
	}
	lora_file.read((uint8_t *)&g_flash_content, sizeof(s_lorawan_settings));
	lora_file.close();
	if (memcmp((void *)&g_flash_content, (void *)&g_lorawan_settings, sizeof(s_lorawan_settings)) != 0)
	{
		API_LOG("FLASH", "Flash content changed, writing new data");
		delay(100);

		LittleFS.remove(settings_name);

		if (lora_file = LittleFS.open(settings_name, FILE_O_WRITE))
		{
			lora_file.write((uint8_t *)&g_lorawan_settings, sizeof(s_lorawan_settings));
			lora_file.flush();
		}
		else
		{
			API_LOG("FLASH", "Flash write failed");
			result = false;
		}
		lora_file.close();
	}
	log_settings();
	return result;
}

/**
 * @brief Reset content of the filesystem
 *
 */
void flash_reset(void)
{
	LittleFS.format();
	if (lora_file = LittleFS.open(settings_name, FILE_O_WRITE))
	{
		s_lorawan_settings default_settings;
		lora_file.write((uint8_t *)&default_settings, sizeof(s_lorawan_settings));
		lora_file.flush();
		lora_file.close();
	}
}

#endif