///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

let Figure = {
    color: "transparent",
    size: null
};

let Square = {
    __proto__: Figure,
};

let Circle = {
    __proto__: Figure
};

function Triangle(lines) {
    this.__proto__ = Figure;
    this.lines = lines;
}

function printDiff(obj1, obj2) {
    for (const key in obj1) {
        if (obj1[key] != obj2[key]) {
            console.log(`свойство ${key} отличается (${obj1[key]}, ${obj2[key]})`)
        }
    }
}

let triangle1 = new Triangle(1);
let triangle2 = new Triangle(3);

console.log(triangle1);
console.log(triangle2);

printDiff(triangle1,triangle2)

///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n');



///////////////// Задание 3 ///////////////
console.log('\n==== Задание 3 ====\n');



///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n');



