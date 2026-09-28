#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- PINS ----------------

// Room A
#define ROOM_A_DHT 4
#define ROOM_A_PIR 5

// Room B
#define ROOM_B_DHT 15
#define ROOM_B_PIR 13

// Room C
#define ROOM_C_DHT 14
#define ROOM_C_PIR 26

// Exhaust Fan Indicator LED
#define EXHAUST_LED 19

#define DHTTYPE DHT22

// ---------------- OLED ----------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ---------------- SENSORS ----------------

DHT dhtA(ROOM_A_DHT, DHTTYPE);
DHT dhtB(ROOM_B_DHT, DHTTYPE);
DHT dhtC(ROOM_C_DHT, DHTTYPE);

// ---------------- THRESHOLDS ----------------

const float TEMP_THRESHOLD = 34.0;
const float HUM_THRESHOLD  = 70.0;

const unsigned long RECOVERY_WINDOW = 10000;
const unsigned long OCCUPANCY_HOLD  = 5000;

// ---------------- ROOM DATA ----------------

float roomTemp[3] = {0};
float roomHum[3] = {0};

bool roomOccupiedRaw[3] = {false};
bool roomOccupied[3] = {false};

unsigned long lastMotion[3] = {0};

const char* roomNames[3] =
{
  "Room A",
  "Room B",
  "Room C"
};

// Room currently being used
int activeRoom = 0;

// relocation state
bool mitigationActive = false;
unsigned long mitigationStart = 0;

// OLED page rotation
unsigned long lastPageSwitch = 0;
int oledPage = 0;

// relocation message
String lastAllocation = "None";

// --------------------------------------------------
// COMFORT SCORE
// --------------------------------------------------

float comfortScore(float temp, float hum)
{
  float score = 100;

  score -= max(0.0f, temp - 26.0f) * 4.0f;
  score -= max(0.0f, hum - 50.0f) * 0.8f;

  return constrain(score, 0, 100);
}

// --------------------------------------------------
// READ ALL SENSORS
// --------------------------------------------------

void readRooms()
{
  float t, h;

  // Room A
  t = dhtA.readTemperature();
  h = dhtA.readHumidity();

  if (!isnan(t)) roomTemp[0] = t;
  if (!isnan(h)) roomHum[0] = h;

  // Room B
  t = dhtB.readTemperature();
  h = dhtB.readHumidity();

  if (!isnan(t)) roomTemp[1] = t;
  if (!isnan(h)) roomHum[1] = h;

  // Room C
  t = dhtC.readTemperature();
  h = dhtC.readHumidity();

  if (!isnan(t)) roomTemp[2] = t;
  if (!isnan(h)) roomHum[2] = h;

  // PIR Occupancy

  int pirPins[3] = {
    ROOM_A_PIR,
    ROOM_B_PIR,
    ROOM_C_PIR
  };

  for (int i = 0; i < 3; i++)
  {
    roomOccupiedRaw[i] = digitalRead(pirPins[i]);

    if (roomOccupiedRaw[i])
    {
      lastMotion[i] = millis();
    }

    roomOccupied[i] =
      (millis() - lastMotion[i]) < OCCUPANCY_HOLD;
  }
}

// --------------------------------------------------
// REALLOCATION ENGINE
// --------------------------------------------------

void relocateClass()
{
  int bestRoom = -1;
  float bestScore = -1;

  for (int i = 0; i < 3; i++)
  {
    if (i == activeRoom)
      continue;

    // Skip occupied rooms
    if (roomOccupied[i])
      continue;

    // Must be comfortable itself
    if (roomTemp[i] >= TEMP_THRESHOLD)
      continue;

    if (roomHum[i] >= HUM_THRESHOLD)
      continue;

    float score =
      comfortScore(roomTemp[i], roomHum[i]);

    if (score > bestScore)
    {
      bestScore = score;
      bestRoom = i;
    }
  }

  if (bestRoom != -1)
  {
    lastAllocation =
      String(roomNames[activeRoom]) +
      " -> " +
      String(roomNames[bestRoom]);

    activeRoom = bestRoom;
  }
  else
  {
    lastAllocation = "No Room Available";
  }
}

// --------------------------------------------------
// OLED
// --------------------------------------------------

void updateOLED()
{
  if (millis() - lastPageSwitch > 3000)
  {
    oledPage++;
    oledPage %= 3;
    lastPageSwitch = millis();
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  // PAGE 1

  if (oledPage == 0)
  {
    display.println("Current Class");

    display.println(roomNames[activeRoom]);

    display.print(roomTemp[activeRoom], 1);
    display.print("C  ");

    display.print(roomHum[activeRoom], 0);
    display.println("%");

    display.println();

    display.println("IN USE");

    if (mitigationActive)
      display.println("MITIGATION ON");
    else
      display.println("COMFORT OK");
  }

  // PAGE 2

  else if (oledPage == 1)
  {
    display.println("Room Status");

    for (int i = 0; i < 3; i++)
    {
      display.print(roomNames[i]);
      display.print(": ");

      if (i == activeRoom)
      {
        display.println("IN USE");
      }
      else
      {
        display.println(
          roomOccupied[i] ?
          "OCCUPIED" :
          "VACANT"
        );
      }
    }
  }

  // PAGE 3

  else
  {
    display.println("Allocation");

    display.println();

    display.println(lastAllocation);
  }

  display.display();
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  dhtA.begin();
  dhtB.begin();
  dhtC.begin();

  pinMode(ROOM_A_PIR, INPUT);
  pinMode(ROOM_B_PIR, INPUT);
  pinMode(ROOM_C_PIR, INPUT);

  pinMode(EXHAUST_LED, OUTPUT);

  digitalWrite(EXHAUST_LED, LOW);

  display.begin(
    SSD1306_SWITCHCAPVCC,
    0x3C
  );

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  Serial.println("SCCRS Started");
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
  readRooms();

  bool uncomfortable =
    roomTemp[activeRoom] >= TEMP_THRESHOLD ||
    roomHum[activeRoom] >= HUM_THRESHOLD;

  // Stage 1
  if (uncomfortable)
  {
    if (!mitigationActive)
    {
      mitigationActive = true;
      mitigationStart = millis();
    }

    digitalWrite(EXHAUST_LED, HIGH);
  }
  else
  {
    mitigationActive = false;
    digitalWrite(EXHAUST_LED, LOW);
  }

  // Stage 2
  if (mitigationActive &&
      millis() - mitigationStart > RECOVERY_WINDOW)
  {
    if (uncomfortable)
    {
      relocateClass();

      mitigationActive = false;

      digitalWrite(EXHAUST_LED, LOW);
    }
  }

  updateOLED();

  delay(200);
}