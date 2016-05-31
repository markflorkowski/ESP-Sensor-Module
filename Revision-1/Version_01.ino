#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "......";
const char* password = "......";
const char* mqtt_server = "......";
const char* pubTopic = "......";
const char* subTopic = "......";

const int motionPin = 12;
const int redLedPin = 13;
const int buzzerPin = 14;

int previousReading = LOW;
int beeper = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() 
{

  delay(10);
  // connect to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}

void callback(char* topic, byte* payload, unsigned int length) 
{
	String message;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
	{
		message += (char)payload[i];
    Serial.print((char)payload[i]);
  }
	
	if(message.substring(0, 9) == "Buzzer ON")
	{
		beeper = 0;
		tone(buzzerPin, (message.substring(10, 14)).toInt());
	}
	
	else if(message.substring(0, 9) == "Beeper ON")
	{
		beeper = (message.substring(10, 14)).toInt();
	}
	
	else if(message.substring(0, 10) == "Buzzer OFF")
	{
		noTone(buzzerPin);
		beeper = 0;
		Serial.println("No BUZZZ");
	}
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
	{
    // Attempt to connect
    if (client.connect("ESP8266Client2")) 
		{
    Serial.print("Connected to broker");
			connection_successful_indicator();
			client.subscribe(subTopic);
    } 
		
		else 
		{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
			connection_failed_indicator();
      delay(5000);
    }
  }
}

void setup() {
	digitalWrite(redLedPin, LOW);
	
  Serial.begin(115200);
  setup_wifi();
	
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
	
  pinMode(motionPin, INPUT); //Motion
	pinMode(buzzerPin, OUTPUT); //Alarm
	pinMode(redLedPin, OUTPUT); //Red Led
	
}

void loop() 
{

  if (!client.connected())
	{
    reconnect();
  }

  client.loop();
  int reading = digitalRead(motionPin);
  if (previousReading == LOW && reading == HIGH)
	{
    client.publish(pubTopic, "ON");
    Serial.print("Motion Sensor Triggered");
    delay(5200);
		noTone(buzzerPin);
		client.publish(pubTopic, "OFF");
		Serial.print("OFF ");
		delay(5000);
  }
	Serial.println(beeper);
  if(beeper != 0)
	{
		tone(buzzerPin, beeper);
		delay(200);
		noTone(buzzerPin);
		delay(100);
		
	}
	
  previousReading = reading;
	
	

}

void connection_successful_indicator()
{
	digitalWrite(redLedPin, HIGH);
	tone(buzzerPin, 300);
	delay(500);
	tone(buzzerPin, 900);
	delay(1000);
	noTone(buzzerPin);
}

void connection_failed_indicator()
{
	digitalWrite(redLedPin, LOW);
	tone(buzzerPin, 300);
	delay(400);
	noTone(buzzerPin);
	delay(200);
	tone(buzzerPin, 300);
	delay(400);
	noTone(buzzerPin);
}
