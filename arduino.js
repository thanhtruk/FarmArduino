function openRoof() {
    // Viết mã JS để mở mái che khi nút được nhấn
    // Ví dụ: gửi yêu cầu đến server để mở mái che
    console.log('Mở mái che');
}

function closeRoof() {
    // Viết mã JS để đóng mái che khi nút được nhấn
    // Ví dụ: gửi yêu cầu đến server để đóng mái che
    console.log('Đóng mái che');
}

//hiển thị cường độ ánh sáng
function showLightIntensity() {
    var lightIntensityElement = document.getElementById('light-intensity');
    if (lightIntensityElement) {
        var lightIntensity = lightIntensityElement.value;
        document.getElementById('light-intensity-value').textContent = lightIntensity;
        console.log('Cường độ ánh sáng: ' + lightIntensity);
    } else {
        console.log('Không tìm thấy phần tử có ID là "lightIntensity"');
    }
}