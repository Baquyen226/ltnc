# YetAnotherTetris

## Description
Đây là một bản clone của Tetris trong C++, sử dung SDL3.
Điều khiển:
- Các nút mũi tên : Di chuyển
- Space : Lập tức thả miếng gạch hiện tại xuống
- Z : Xoay miếng gạch theo chiều kim đồng hồ
- X : Xoay miếng gạch ngược chiều kim đồng hồ
- C : Giữ miếng gạch hiện tại để sử dụng sau

## Dependencies
- Microsoft Visual Studio
- SDL3 Development Libaries (link tải cả các header và library: https://drive.google.com/file/d/12GsNO4KY3PEN4XhNHNTD6kLaEHqK52Ul/view?usp=sharing)

## Compile
1. Clone repo về máy
```sh
git clone https://github.com/Baquyen226/ltnc.git
```
2. Tạo project mới, chọn Empty Project(C++), chọn nơi clone repo làm Project Location
3. Trong Solution Explorer, click chuột phải vào Source Files → Add → Existing Item, và chọn các file source. Click chuột phải vào Header Files → Add → Existing Item và chọn toàn bộ file header
4. Click chuột phải vào Project → Properties:
- Vào C/C++ → General, tìm Additional Include Directories, thêm đường dẫn đến thư mục include của SDL
- Vào Linker → General, tìm Additional Library Directories, thêm đường dẫn đến thư mục lib của SDL
- Vào Linker → Input, tìm Additional Dependencies, thêm toàn bộ các file có dạng .lib
5. Build và Run

## Reflection
- Hiện tại em thấy code vẫn còn rất lộn xộn, nhất là trong việc xử lí các GameState, ngoài ra, cách lưu trữ dữ lieu của các tetromino cũng chưa that sự hiệu quả. Trong thời gian tới em sẽ cố gắng cải thiện cách tổ chức code, và thêm một số chức năng mới(nếu có thể :D)

## Credits

- Âm thanh: https://you.have.fail/ed/at/tetrioplus/#sfx-8bJackie-Tetrio_retro_pack
- SRS: https://tetris.fandom.com/wiki/SRS