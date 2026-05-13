# Tetris OOP – Tuần 3

## Phân công

| File                  | Người làm     | Việc cần làm                                    |
|-----------------------|---------------|-------------------------------------------------|
| `CBlock.h/.cpp`       | **Thuận**     | `load`, `rotated`, `copyFrom`, `clear`          |
| `CBoard.h/.cpp`       | **Thuận**     | `init`, `canMove`, `removeBlock`, `placeBlock`, `clearLines` |
| `CScoreManager.h/.cpp`| **Long Nhật** | `load`, `save`, `calcScore`, `addLines`         |
| `CRenderer.h/.cpp`    | **Linh**      | `gotoxy`, `applyColor`, `padLeft`, `draw`, `drawGameOver` |
| `CInput.h/.cpp`       | **Hồng Phúc** | `hasInput`, `getChar`, `getArrowKey`, `sleepMs` |
| `CGame.h/.cpp`        | **Hoàng Nhật**| `shuffleBag`, `updateNextBlock`, `spawnBlock`, `tryRotate`, `hardDrop`, `handleInput`, `run` |

## Quy tắc

- **Chỉ sửa file được giao**, không đụng file của người khác
- Mỗi hàm có comment `// TODO` — điền vào đó, không thay đổi chữ ký hàm
- Tham khảo `tetris_oop.cpp` (bản hoàn chỉnh) nếu bí

## Compile

```bash
g++ -std=c++14 -o tetris main.cpp CBlock.cpp CBoard.cpp CScoreManager.cpp CRenderer.cpp CInput.cpp CGame.cpp
./tetris
```

## Phụ thuộc giữa các file

```
common.h
  └── CBlock.h/.cpp       (Thuận)
        └── CBoard.h/.cpp (Thuận)
              └── CScoreManager.h/.cpp (Long Nhật)
              └── CRenderer.h/.cpp     (Linh)
              └── CInput.h/.cpp        (Hồng Phúc)
                    └── CGame.h/.cpp   (Hoàng Nhật)
                          └── main.cpp
```
