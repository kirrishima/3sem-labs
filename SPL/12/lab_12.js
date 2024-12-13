class Sudoku {
    constructor() {
        this.resetBoard();
    }

    // Метод для сброса игрового поля до исходного состояния (пустого)
    resetBoard() {
        this.board = Array.from({ length: 9 }, () => Array(9).fill(0));
    }

    // Метод для проверки строки на наличие повторяющихся цифр
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

    // Метод для проверки столбца на наличие повторяющихся цифр
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

    // Метод для проверки квадрата 3x3 на наличие повторяющихся цифр
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

    // Метод для проверки всего игрового поля
    checkBoard() {
        const errors = { rows: [], cols: [], squares: [] };

        for (let i = 0; i < 9; i++) {
            if (!this.checkRow(i)) errors.rows.push(i);
            if (!this.checkColumn(i)) errors.cols.push(i);
        }

        for (let row = 0; row < 9; row += 3) {
            for (let col = 0; col < 9; col += 3) {
                if (!this.checkSquare(row, col)) {
                    errors.squares.push(`[${row}, ${col}]`);
                }
            }
        }

        return errors;
    }

    // Метод для генерации правильно заполненного игрового поля
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

    // Метод для вывода игрового поля в консоль
    printBoard() {
        console.log(this.board.map((row, rowIndex) => {
            const formattedRow = row.map((cell, colIndex) => {
                return (colIndex + 1) % 3 === 0 && colIndex !== 8 ? cell + ' |' : cell;
            }).join(' ');
            return (rowIndex + 1) % 3 === 0 && rowIndex !== 8
                ? formattedRow + '\n---------------------'
                : formattedRow;
        }).join('\n'));
    }
}

// Пример использования
const sudoku = new Sudoku();
sudoku.generateBoard();
sudoku.printBoard();
const errors = sudoku.checkBoard();
console.log("Ошибки:", errors);
