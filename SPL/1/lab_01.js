// задание 1
console.log("\nзадание 1\n");
let a = 5;
console.log(typeof a);

let name = "Name";
console.log(typeof name);

let i = 0;
console.log(typeof i);

let double = 0.23;
console.log(typeof double);

let result = 1 / 0;
console.log(typeof result);

let answer = true;
console.log(typeof answer);

let no = null;
console.log(typeof no);


// задание 2
console.log("\nзадание 2\n");
let squareSide = 5;
let squareArea = squareSide * squareSide;

let rectangleLength = 45;
let rectangleWidth = 21;
let rectangleArea = rectangleLength * rectangleWidth;

let squaresFit = Math.floor(rectangleArea / squareArea);

console.log(`Количество квадратов: ${squaresFit}`);


// задание 3
console.log("\nзадание 3\n");
let i_2 = 2;
let a_2 = i_2++;
let b_2 = ++i_2;
console.log(a_2 == b_2);


// задание 4
console.log("\nзадание 4\n");

console.log('Котик' > 'котик' ? true : false); // false
console.log('Котик' > 'китик' ? true : false); // false
console.log('Кот' > 'Котик' ? true : false); // false
console.log('Привет' > 'Пока' ? true : false); // true
console.log(73 > '53' ? true : false); // true
console.log(false > 0 ? true : false); // false
console.log(54 > true ? true : false); // true
console.log(123 > false ? true : false); // true
console.log(true > '3' ? true : false); // false
console.log(3 > '5mm' ? true : false); // false
console.log(8 > '-2' ? true : false); // true
console.log(34 > '34' ? true : false); // false
console.log(null > undefined ? true : false); // false


// задание 5
console.log("\nзадание 5\n");

const teacherName = "Кудлацкая Марина Фёдоровна".toLowerCase();
const teacherNameParts = teacherName.toLowerCase().split(" ");

const userInput = prompt("введите ваше имя или ИО:");
const userInputParts = userInput.toLowerCase().split(" ");

let flag = true;

if (userInputParts > 3 || userInputParts < 1) {
    flag = false;
}
else {
    for (let index = 0; index < userInputParts.length; index++) {
        if (userInputParts[i] != teacherNameParts[i + 1]) {
            flag = false;
        }
    }
}

if (flag) {
    alert('введенные данные верные');
} else {
    alert('Введенные данные неверные');
}


// задание 6
console.log("\nзадание 6\n");
let examMath = prompt(`Сдал ли студент экзамен по математике? (ДА - "+", НЕТ - "-")`);
let examRussian = prompt(`Сдал ли студент экзамен по русскому? (ДА - "+", НЕТ - "-")`);
let examEnglish = prompt(`Сдал ли студент экзамен по английскому? (ДА - "+", НЕТ - "-")`);

if (examMath == '+' && examRussian == '+' && examEnglish == '+') {
    alert(`Студент переведён на второй курс!`);
} else if (examMath == '-' && examRussian == '-' && examEnglish == '-') {
    alert(`Студент отчислен!`);
} else if (examMath == '-' || examRussian == '-' || examEnglish == '-') {
    alert(`Студента ожидает пересдача!`);
}


// задание 7
console.log("\nзадание 7\n");
let ex7_1 = true + true;
console.log(ex7_1);
let ex7_2 = 0 + "5";
console.log(ex7_2);
let ex7_3 = 5 + "mm";
console.log(ex7_3);
let ex7_4 = 8 / Infinity;
console.log(ex7_4);
let ex7_5 = 9 * "9";
console.log(ex7_5);
let ex7_6 = null - 1;
console.log(ex7_6);
let ex7_7 = "5" - 2;
console.log(ex7_7);
let ex7_8 = "5px" - 3;
console.log(ex7_8);
let ex7_9 = true - 3;
console.log(ex7_9);
let ex7_10 = 7 || 0;  
console.log(ex7_10);


// задание 8
console.log("\nзадание 8\n");
let arr = [10];

for (let index = 0; index < 10; index++) {
    arr[index] = index + 1;

    if (arr[index] % 2 == 0)
        arr[index] += 2;

    if (arr[index] % 2 != 0)
        arr[index] = `${arr[index]}mm`;

    console.log(`${arr[index]}`);
}


// задание 9
console.log("\nзадание 9\n");

//объект
let daysOfWeek = {
    1: "Понедельник",
    2: "Вторник",
    3: "Среда",
    4: "Четверг",
    5: "Пятница",
    6: "Суббота",
    7: "Воскресенье"
};

let dayNumber = parseInt(prompt("Введите номер дня недели (1-7):"));
alert(daysOfWeek[dayNumber] || "Некорректный номер дня недели");

//массив
let days = ["Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"];

dayNumber = parseInt(prompt("Введите номер дня недели (1-7):"));
alert(days[dayNumber - 1] || "Некорректный номер дня недели");


// задание 10
console.log("\nзадание 10\n");

function createString(param1 = "по умолчанию", param2, param3) {
    return `${param1}, ${param2}, ${param3}`;
}

let userParam3 = prompt("Введите третий параметр:");
alert(createString("Первый параметр", "Второй параметр", userParam3));


// задание 11
console.log("\nзадание 11\n");

// Function Declaration
function paramsDeclaration(a, b) {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
}

// Function Expression
const paramsExpression = function (a, b) {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
};

// Arrow Function
const paramsArrow = (a, b) => (a === b ? 4 * a : a * b);

const sideA = parseFloat(prompt("Введите длину стороны A:"));
const sideB = parseFloat(prompt("Введите длину стороны B:"));

const resultDeclaration = paramsDeclaration(sideA, sideB);
const resultExpression = paramsExpression(sideA, sideB);
const resultArrow = paramsArrow(sideA, sideB);

if (sideA === sideB) {
    console.log(`Это квадрат, его периметр: ${resultDeclaration}`);
} else {
    console.log(`Это прямоугольник, его площадь (Function Declaration): ${resultDeclaration}`);
}

if (sideA === sideB) {
    console.log(`Это квадрат, его периметр: ${resultExpression}`);
} else {
    console.log(`Это прямоугольник, его площадь (Function Expression): ${resultExpression}`);
}

if (sideA === sideB) {
    console.log(`Это квадрат, его периметр: ${resultArrow}`);
} else {
    console.log(`Это прямоугольник, его площадь (Arrow Function): ${resultArrow}`);
}
