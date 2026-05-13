# 🕹️ Tetris Classic Game

Chào mừng bạn đến với dự án **Tetris**! Đây là phiên bản tái hiện lại trò chơi xếp gạch huyền thoại, được phát triển với mục tiêu học hỏi về tư duy lập trình logic, quản lý bộ nhớ và cấu trúc dữ liệu.

## 🌟 Tính năng nổi bật
- **Core Gameplay:** Xoay, di chuyển và làm biến mất các hàng gạch hoàn chỉnh một cách mượt mà.
- **Hệ thống tính điểm:** Tăng điểm dựa trên số hàng bị xóa đồng thời.
- **Cấp độ (Level):** Tốc độ rơi của gạch tăng dần theo thời gian để thử thách người chơi. 📈
- **Preview:** Hiển thị khối gạch tiếp theo (Next Piece).
- **High Score:** Lưu giữ kỷ lục điểm số cao nhất.

## 🛠️ Công nghệ sử dụng
- **Ngôn ngữ:** C++ / Python
- **Thư viện:** Pygame
- **Hệ điều hành:** Ưu tiên chạy mượt mà trên **Arch Linux** 🐧.

## 🚀 Hướng dẫn cài đặt & Chạy

**1. Clone project:**
```bash
git clone [https://github.com/pnkhp/tetrisgame.git](https://github.com/pnkhp/tetrisgame.git)
cd tetrisgame
```
2. Cài đặt môi trường:

Đảm bảo đã cài đặt trình biên dịch/thông dịch phù hợp (g++ và python3).

Cài đặt thư viện phụ thuộc (nếu dùng phiên bản Python):

```bash
pip install pygame
```
3. Biên dịch và thực thi:
Nếu chạy phiên bản Python:

```bash
python main.py
```
Nếu chạy phiên bản C++:

```bash
g++ frame.cpp -o tetris
./tetris
```
🎮 Cách điều khiển
Phím ← / →: Di chuyển sang trái/phải.

Phím ↑: Xoay gạch.

Phím ↓: Rơi nhanh (Soft Drop).

Phím Space: Rơi tức thì (Hard Drop).

Phím P: Tạm dừng (Pause).

🧩 Cấu trúc Code cần lưu ý (Tiểu tiết quan trọng)
Ma trận Board (Grid): Trái tim của game. Sử dụng mảng 2 chiều để map chính xác tọa độ trạng thái các ô vuông. Đòi hỏi quản lý cấp phát/giải phóng bộ nhớ chặt chẽ (với C++) để tránh memory leak.

Logic va chạm (Collision Detection): Tính toán tọa độ lưới nghiêm ngặt. Phải kiểm tra biên (tường) và các khối gạch đã "đóng băng" trước khi cấp quyền di chuyển hoặc xoay khối mới.

Vòng lặp Game (Game Loop): Áp dụng mô hình Input -> Update Logic -> Render. Phải khống chế Frame Rate (FPS) ổn định để game không bị trôi quá nhanh trên các máy có cấu hình mạnh.

🤝 Đóng góp
Nếu bạn có ý tưởng tối ưu hóa thuật toán duyệt mảng, hay cải thiện hiệu năng xử lý, đừng ngần ngại tạo Pull Request nhé! 💡

✨ Phát triển bởi IloveUS - Nhóm Sinh viên Mạng máy tính UIT.
