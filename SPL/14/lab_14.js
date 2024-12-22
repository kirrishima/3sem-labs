class Sudoku {
    constructor() {
        this.resetBoard();
    }

    resetBoard() {
        this.board = Array.from({ length: 9 }, () => Array(9).fill(0));
    }

    checkRow(row) {
        const seen = new Set();
        for (let col = 0; col < 9; col++) {
            const value = this.board[row][col];
            if (value !== 0) {
                if (seen.has(value)) return false;
                seen.add(value);
            }
        }
        return true;
    }

    checkColumn(col) {
        const seen = new Set();
        for (let row = 0; row < 9; row++) {
            const value = this.board[row][col];
            if (value !== 0) {
                if (seen.has(value)) return false;
                seen.add(value);
            }
        }
        return true;
    }

    checkSquare(row, col) {
        const seen = new Set();
        const startRow = Math.floor(row / 3) * 3;
        const startCol = Math.floor(col / 3) * 3;
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                const value = this.board[startRow + i][startCol + j];
                if (value !== 0) {
                    if (seen.has(value)) return false;
                    seen.add(value);
                }
            }
        }
        return true;
    }

    validateCellValue(value) {
        // Проверка, что значение от 1 до 9 или 0 (пустая ячейка)
        return value > 0 && value <= 9 && Number.isInteger(value);
    }


    checkBoard() {
        const errors = { rows: [], cols: [], squares: [], cells: [] };

        for (let row = 0; row < 9; row++) {
            for (let col = 0; col < 9; col++) {
                const value = this.board[row][col];
                if (!this.validateCellValue(value)) {
                    errors.cells.push({ row, col });
                }
            }
        }

        for (let i = 0; i < 9; i++) {
            if (!this.checkRow(i)) errors.rows.push(i);
            if (!this.checkColumn(i)) errors.cols.push(i);
        }

        for (let row = 0; row < 9; row += 3) {
            for (let col = 0; col < 9; col += 3) {
                if (!this.checkSquare(row, col)) {
                    errors.squares.push({ row, col });
                }
            }
        }

        return errors;
    }

    generateBoard() {
        this.resetBoard();

        const fillBoard = (row, col) => {
            if (row === 9) return true; // Если дошли до конца
            if (col === 9) return fillBoard(row + 1, 0); // Перейти на следующую строку
            if (this.board[row][col] !== 0) return fillBoard(row, col + 1);

            const numbers = Array.from({ length: 9 }, (_, i) => i + 1).sort(() => Math.random() - 0.5);

            for (const num of numbers) {
                this.board[row][col] = num;
                if (this.checkRow(row) && this.checkColumn(col) && this.checkSquare(row, col)) {
                    if (fillBoard(row, col + 1)) return true;
                }
                this.board[row][col] = 0; // Откат
            }

            return false;
        };

        fillBoard(0, 0);
    }
}

const sudoku = new Sudoku();
const sudokuGrid = document.getElementById("sudokuGrid");

function renderBoard(board) {
    sudokuGrid.innerHTML = "";
    for (let row = 0; row < 9; row++) {
        for (let col = 0; col < 9; col++) {
            const cell = document.createElement("input");
            cell.type = "text";
            cell.maxLength = 1;
            cell.value = board[row][col] === 0 ? "" : board[row][col];
            cell.dataset.row = row;
            cell.dataset.col = col;
            cell.addEventListener("input", (e) => {
                const value = parseInt(e.target.value, 10);
                board[row][col] = isNaN(value) ? 0 : value;
            });
            sudokuGrid.appendChild(cell);
        }
    }
}

function highlightErrors(errors) {
    document.querySelectorAll(".grid input").forEach(cell => {
        cell.classList.remove("error", "correct");
    });

    errors.cells.forEach(cell => {
        const _cell = document.querySelector(`input[data-row='${cell.row}'][data-col='${cell.col}']`);
        _cell.classList.add("error");
    });

    errors.rows.forEach(row => {
        for (let col = 0; col < 9; col++) {
            const cell = document.querySelector(`input[data-row='${row}'][data-col='${col}']`);
            cell.classList.add("error");
        }
    });

    errors.cols.forEach(col => {
        for (let row = 0; row < 9; row++) {
            const cell = document.querySelector(`input[data-row='${row}'][data-col='${col}']`);
            cell.classList.add("error");
        }
    });

    errors.squares.forEach(({ row, col }) => {
        const startRow = Math.floor(row / 3) * 3;
        const startCol = Math.floor(col / 3) * 3;
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                const cell = document.querySelector(`input[data-row='${startRow + i}'][data-col='${startCol + j}']`);
                cell.classList.add("error");
            }
        }
    });
}

document.getElementById("generateNew").addEventListener("click", () => {
    sudoku.resetBoard();
    renderBoard(sudoku.board);
});

document.getElementById("checkField").addEventListener("click", () => {
    const errors = sudoku.checkBoard();
    if (errors.rows.length === 0 && errors.cols.length === 0
        && errors.squares.length === 0 && errors.cells.length === 0) {
        document.querySelectorAll(".grid input").forEach(cell => cell.classList.add("correct"));
    } else {
        highlightErrors(errors);
    }
});

document.getElementById("generateSolution").addEventListener("click", () => {
    sudoku.generateBoard();
    renderBoard(sudoku.board);
});

renderBoard(sudoku.board);