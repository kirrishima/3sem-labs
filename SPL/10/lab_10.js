// Пример 1
const set = new Set([1, 1, 2, 3, 4]);
console.log(set);

// Пример 3
const name = "Lydia";
age = 21;
console.log(delete name);
console.log(delete age);

// Пример 4
const numbers = [1, 2, 3, 4, 5];
const [y] = numbers;
console.log(y);

// Пример 5
const user = { name: "Lydia", age: 21 };
const admin = { admin: true, ...user };
console.log(admin);

// Пример 6
const person = { name: "Lydia" };
Object.defineProperty(person, "age", { value: 21 });
console.log(person);
console.log(Object.keys(person));

// Пример 7
const a = {};
const b = { key: "b" };
const c = { key: "c" };

a[b] = 123;
a[c] = 456;

console.log(a[b]);

// Пример 8
let num = 10;

const increaseNumber = () => num++;
const increasePassedNumber = (number) => number++;

const num1 = increaseNumber();
const num2 = increasePassedNumber(num1);

console.log(num1);
console.log(num2);

console.log('Пример 9')
// Пример 9
const value = { number: 10 };

const multiply = (x = { ...value }) => {
    console.log(x.number *= 2);
};

multiply();
multiply();
multiply(value);
multiply(value);

// Пример 10
[1, 2, 3, 4].reduce((x, y) => console.log(x, y));