///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n')

let person = {
    name: "Oleg",
    age: 52,
    greet: function () { console.log(`Привет, ${this.name}!`); },
    ageAfterYears: function (years) { return this.age + years; }
}

person.greet();
console.log(person.ageAfterYears(18));

///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n')

let car = {
    model: "Tesla",
    year: 2077,
    getInfo: function () { console.log(`Модель: ${this.model}, Year: ${this.year}`) }
}

car.getInfo();

///////////////// Задание 3 ///////////////
console.log('\n==== Задание 3 ====\n')



///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n')



