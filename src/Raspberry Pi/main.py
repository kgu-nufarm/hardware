from flask import Flask, jsonify
import serial
import time
import threading
import requests

app = Flask(__name__)

# 시리얼 포트 설정 (사용자의 환경에 맞게 변경 필요)
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # 시리얼 포트 연결 대기

# 전역 변수로 센서 데이터 저장
sensor_data = {
    'temperature': None,
    'humidity': None,
    'illuminance': None,
    'soil_moisture': None,
    'co2': None
}

# 백엔드 서버의 URL 설정 (사용자의 백엔드 URL로 변경 필요)
backend_server_url = 'http://192.168.137.237:8123/api/sensor/addSensorResult/1'

# 시리얼 데이터를 읽고 전역 변수에 저장하는 함수
def read_serial_data():
    global sensor_data
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {line}")

            # 데이터를 콤마로 나누어 리스트로 변환
            sensor_values = line.split(',')

            if len(sensor_values) == 5:
                # 각 센서 데이터를 전역 변수에 저장
                sensor_data['temperature'] = int(sensor_values[0])
                sensor_data['humidity'] = int(sensor_values[1])
                sensor_data['illuminance'] = int(sensor_values[2])
                sensor_data['soil_moisture'] = int(sensor_values[3])
                sensor_data['co2'] = int(sensor_values[4])

        # 데이터가 없으면 1초 대기
        time.sleep(1)

# 전역 변수에 저장된 데이터를 백엔드 서버로 보내는 함수
def send_data_to_backend():
    global sensor_data
    while True:
        try:
            # 백엔드 서버로 POST 요청 보내기
            response = requests.post(backend_server_url, json=sensor_data)
            if response.status_code == 200:
                print("Data successfully sent to backend server.")
            else:
                print(f"Failed to send data. Status code: {response.status_code}")
        except Exception as e:
            print(f"Error sending data: {e}")

        # 1시간마다 데이터 전송
        time.sleep(5)

# HTTP GET 요청에 대해 전역 변수의 센서 데이터를 반환하는 엔드포인트
@app.route('/sensor', methods=['GET'])
def get_sensor_data():
    global sensor_data
    return jsonify(sensor_data), 200

# 스레드를 시작하는 함수
def start_threads():
    # 시리얼 데이터를 읽는 스레드
    serial_thread = threading.Thread(target=read_serial_data)
    serial_thread.daemon = True
    serial_thread.start()

    # 1시간마다 백엔드에 데이터를 보내는 스레드
    backend_thread = threading.Thread(target=send_data_to_backend)
    backend_thread.daemon = True
    backend_thread.start()

if __name__ == '__main__':
    # 스레드 시작
    start_threads()

    # Flask 서버 실행
    app.run(host='0.0.0.0', port=5000)
