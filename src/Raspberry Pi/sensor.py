import serial
import time
import requests

# 시리얼 포트와 보레이트 설정
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # 아두이노와 연결  대기

# 웹 서버의 URL 설정
server_url = 'http://server-url/sensor/addSensorResult'

def send_data_to_server(temperature, humidity, illuminance, soil_moisture, co2):
    data = {
        'temperature': temperature,
        'humidity': humidity,
        'illuminance': illuminance,
        'soil_moisture': soil_moisture,
        'co2': co2
    }
    try:
        response = requests.post(server_url, json=data)
        if response.status_code == 200:
            print("Data successfully sent to server.")
        else:
            print(f"Failed to send data. Status code: {response.status_code}")
    except Exception as e:
        print(f"Error sending data: {e}")

try:
    last_sent_time = time.time()

    while True:
        # 아두이노로부터 데이터 읽기
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {line}")

            # 데이터를 콤마로 나누어 리스트로 변환
            sensor_values = line.split(',')

            if len(sensor_values) == 5:
                # 각 센서 데이터를 변수에 저장
                temperature = int(sensor_values[0])
                humidity = int(sensor_values[1])
                illuminance = int(sensor_values[2])
                soil_moisture = int(sensor_values[3])
                co2 = int(sensor_values[4])

                # 데이터 출력
                print(f"Temperature: {temperature} C")
                print(f"Humidity: {humidity} %")
                print(f"Illuminance: {illuminance} LUX")
                print(f"Soil Moisture: {soil_moisture} %")
                print(f"CO2 Level: {co2} ppm")

                # 한 시간마다 데이터를 웹 서버로 전송
                current_time = time.time()
                if current_time - last_sent_time >= 3600:  # 3600초 = 1시간
                    send_data_to_server(temperature, humidity, illuminance, soil_moisture, co2)
                    last_sent_time = current_time

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
