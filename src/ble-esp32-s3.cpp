/**
 * @file ble-esp32.cpp
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief BLE initialization & device configuration
 * @version 0.1
 * @date 2022-06-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifdef ESP32
#ifdef _VARIANT_RAK3112_
// #warning "RAK3112"

#include "WisBlock-API-V2.h"

void start_ble_adv(void);

// List of Service and Characteristic UUIDs
/** Service UUID for Uart */
#define UART_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
/** Characteristic UUID for receiver */
#define RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
/** Characteristic UUID for transmitter */
#define TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

/** Characteristic for BLE-UART TX */
BLECharacteristic *uart_tx_characteristic;
/** Characteristic for BLE-UART RX */
BLECharacteristic *uart_rx_characteristic;
/** BLE Advertiser */
BLEAdvertising *ble_advertising;
/** BLE Service for Uart*/
BLEService *uart_service;
/** BLE Server */
BLEServer *ble_server;

/** Flag used to detect if a BLE advertising is enabled */
bool g_ble_is_on = false;
/** Flag if device is connected */
bool g_ble_uart_is_connected = false;

Ticker advertising_timer;
Ticker blue_led_timer;
uint16_t _timeout;

void toggle_blue_led(void)
{
#ifndef _CUSTOM_BOARD_
	digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
#endif
}

/**
 * Callbacks for client connection and disconnection
 */
class MyServerCallbacks : public BLEServerCallbacks
{
	/**
	 * Callback when a device connects
	 * @param ble_server
	 * 			Pointer to server that was connected
	 */
	void onConnect(BLEServer *ble_server)
	{
		API_LOG("BLE", "BLE client connected");
		advertising_timer.detach();
#if NO_BLE_LED > 0
		blue_led_timer.detach();
#endif
		g_ble_uart_is_connected = true;
#ifndef _CUSTOM_BOARD_
		digitalWrite(LED_BLUE, HIGH);
#endif
	};

	/**
	 * Callback when a device disconnects
	 * @param ble_server
	 * 			Pointer to server that was disconnected
	 */
	void onDisconnect(BLEServer *ble_server)
	{
		API_LOG("BLE", "BLE client disconnected");
		g_ble_uart_is_connected = false;
		ble_advertising->start();
		advertising_timer.once(_timeout, stop_ble_adv);
#if NO_BLE_LED > 0
		blue_led_timer.attach(1, toggle_blue_led);
#endif
#ifndef _CUSTOM_BOARD_
		digitalWrite(LED_BLUE, LOW);
#endif
	}
};

void at_ble_input(std::string cmd);
std::string ble_at_cmd;
SemaphoreHandle_t _ble_notify;
TaskHandle_t _notifyTaskHandle;
static BaseType_t xHigherPriorityTaskWoken = pdTRUE;
#ifndef TASK_PRIO_NORMAL
#define TASK_PRIO_NORMAL 1
#endif

void _notify_task(void *pvParameters)
{
	while (1)
	{
		if (xSemaphoreTake(_ble_notify, portMAX_DELAY) == pdTRUE)
		{
			for (int idx = 0; idx < ble_at_cmd.length(); idx++)
			{
				Serial.printf("%c", ble_at_cmd[idx]);
				at_serial_input(ble_at_cmd[idx]);
			}
			at_serial_input('\n');
		}
	}
}
/**
 * Callbacks for BLE UART write requests
 * on WiFi characteristic
 */
class UartCallBackHandler : public BLECharacteristicCallbacks
{
	/**
	 * Callback for write request on UART characteristic
	 * @param pCharacteristic
	 * 			Pointer to the characteristic
	 */
	void onWrite(BLECharacteristic *ble_characteristic)
	{
		std::string rx_value = ble_characteristic->getValue();

		if (rx_value.length() > 0)
		{
			ble_at_cmd = rx_value;
			xSemaphoreGiveFromISR(_ble_notify, &xHigherPriorityTaskWoken);
		}
	}
};

/**
 * Initialize BLE service and characteristic
 * Start BLE server and service advertising
 */
void init_ble()
{
	// Create the BLE Write event semaphore
	_ble_notify = xSemaphoreCreateBinary();
	// Initialize semaphore
	xSemaphoreGive(_ble_notify);
	xSemaphoreTake(_ble_notify, 10);
	// Start task to handle Write events
	xTaskCreate(_notify_task, "NOTIFY", 4096, NULL, TASK_PRIO_NORMAL, &_notifyTaskHandle);

	// Create device name
	char helper_string[256] = {0};
	sprintf(helper_string, "%s-%02X%02X%02X%02X%02X%02X", g_ble_dev_name,
			(uint8_t)(g_lorawan_settings.node_device_eui[2]), (uint8_t)(g_lorawan_settings.node_device_eui[3]),
			(uint8_t)(g_lorawan_settings.node_device_eui[4]), (uint8_t)(g_lorawan_settings.node_device_eui[5]), (uint8_t)(g_lorawan_settings.node_device_eui[6]), (uint8_t)(g_lorawan_settings.node_device_eui[7]));

	API_LOG("BLE", "Initialize BLE");
	// Initialize BLE and set output power
	BLEDevice::init(g_ble_dev_name);

	BLEDevice::setPower(ESP_PWR_LVL_P9);

	// BLEDevice::setMTU(200);

	BLEAddress thisAddress = BLEDevice::getAddress();

	API_LOG("BLE", "BLE address: %s\n", thisAddress.toString().c_str());

	// Create BLE Server
	ble_server = BLEDevice::createServer();

	// Set server callbacks
	ble_server->setCallbacks(new MyServerCallbacks());

	// Create the UART BLE Service
	uart_service = ble_server->createService(UART_UUID);

	// Create a BLE Characteristic
	uart_tx_characteristic = uart_service->createCharacteristic(
		TX_UUID,
		BLECharacteristic::PROPERTY_NOTIFY);

	uart_tx_characteristic->addDescriptor(new BLE2902());

	uart_rx_characteristic = uart_service->createCharacteristic(
		RX_UUID,
		BLECharacteristic::PROPERTY_WRITE);

	uart_rx_characteristic->setCallbacks(new UartCallBackHandler());

	// Start the service
	uart_service->start();

	// Start advertising
	ble_advertising = ble_server->getAdvertising();
	ble_advertising->addServiceUUID(UART_UUID);
	start_ble_adv();
}

void restart_advertising(uint16_t timeout)
{
	_timeout = timeout;
	if (timeout != 0)
	{
		advertising_timer.once(timeout, stop_ble_adv);
#ifndef _CUSTOM_BOARD_
#if NO_BLE_LED > 0
		blue_led_timer.attach(1, toggle_blue_led);
#endif
#endif
	}
	ble_advertising->start();
	g_ble_is_on = true;
}

/**
 * Stop BLE advertising
 */
void stop_ble_adv(void)
{
	advertising_timer.detach();
#if NO_BLE_LED > 0
	blue_led_timer.detach();
#endif
#ifndef _CUSTOM_BOARD_
	digitalWrite(LED_BLUE, LOW);
#endif
	/// \todo needs patch in BLEAdvertising.cpp -> handleGAPEvent() -> remove start(); from ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT
	ble_advertising->stop();
	g_ble_is_on = false;
}

/**
 * Start BLE advertising
 */
void start_ble_adv(void)
{
#if NO_BLE_LED > 0
	blue_led_timer.attach(1, toggle_blue_led);
#endif
	if (g_lorawan_settings.auto_join)
	{
		advertising_timer.once(60, stop_ble_adv);
		ble_advertising->start();
	}
	else
	{
		ble_advertising->start();
	}
	g_ble_is_on = true;
}

#endif // _VARIANT_RAK3112_
#endif // ESP32