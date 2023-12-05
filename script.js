document.addEventListener("DOMContentLoaded", function () {
    const motionButton = document.getElementById("motionButton");
    const motionAlert = document.getElementById("motionAlert");

    motionButton.addEventListener("click", function () {
        if (motionButton.classList.contains("on")) {
            motionButton.classList.remove("on");
            motionButton.classList.add("off");
            motionButton.textContent = "Tắt";
            motionAlert.style.display = "none";
        } else {
            motionButton.classList.remove("off");
            motionButton.classList.add("on");
            motionButton.textContent = "Bật";
            motionAlert.style.display = "block";
        }
    });
});
