import cv2
import numpy as np


# Hàm callback cho các thanh kéo (trackbars)
def nothing(x):
    pass


# Tạo một cửa sổ
cv2.namedWindow('HSV Color Picker')

# Tạo các thanh kéo cho H, S và V
cv2.createTrackbar('H', 'HSV Color Picker', 0, 179, nothing)  # H có giá trị từ 0 đến 179
cv2.createTrackbar('S', 'HSV Color Picker', 255, 255, nothing)  # S có giá trị từ 0 đến 255
cv2.createTrackbar('V', 'HSV Color Picker', 255, 255, nothing)  # V có giá trị từ 0 đến 255

img_hsv = np.zeros((250, 500, 3), np.uint8)
while True:
    # Lấy giá trị hiện tại của các thanh kéo
    h = cv2.getTrackbarPos('H', 'HSV Color Picker')
    s = cv2.getTrackbarPos('S', 'HSV Color Picker')
    v = cv2.getTrackbarPos('V', 'HSV Color Picker')

    # Tạo một hình ảnh màu HSV với các giá trị hiện tại của H, S, V
    img_hsv[:] = (h, s, v)
    img_bgr = cv2.cvtColor(img_hsv, cv2.COLOR_HSV2BGR)



    # Hiển thị hình ảnh
    cv2.imshow('HSV Color Picker', img_bgr)

    # Nhấn phím 'q' để thoát
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Đóng tất cả các cửa sổ
cv2.destroyAllWindows()