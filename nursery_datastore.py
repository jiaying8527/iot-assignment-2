#import all the library needed 
import pymongo #pymongo - for mongodb interaction
import paho.mqtt.client as mqtt #mqtt client
from datetime import datetime #datetime module

# MongoDB configuration

#connect to MongoDB server
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
#select the smartnursery database
db = mongo_client["smartnursery"]
#select the collection "iot" within the smartnuersery db
collection = db["iot"]

# MQTT configuration
mqtt_broker_address = '34.67.164.251' #mqtt broker address
mqtt_topic = 'iot' #mqtt topic to subscribe to

#callback fn when a message is received on the mqtt topic
def on_message(client, userdata, message):

    #decode the message payload from byte to string
    payload = message.payload.decode('utf-8')
    #print the received message to the console
    print(f'Received message: {payload}')

    # Convert MQTT timestamp to datetime
    timestamp = datetime.utcnow()  # Use current UTC time
    datetime_obj = timestamp.strftime("%Y-%m-%dT%H:%M:%S.%fZ") #format the timestamp

    # Process the payload and insert into MongoDB with proper timestamp
    document = {
            'timestamp': datetime_obj, #timestamp of the message
            'data': payload #data received from mqtt
    }
    #insert the document into the MongoDB collection
    collection.insert_one(document)
    #print confirmation message
    print('Data ingested into MongoDB')

#create mqtt client instance and set the callback fn
client = mqtt.Client()
client.on_message = on_message

# Connect to MQTT broker (with a timeout - 60s)
client.connect(mqtt_broker_address, 1883, 60)

# Subscribe to MQTT topic - "iot"
client.subscribe(mqtt_topic)

# Start the MQTT loop to handle incoming messages indefinitely
client.loop_forever()