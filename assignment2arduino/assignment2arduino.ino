//Include all the library needed (wifi, mqtt and dht sensor)
#include <WiFi.h> 
#include <PubSubClient.h> 
#include "DHT.h"  

//wifi credentials
const char *WIFI_SSID = "wmf@unifi"; 
const char *WIFI_PASSWORD = "wongmuifang88"; 

//mqtt broker setup
const char *MQTT_SERVER = "34.67.164.251";  
const int MQTT_PORT = 1883; 
const char *MQTT_TOPIC = "iot"; 

//set dht sensor type
const int DHT_TYPE = DHT11; 

//pin configuration
const int DHT_PIN = 40;  
const int PIR_PIN = 39;
const int MQ2_PIN = 38;

//create dht, wifi client and mqtt client object
DHT dht(DHT_PIN, DHT_TYPE); 
WiFiClient espClient; 
PubSubClient client(espClient);  

//fn to set up wifi connection
void setup_wifi() 
{     
  delay(10);     
  Serial.println("Connecting to WiFi...");     
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);     
  while (WiFi.status() != WL_CONNECTED)     
  {         
    delay(500);         
    Serial.print(".");     
    }     
  Serial.println("WiFi connected");     
  Serial.println("IP address: ");     
  Serial.println(WiFi.localIP()); 
  }  
  
  //setup fn
  void setup() 
  { 
    //start serial communication
    Serial.begin(115200);   
    //initialise dht sensor  
    dht.begin(); 

    //set pir,mq2 sensor pin as input
    pinMode(PIR_PIN, INPUT);
    pinMode(MQ2_PIN, INPUT);

    //set up wifi conneciton
    setup_wifi();  

    //set mqtt server details   
    client.setServer(MQTT_SERVER, MQTT_PORT); 
  }  
    
  //fn to reconnect to mqtt server
  void reconnect() 
  {     
    while (!client.connected())     
    {         
      Serial.println("Attempting MQTT connection...");         
      if (client.connect("ESP32Client"))         
      {             
        Serial.println("Connected to MQTT server");         
      }         
      else         
      {             
        Serial.print("Failed, rc=");             
        Serial.print(client.state());             
        Serial.println(" Retrying in 5 seconds...");             
        delay(5000);         
      }     
    } 
  }  
  
  //main fn loop
  void loop() 
  {     
    if (!client.connected())     
    {   
      //reconnect to mqtt server if not connected      
      reconnect();     
    }      
    
    //to maintain mqtt connection
    client.loop(); 

    //delay between readings     
    delay(5000);      
    
    //read sensor data
    float temperature = dht.readTemperature();  
    float humidity = dht.readHumidity();
    int pirState = digitalRead(PIR_PIN);
    int mq2State = digitalRead(MQ2_PIN);

    //create message payload
    char payload[150];    
    sprintf(payload, "Temperature(°C): %.2f, Humidity(%%): %.2f, Motion: %s, Gas: %s", 
            temperature, humidity, 
            pirState ? "Motion Detected" : "No Motion", 
            mq2State ? "Gas Detected" : "No Gas");

    //print payload to serial monitor
    Serial.println(payload);

    //publish payload to mqtt topic "iot"
    client.publish(MQTT_TOPIC, payload); 
  }