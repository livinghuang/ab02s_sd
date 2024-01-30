#include "LoRaWan_APP.h"
#include "Arduino.h"

#include "SD.h"
#include "SPI.h"
#define SD_CS_PIN GPIO4
#define SD_SCK_PIN SCK1
#define SD_MISO_PIN MISO1
#define SD_MOSI_PIN MOSI1

bool sdInitialized = false;
/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB red means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY 868000000 // Hz

#define TX_OUTPUT_POWER 5 // dBm

#define LORA_BANDWIDTH 0        // [0: 125 kHz,
                                //  1: 250 kHz,
                                //  2: 500 kHz,
                                //  3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1       // [1: 4/5,
                                //  2: 4/6,
                                //  3: 4/7,
                                //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone(void);
void OnTxTimeout(void);
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

typedef enum
{
  LOWPOWER,
  RX,
  TX
} States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi, rxSize;

void pingpong_setup()
{
  Serial.begin(115200);

  txNumber = 0;
  Rssi = 0;

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
  state = TX;
}

void pingpong_loop()
{
  switch (state)
  {
  case TX:
    delay(1000);
    txNumber++;
    sprintf(txpacket, "%s", "hello");
    sprintf(txpacket + strlen(txpacket), "%d", txNumber);
    sprintf(txpacket + strlen(txpacket), "%s", " Rssi : ");
    sprintf(txpacket + strlen(txpacket), "%d", Rssi);
    turnOnRGB(COLOR_SEND, 0);

    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));

    Radio.Send((uint8_t *)txpacket, strlen(txpacket));
    state = LOWPOWER;
    break;
  case RX:
    Serial.println("into RX mode");
    Radio.Rx(0);
    state = LOWPOWER;
    break;
  case LOWPOWER:
    lowPowerHandler();
    break;
  default:
    break;
  }
  Radio.IrqProcess();
}

void OnTxDone(void)
{
  Serial.print("TX done......");
  turnOnRGB(0, 0);
  state = RX;
}

void OnTxTimeout(void)
{
  Radio.Sleep();
  Serial.print("TX Timeout......");
  state = TX;
}
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  Rssi = rssi;
  rxSize = size;
  memcpy(rxpacket, payload, size);
  rxpacket[size] = '\0';
  turnOnRGB(COLOR_RECEIVED, 0);
  Radio.Sleep();

  Serial.printf("\r\nreceived packet \"%s\" with Rssi %d , length %d\r\n", rxpacket, Rssi, rxSize);
  Serial.println("wait to send next packet");

  state = TX;
}

void sd_init(void)
{

  sdInitialized = sdd_setting();

  if (sdInitialized)
  {
    Serial.println("SD卡初始化成功");
  }
  else
  {
    Serial.println("SD卡初始化失敗");
  }
}

void sd_test()
{
  // 主要迴圈程式碼放在這裡
  File file = SD.open("/test.txt");
  if (file)
  {
    Serial.println("Successfully opened file:");
    while (file.available())
    {
      Serial.write(file.read());
    }
    file.close();
  }
  else
  {
    Serial.println("Failed to open file");
  }

  File writeFile = SD.open("/test.txt", FILE_WRITE);
  if (writeFile)
  {
    String data = "Hello, SD card ^_^";
    writeFile.println(data);
    writeFile.close();
    Serial.println("Successfully wrote to file");
  }
  else
  {
    Serial.println("Failed to write to file");
  }

  delay(500);
}

bool sdd_setting()
{
  Radio.Init(&RadioEvents);
  Radio.Sleep();
  SPI.end();
  delay(10);
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

  if (!SD.begin(SD_CS_PIN))
  {
    return false;
  }

  return true;
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // nothing happens after setup
  Serial.println("Initializing SD card...");
  Serial.flush();

  sd_init();
  delay(1000);
}

void loop()
{
  sdd_setting();
  sd_test();
  delay(1000);
}
