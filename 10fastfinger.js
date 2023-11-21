const textarea = document.getElementById('typing-box');
textarea.addEventListener('input', function () {
    Array.from(document.getElementsByClassName('key')).forEach(function (keyElement) {
        keyElement.style.backgroundColor = '#EEEEEE';
    });
    const inputValue = textarea.value.trim();
    const lastTypedChar = inputValue[inputValue.length - 1];
    if (lastTypedChar) {
        const keyElement = document.getElementById(lastTypedChar.toUpperCase());
        const quesWord = document.getElementById('random-word').textContent;
        if (quesWord.slice(0, inputValue.length) === inputValue) {
            keyElement.style.backgroundColor = '#00ADB5';
        } else {
            keyElement.style.backgroundColor = '#F05454';
        }
    }
});

document.addEventListener("keyup", function (event) {
    if (event.key === 'Enter') {
        Array.from(document.getElementsByClassName('key')).forEach(function (keyElement) {
            keyElement.style.backgroundColor = '#EEEEEE';
        });
        if (textarea.value.trim() === document.getElementById('random-word').textContent) {
            document.getElementById('correctCount').textContent = parseInt(document.getElementById('correctCount').textContent) + 1;
        } else {
            document.getElementById('incorrectCount').textContent = parseInt(document.getElementById('incorrectCount').textContent) + 1;
        }
        textarea.value = '';
        const words = ['apple', 'banana', 'orange', 'grape', 'kiwi'];
        const randomIndex = Math.floor(Math.random() * words.length);
        const randomWord = words[randomIndex];
        document.getElementById('random-word').textContent = randomWord;
    }
});