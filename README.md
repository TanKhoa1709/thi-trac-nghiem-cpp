# Đề tài môn CTDL&GT: Thi Trắc Nghiệm

## Mô tả
Chương trình quản lý và thực hiện thi trắc nghiệm cho sinh viên. Gồm các chức năng nhập, lưu trữ và xử lý dữ liệu về lớp, sinh viên, môn học, câu hỏi thi và điểm số. Cho phép sinh viên đăng nhập và làm bài thi trắc nghiệm, đồng thời hỗ trợ giáo viên quản lý toàn bộ hệ thống.

## Cấu trúc dữ liệu
- **Danh sách môn học**: Danh sách tuyến tính, mỗi môn học có con trỏ đến cây câu hỏi.
- **Danh sách câu hỏi thi**: Cây nhị phân tìm kiếm, mỗi câu hỏi có ID duy nhất.
- **Danh sách lớp**: Mảng con trỏ, mỗi phần tử trỏ đến danh sách liên kết sinh viên.
- **Danh sách sinh viên**: Danh sách liên kết, mỗi sinh viên có danh sách điểm đã thi.
- **Danh sách điểm thi**: Danh sách liên kết đơn, lưu điểm và bài thi chi tiết.

## Chức năng chính
- Đăng nhập với quyền sinh viên hoặc giáo viên.
- Nhập/xuất lớp, sinh viên, môn học, câu hỏi thi.
- Làm bài thi trắc nghiệm (ngẫu nhiên câu hỏi theo môn).
- Xem lại chi tiết bài thi đã làm.
- In bảng điểm của môn học theo lớp.
- Lưu và đọc dữ liệu từ file.

## File chính
- `main.cpp` – Hàm main và menu chương trình.
- `monhoc.h` – Khai báo & xử lý danh sách môn học.
- `cauhoi.h` – Cấu trúc cây câu hỏi và thao tác câu hỏi.
- `lop.h` – Khai báo mảng lớp và nhập/xuất lớp.
- `sinhvien.h` – Danh sách sinh viên và chức năng xử lý.
- `diemthi.h` – Quản lý điểm thi và bài thi chi tiết.

## Biên dịch
- Sử dụng trình biên dịch C++ như `g++`:
`g++ main.cpp cauhoi.cpp monhoc.cpp sinhvien.cpp diemthi.cpp lop.cpp -o ThiTracNghiem `
- Sau đó chạy `./ThiTracNghiem`
