// Задание 1

console.log('Пример 1');

function makeCounter() {
    let currentCount = 1;

    return function () { // (*) return currentCount++;
        return currentCount++;
    };
}

let counter = makeCounter();

console.log(counter()); // 1
console.log(counter()); // 2 
console.log(counter()); // 3

let counter2 = makeCounter();

console.log(counter2()); // 1


console.log('Пример 2');
let currentCount2 = 1;
function makeCounter2() {
    return function () { // (*) return currentCount++;
        return currentCount2++;
    };
}

let counter_1 = makeCounter2();
let counter_2 = makeCounter2();

console.log(counter_1()); // 1
console.log(counter_1()); // 2 

console.log(counter_2()); // 3
console.log(counter_2()); // 4


// Задание 2
console.log('Задание 2');

function calcVolume(length) {
    return function (width) {
        return function (height) {
            return length * width * height;
        };
    };
}

console.log(calcVolume(2)(3)(5));

const calcVolumeLen2 = calcVolume(2);
console.log(calcVolumeLen2(6)(2));
console.log(calcVolumeLen2(4)(5));


// Задание 
console.log('Задание 3')

// function* moveObj() {
//     let x = 0;
//     let y = 0;

//     for (let i = 0; i < 10; i++) {
//         const direction = yield { x, y };
//         switch (direction) {
//             case 'l':
//                 x -= 10;
//                 break;
//             case 'r':
//                 x += 10;
//                 break;
//             case 'u':
//                 y += 10;
//                 break;
//             case 'd':
//                 y -= 10;
//                 break;
//             default:
//                 break;
//         }
//         console.log(`x:${x}, y:${y}`);
//     }
//     return;
// }

// let mov = moveObj();

// mov.next();

// for (let index = 0; index < 10; index++) {
//     mov.next(prompt());
// }

// Задание 4

// for (let prop in window) {
//     if (window.hasOwnProperty(prop)) {
//       console.log(`Property: ${prop}, Value: ${window[prop]}`);
//     }
//   }


var globalVar = "Это глобальная переменная";
let localVar = "Это локальная переменная";
const constantVar = "Это константа";

function globalFunction() {
    return "Это глобальная функция";
}

console.log(window.globalVar); // "Это глобальная переменная"
console.log(window.localVar); // undefined, так как let не добавляет переменную в объект window
console.log(window.constantVar); // undefined, так как const не добавляет переменную в объект window
console.log(window.globalFunction()); // "Это глобальная функция"

// Шаг 3: Переопределение переменной через window
window.globalVar = "Глобальная переменная изменена";
console.log(globalVar); // "Глобальная переменная изменена"

// Переопределение функции через window
window.globalFunction = function () {
    return "Глобальная функция изменена";
};
console.log(globalFunction()); // "Глобальная функция изменена"

