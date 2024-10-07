import serial
import time

# 시리얼 포트와 보레이트 설정
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # 아두이노와 연결 안정화 대기

try:
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

                # 데이터를 출력
                print(f"Temperature: {temperature} C")
                print(f"Humidity: {humidity} %")
                print(f"Illuminance: {illuminance} LUX")
                print(f"Soil Moisture: {soil_moisture} %")
                print(f"CO2 Level: {co2} ppm")

except KeyboardInterrupt:
    # 프로그램 종료 시 시리얼 포트 닫기
    ser.close()
    print("Serial connection closed.")
