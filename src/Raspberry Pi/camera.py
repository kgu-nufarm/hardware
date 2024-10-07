import cv2
import requests
import time

# 서버 URL 설정 (모델 서버의 엔드포인트 주소)
SERVER_URL = 'http://model-server-ip-address:port/receive_frame'

# 웹캠에서 비디오 스트림을 캡처합니다.
camera = cv2.VideoCapture(0)

try:
    while True:
        # 웹캠으로부터 프레임을 읽어옵니다.
        success, frame = camera.read()
        if not success:
            break

        # 프레임을 JPEG 형식으로 인코딩합니다.
        _, buffer = cv2.imencode('.jpg', frame)
        frame_bytes = buffer.tobytes()

        # 현재 시간을 타임스탬프로 추가하여 프레임과 함께 전송합니다.
        timestamp = time.time()

        # 프레임을 서버로 POST 요청을 통해 전송합니다.
        response = requests.post(SERVER_URL, files={'frame': frame_bytes}, data={'timestamp': timestamp})

        # 서버의 응답을 출력합니다.
        if response.status_code == 200:
            print('Frame sent successfully.')
        else:
            print(f'Failed to send frame. Status code: {response.status_code}')

        # 약간의 지연을 추가합니다 (0.1초). 이것은 필요에 따라 조절 가능.
        time.sleep(0.1)

except KeyboardInterrupt:
    # 프로그램 종료 시 카메라 리소스 해제
    camera.release()
    print("Stopped by the user.")

finally:
    camera.release()
