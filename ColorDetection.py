import cv2
import requests

# Địa chỉ IP ESP32 (xem trên Serial Monitor)
ESP32_IP = 'http://192.168.43.147'  # Thay bằng IP thật của ESP32

cap = cv2.VideoCapture(0)
last_color = ""

while True:
    ret_frame, frame = cap.read()
    if not ret_frame:
        continue

    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    height, width, _ = frame.shape
    cx = int(width / 2)
    cy = int(height / 2)

    pixel_center = hsv_frame[cy, cx]
    hue_value = pixel_center[0]

    color = "UNDEFINED"
    if 0 < hue_value < 26:
        color = "ORANGE"
    # elif hue_value < 100:
    #     color = "GREEN"
    elif 100 < hue_value < 113:
        color = "BLUE"

    # Gửi màu đến ESP32 qua HTTP khi màu thay đổi
    if color in ['ORANGE', 'BLUE',] and color != last_color:
        try:
            url = f"{ESP32_IP}/color?name={color}"
            response = requests.get(url, timeout=0.5)

            if response.status_code == 200:
                print(f"Đã gửi màu {color} đến ESP32")
                last_color = color
            else:
                print(f"ESP32 trả về mã lỗi: {response.status_code}")

        except requests.exceptions.RequestException:
            print(f"Không thể gửi màu {color} đến ESP32")

    # Hiển thị khung hình
    cv2.putText(frame, color, (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 2)
    cv2.circle(frame, (cx, cy), 5, (0, 0, 255), 3)
    cv2.imshow("Color Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
