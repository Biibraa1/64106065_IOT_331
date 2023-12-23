import paho.mqtt.client as mqtt
import requests
import json

# สร้าง MQTT client instance
client = mqtt.Client()

# กำหนด callback function เมื่อมีข้อมูลเข้ามาใน Topic "dht11"
def on_message(client, userdata, message):
    payload_str = message.payload.decode()
    print(f"Received message: {payload_str} from topic {message.topic}")

    try:
        # Split the payload into two values based on the '+'
        values = payload_str.split('+')
        # Convert each value to a float
        value1 = float(values[0])
        value2 = float(values[1])

        # Create a dictionary to represent the payload
        payload = {"value1": value1, "value2": value2}

        # ทำ HTTP POST request ไปยัง URL โดยส่งข้อมูลในรูปแบบ JSON
        url = "http://192.168.245.253:3000/dht11"
        headers = {'Content-Type': 'application/json'}
        response = requests.post(url, json=payload, headers=headers)
        print(response.text)  # แสดงผลลัพธ์จากการส่งข้อมูล
    except ValueError as ve:
        print(f"Error converting values to float: {ve}")
    except IndexError as ie:
        print(f"Error splitting payload: {ie}")



# กำหนด callback function เมื่อเชื่อมต่อ MQTT broker สำเร็จ
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    # Subscribe เข้า Topic "dht11" เมื่อเชื่อมต่อสำเร็จ
    client.subscribe("dht11")

# กำหนด callback function สำหรับการรับข้อความ
client.on_message = on_message
client.on_connect = on_connect

# เชื่อมต่อไปยัง MQTT broker
client.connect("192.168.245.253", 1883, 60)

# เริ่มการทำงานของ MQTT client ในโหมด loop สำหรับรับข้อมูล
client.loop_forever()
