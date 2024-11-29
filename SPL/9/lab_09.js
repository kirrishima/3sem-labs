///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

let Figure = {
    color: "transparent",
    size: null
};

function Square(size) {
    this.__proto__ = Figure;
    this.size = size;
};

function Circle(color) {
    this.__proto__ = Figure;
    this.color = color;
};

function Triangle(lines) {
    this.__proto__ = Figure;
    this.lines = lines;
}

function printDiff(obj1, obj2) {
    let keys1 = Object.keys(obj1).filter(k => !(k in obj2));
    let keys2 = Object.keys(obj2).filter(k => !(k in obj1));

    for (const key in obj1) {
        if (obj1[key] != obj2[key]) {
            console.log(`свойство ${key} отличается (${obj1[key]}, ${obj2[key]})`)
        }
    }
    console.log(`Уникальные свойства первого объекта: ${keys1}`);
    console.log(`Уникальные свойства второго объекта: ${keys2}`);
}

let triangle1 = new Triangle(1);
let triangle2 = new Triangle(3);

printDiff(triangle1, triangle2)
console.log('\n');

let circle1 = new Circle("none");
let circle2 = new Circle("green");

printDiff(circle1, circle2)
console.log('\n');

let square1 = new Square(10);
let square2 = new Square(2);

printDiff(square1, square2)

///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n');



///////////////// Задание 3 ///////////////
console.log('\n==== Задание 3 ====\n');



///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n');



