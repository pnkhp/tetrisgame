
# 🕹️ Tetris Classic Game

Chào mừng bạn đến với dự án **Tetris**! Đây là phiên bản tái hiện lại trò chơi xếp gạch huyền thoại, được phát triển với mục tiêu học hỏi về tư duy lập trình logic, quản lý bộ nhớ và cấu trúc dữ liệu.

## 🌟 Tính năng nổi bật
- **Core Gameplay:** Xoay, di chuyển và làm biến mất các hàng gạch hoàn chỉnh một cách mượt mà.
- **Hệ thống tính điểm:** Tăng điểm dựa trên số hàng bị xóa đồng thời.
- **Cấp độ (Level):** Tốc độ rơi của gạch tăng dần theo thời gian để thử thách người chơi. 📈
- **Preview:** Hiển thị khối gạch tiếp theo (Next Piece).
- **High Score:** Lưu giữ kỷ lục người chơi cao nhất.

## 🛠️ Công nghệ sử dụng
- **Ngôn ngữ:**  C++ / Python 
- **Thư viện:**  Pygame 
- **Hệ điều hành:** Ưu tiên chạy tốt trên **Arch Linux** 🐧.

## 🚀 Hướng dẫn cài đặt & Chạy

1. **Clone project:**
   ```bash
   git clone [https://github.com/pnkhp/tetrisgame)
   cd tetris-game
   ```
2. **Cài đặt môi trường:**
   - Đảm bảo bạn đã cài đặt trình biên dịch phù hợp (ví dụ: `g++` hoặc `python3`).
   - Cài đặt thư viện phụ thuộc (nếu có).

3. **Biên dịch và thực thi:**
   ```bash
   # Ví dụ nếu dùng g++
   g++ frame.cpp -o tetris
   ./tetris
   ```

## 🎮 Cách điều khiển
- **Phím ← / →**: Di chuyển sang trái/phải.
- **Phím ↑**: Xoay gạch.
- **Phím ↓**: Rơi nhanh (Soft Drop).
- **Phím Space**: Rơi tức thì (Hard Drop).
- **Phím P**: Tạm dừng (Pause).

## 🧩 Cấu trúc Code cần lưu ý (Tiểu tiết quan trọng)
- **Ma trận Board:** Sử dụng mảng 2 chiều để quản lý trạng thái các ô vuông trên màn hình.
- **Logic va chạm (Collision Detection):** Kiểm tra biên và các khối gạch đã cố định trước khi cho phép di chuyển/xoay.
- **Vòng lặp Game (Game Loop):** Xử lý Input -> Cập nhật logic -> Vẽ (Render) để đảm bảo không bị giật lag.

## 🤝 Đóng góp
Nếu bạn có ý tưởng nào để cải thiện thuật toán xử lý hàng gạch hoặc tối ưu hóa hiệu năng, đừng ngần ngại tạo một **Pull Request** nhé! 💡

---
✨ Phát triển bởi IloveUS - Nhóm Sinh viên mạng máy tính UIT.

