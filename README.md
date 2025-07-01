# 📌 Project: Thuật toán MOGA trên STM32F429 – Hiển thị qua UART

## 📘 Mô tả ngắn
Project cài đặt thuật toán tiến hóa đa mục tiêu (Multi-Objective Genetic Algorithm – MOGA) trên vi điều khiển STM32F429ZITx.
Hệ thống hoạt động độc lập và truyền kết quả tối ưu qua giao tiếp UART để hiển thị trên màn hình máy tính.

---

## ⚙️ Công cụ và môi trường phát triển

| Công cụ | Phiên bản |
|--------|-----------|
| STM32CubeIDE | v1.15.0 |
| STM32 HAL Drivers | Bundled trong CubeIDE |
| STM32F429ZIT6 | Chip vi điều khiển |
| UART Terminal (Hercules / Tera Term / PuTTY) | Mới nhất |
| Cáp USB to TTL (nếu không có UART debug) | Tùy chọn |

---

## 🔧 Cách build và chạy project

### A. Cấu hình phần cứng trong STM32CubeIDE:
- Kích hoạt UART2 hoặc UART3 (tuỳ theo chân được kết nối).
- Tốc độ baud: 115200
- Chế độ: TX Only hoặc TX/RX nếu cần
- Clock: cấu hình HSE hoặc HSI tuỳ board.

### B. Build và nạp chương trình:
1. Mở STM32CubeIDE.
2. Import project (`.project` và `.ioc`).
3. Click **Build Project**.
4. Kết nối STM32 với máy tính qua cáp USB hoặc USB-UART.
5. Mở Tera Term hoặc PuTTY, chọn đúng cổng COM.
6. Baudrate: **115200**, 8 bit, no parity, 1 stop bit.
7. Chạy chương trình và theo dõi kết quả trên terminal.

---

## 🧠 Mô tả các module chính

| File/Module | Chức năng |
|-------------|-----------|
| `main.c` | Hàm `main()` khởi tạo hệ thống, gọi thuật toán. |
| `moga.c / .h` | Cài đặt thuật toán MOGA: tạo quần thể, lai ghép, đột biến, chọn lọc. |
| `uart_io.c / .h` | Các hàm tiện ích để in ra UART, dùng `printf()` hoặc `HAL_UART_Transmit()`. |
| `stm32f4xx_hal_*.c` | Thư viện hỗ trợ cho UART, Timer, GPIO... |
| `.ioc` | Cấu hình phần cứng (pinout, clock, UART) tạo bởi STM32CubeMX. |

