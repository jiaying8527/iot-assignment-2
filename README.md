# smart hospital nursery
Arduino sketches and Python script to complete this assignment are included in this repository.

The Arduino sketch reads data from DHT, PIR, and MQ2 sensors, publishes it to an MQTT topic and connects to a WiFi network for IoT communication.
Meanwhile, the Python script subscribes to an MQTT topic - "iot", processes incoming messages and stores the data in a MongoDB collection - "iot".
