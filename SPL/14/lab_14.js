class Sudoku {
    constructor() {
        this.resetBoard(); // Инициализация доски при создании экземпляра Sudoku
    }

    // Сбрасывает игровую доску, заполняя её нулями (пустыми ячейками)
    resetBoard() {
        this.board = Array.from({ length: 9 }, () => Array(9).fill(0));
    }

    // Проверяет строку на наличие повторяющихся чисел
    checkRow(row) {
        const seen = new Set();
        for (let col = 0; col < 9; col++) {
            const value = this.board[row][col];
            if (value !== 0) { // Игнорируем пустые ячейки
                if (seen.has(value)) return false; // Если число уже встречалось, строка некорректна
                seen.add(value);
            }
        }
        return true; // Строка корректна
    }

    // Проверяет колонку на наличие повторяющихся чисел
    checkColumn(col) {
        const seen = new Set();
        for (let row = 0; row < 9; row++) {
            const value = this.board[row][col];
            if (value !== 0) { // Игнорируем пустые ячейки
                if (seen.has(value)) return false; // Если число уже встречалось, колонка некорректна
                seen.add(value);
            }
        }
        return true; // Колонка корректна
    }

    // Проверяет 3x3 квадрат, в который входит ячейка, на наличие повторяющихся чисел
    checkSquare(row, col) {
        const seen = new Set();
        const startRow = Math.floor(row / 3) * 3; // Определяем начальную строку квадрата
        const startCol = Math.floor(col / 3) * 3; // Определяем начальную колонку квадрата
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                const value = this.board[startRow + i][startCol + j];
                if (value !== 0) { // Игнорируем пустые ячейки
                    if (seen.has(value)) return false; // Если число уже встречалось, квадрат некорректен
                    seen.add(value);
                }
            }
        }
        return true; // Квадрат корректен
    }

    // Проверяет, что значение в ячейке является целым числом от 1 до 9 или 0 (пустая ячейка)
    validateCellValue(value) {
        return value >= 0 && value <= 9 && Number.isInteger(value);
    }

    // Проверяет всё игровое поле на ошибки
    checkBoard() {
        const errors = { rows: [], cols: [], squares: [], cells: [] }; // Объект для хранения ошибок

        // Проверка значений в каждой ячейке
        for (let row = 0; row < 9; row++) {
            for (let col = 0; col < 9; col++) {
                const value = this.board[row][col];
                if (!this.validateCellValue(value)) {
                    errors.cells.push({ row, col }); // Записываем некорректные значения
                }
            }
        }

        // Проверка строк и колонок
        for (let i = 0; i < 9; i++) {
            if (!this.checkRow(i)) errors.rows.push(i); // Строка с ошибками
            if (!this.checkColumn(i)) errors.cols.push(i); // Колонка с ошибками
        }

        // Проверка квадратов 3x3
        for (let row = 0; row < 9; row += 3) {
            for (let col = 0; col += 3) {
                if (!this.checkSquare(row, col)) {
                    errors.squares.push({ row, col }); // Квадрат с ошибками
                }
            }
        }

        return errors; // Возвращаем объект с ошибками
    }

    // Генерирует заполненное игровое поле, используя рекурсивный алгоритм с проверкой
    generateBoard() {
        this.resetBoard(); // Сбрасываем игровое поле

        const fillBoard = (row, col) => {
            if (row === 9) return true; // Если дошли до конца, всё заполнено
            if (col === 9) return fillBoard(row + 1, 0); // Переходим на следующую строку
            if (this.board[row][col] !== 0) return fillBoard(row, col + 1); // Пропускаем заполненные ячейки

            // Создаём массив чисел от 1 до 9 в случайном порядке
            const numbers = Array.from({ length: 9 }, (_, i) => i + 1).sort(() => Math.random() - 0.5);

            for (const num of numbers) {
                this.board[row][col] = num; // Пробуем вставить число
                // Проверяем, корректно ли заполнение
                if (this.checkRow(row) && this.checkColumn(col) && this.checkSquare(row, col)) {
                    if (fillBoard(row, col + 1)) return true; // Если дальше всё корректно, выходим
                }
                this.board[row][col] = 0; // Если ошибка, откатываем значение
            }

            return false; // Если ни одно число не подошло, возвращаемся назад
        };

        fillBoard(0, 0); // Начинаем заполнение с первой ячейки
    }
}

// Создаём экземпляр игры Sudoku
const sudoku = new Sudoku();
const sudokuGrid = document.getElementById("sudokuGrid");

// Отображает игровую доску в HTML
function renderBoard(board) {
    sudokuGrid.innerHTML = ""; // Очищаем контейнер
    for (let row = 0; row < 9; row++) {
        for (let col = 0; col < 9; col++) {
            const cell = document.createElement("input"); // Создаём поле ввода
            cell.type = "text";
            cell.maxLength = 1; // Ограничиваем ввод до одного символа
            cell.value = board[row][col] === 0 ? "" : board[row][col]; // Пустые ячейки отображаются как пустые строки
            cell.dataset.row = row; // Добавляем атрибут строки
            cell.dataset.col = col; // Добавляем атрибут колонки
            cell.addEventListener("input", (e) => {
                const value = parseInt(e.target.value, 10);
                board[row][col] = isNaN(value) ? 0 : value; // Если ввод некорректен, считаем ячейку пустой
            });
            sudokuGrid.appendChild(cell); // Добавляем ячейку в контейнер
        }
    }
}

// Подсвечивает ячейки с ошибками
function highlightErrors(errors) {
    document.querySelectorAll(".grid input").forEach(cell => {
        cell.classList.remove("error", "correct"); // Убираем подсветку
    });

    errors.cells.forEach(cell => {
        const _cell = document.querySelector(`input[data-row='${cell.row}'][data-col='${cell.col}']`);
        _cell.classList.add("error"); // Подсвечиваем некорректные ячейки
    });

    errors.rows.forEach(row => {
        for (let col = 0; col < 9; col++) {
            const cell = document.querySelector(`input[data-row='${row}'][data-col='${col}']`);
            cell.classList.add("error"); // Подсвечиваем строки с ошибками
        }
    });

    errors.cols.forEach(col => {
        for (let row = 0; row < 9; row++) {
            const cell = document.querySelector(`input[data-row='${row}'][data-col='${col}']`);
            cell.classList.add("error"); // Подсвечиваем колонки с ошибками
        }
    });

    errors.squares.forEach(({ row, col }) => {
        const startRow = Math.floor(row / 3) * 3;
        const startCol = Math.floor(col / 3) * 3;
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                const cell = document.querySelector(`input[data-row='${startRow + i}'][data-col='${startCol + j}']`);
                cell.classList.add("error"); // Подсвечиваем квадраты с ошибками
            }
        }
    });
}

// Сбрасываем игровое поле по нажатию на кнопку "Generate New"
document.getElementById("generateNew").addEventListener("click", () => {
    sudoku.resetBoard();
    renderBoard(sudoku.board);
});

// Проверяем игровое поле на ошибки по нажатию на кнопку "Check Field"
document.getElementById("checkField").addEventListener("click", () => {
    const errors = sudoku.checkBoard();
    if (errors.rows.length === 0 && errors.cols.length === 0
        && errors.squares.length === 0 && errors.cells.length === 0) {
        document.querySelectorAll(".grid input").forEach(cell => cell.classList.add("correct")); // Если ошибок нет, подсвечиваем зелёным
    } else {
        highlightErrors(errors); // Иначе подсвечиваем ошибки
    }
});

// Генерируем полное решение по нажатию на кнопку "Generate Solution"
document.getElementById("generateSolution").addEventListener("click", () => {
    sudoku.generateBoard();
    renderBoard(sudoku.board);
});

// Отображаем пустую доску при загрузке страницы
renderBoard(sudoku.board);
