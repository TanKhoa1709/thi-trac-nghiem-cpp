# Hệ Thống Quản Lý Thi Trắc Nghiệm (CTDL&GT)

## Tổng Quan

Hệ thống quản lý thi trắc nghiệm toàn diện với phân quyền truy cập cho giáo viên và sinh viên.

## Cấu Trúc Dữ Liệu

### 1. Danh Sách Môn Học (Danh Sách Tuyến Tính)

- **Cấu trúc**: `MAMH (C15)`, `TENMH`, `con trỏ quản lí câu hỏi`
- **Mô tả**: Quản lý các môn học với con trỏ trỏ đến danh sách câu hỏi tương ứng

### 2. Danh Sách Câu Hỏi Thi (Cây Nhị Phân Tìm Kiếm)

- **Khóa**: ID câu hỏi (duy nhất trong toàn hệ thống)
- **Cấu trúc**: `Id`, `Nội dung`, `A`, `B`, `C`, `D`, `Đáp án`
- **Mô tả**: Lưu trữ câu hỏi trắc nghiệm với 4 lựa chọn

### 3. Danh Sách Lớp (Mảng Con Trỏ)

- **Cấu trúc**: `MALOP`, `TENLOP`, `con trỏ quản lí sinh viên`
- **Mô tả**: Quản lý các lớp học với con trỏ trỏ đến danh sách sinh viên

### 4. Danh Sách Sinh Viên (Danh Sách Liên Kết)

- **Cấu trúc**: `MASV`, `HO`, `TEN`, `PHAI`, `password`, `con trỏ quản lí điểm`
- **Mô tả**: Thông tin sinh viên với con trỏ trỏ đến điểm thi

### 5. Danh Sách Điểm Thi (Danh Sách Liên Kết Đơn)

- **Cấu trúc**: `MAMH`, `Điểm`, `Chi Tiết Bài Thi`
- **Mô tả**: Lưu trữ kết quả thi và chi tiết câu trả lời

## Chức Năng Hệ Thống

### Đăng Nhập

- **Sinh viên**: Sử dụng MASV + mật khẩu
- **Giáo viên**: Tài khoản "GV" + mật khẩu "GV" (toàn quyền)

### basic crud

- Nhập/xóa/sửa thông tin lớp, sinh viên, môn học, câu hỏi, ...

### Quản Lý Câu Hỏi

- ID câu hỏi tự động tạo ngẫu nhiên và ID phù hợp cho cây nhị phân câu hỏi
- Không cho phép xóa câu hỏi đã có trong bài thi

### Hệ Thống Thi Trắc Nghiệm

- **Thiết lập trước khi thi**: Chọn môn, số câu hỏi, thời gian thi
- **Lấy câu hỏi ngẫu nhiên**: Tự động chọn câu hỏi từ ngân hàng đề
- **Thi có giới hạn thời gian**: Tính năng đếm ngược thời gian

### Báo Cáo và Thống Kê

- **Chi tiết bài thi**: Xem câu trả lời chi tiết của sinh viên theo môn
- **Bảng điểm lớp**: Điểm thi trắc nghiệm của cả lớp (hiển thị "Chưa thi" nếu chưa làm bài)

## Yêu Cầu Kỹ Thuật

- Lưu trữ dữ liệu vào file
- Kiểm tra và xác thực dữ liệu
- Thiết kế mở rộng cho các tính năng bổ sung
- Quản lý bộ nhớ cho cấu trúc dữ liệu động

## Quy Trình Sử Dụng

1. Đăng nhập với tài khoản phù hợp
2. Thiết lập lớp học và sinh viên (chỉ giáo viên)
3. Cấu hình môn học và câu hỏi (chỉ giáo viên)
4. Sinh viên làm bài thi với câu hỏi ngẫu nhiên
5. Xem kết quả và tạo báo cáo

## Rules of code

- with model like CauHoi, DiemThi, Lop, MonHoc, SinhVien, before adding it into the datastructure (BST, LinkedList, etc.), should create instance in heap (using new) and pass it deferenced value (eg: SinhVien*svMoi = new SinhVien(masv, ho, ten, phai, password); lop->getQuanLySinhVien()->them(*svMoi)). Because if created in stack, it will wipe all data, the datastructure holding that will cause bad behavior (eg: segmentation fault).
