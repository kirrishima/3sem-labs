///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

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
console.log('\n==== Задание 3 ====\n');

function Book(title, author) {
    this.title = title;
    this.author = author;
    this.getTitle = function () {
        return this.title;
    };
    this.getAuthor = function () {
        return this.author;
    };
}

let myBook = new Book("Фотолупы 2024", "Валерий Жмышенко");
console.log(myBook.getTitle()); // JavaScript: The Good Parts
console.log(myBook.getAuthor()); // Douglas Crockford

///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n');

let team = {
    players: ["Игрок 1", "Игрок 2", "Игрок 3"],
    showPlayers() {
        this.players.forEach(player => {
            console.log(`${this.players.indexOf(player) + 1}. ${player}`);
        });
    }
};

team.showPlayers();

///////////////// Задание 5 ///////////////
console.log('\n==== Задание 5 ====\n');

const counter = (function () {
    let count = 0;
    return {
        increment() {
            return ++count;
        },
        decrement() {
            return --count;
        },
        getCount() {
            return count;
        }
    };
})();

console.log(counter.increment());
console.log(counter.increment());
console.log(counter.decrement());
console.log(counter.getCount());

///////////////// Задание 6 ///////////////
console.log('\n==== Задание 6 ====\n');

let item = {};
Object.defineProperty(item, "price", {
    value: 100,
    writable: true,
    configurable: true
});

console.log(item.price);

Object.defineProperty(item, "price", {
    writable: false,
    configurable: false
});

// Попытка изменения свойства теперь выдаст ошибку в строгом режиме
item.price = 200;
console.log(item.price);


///////////////// Задание 7 ///////////////
console.log('\n==== Задание 7 ====\n');

let circle = {
    _radius: 5,
    get radius() {
        return this._radius;
    },
    set radius(value) {
        this._radius = value;
    },
    get area() {
        return Math.PI * this._radius ** 2;
    }
};

console.log(circle.area);
circle.radius = 10;
console.log(circle.area);


///////////////// Задание 8 ///////////////
console.log('\n==== Задание 8 ====\n');

{
    let car = {};
    Object.defineProperties(car, {
        make: { value: "Honda", writable: true, configurable: true },
        model: { value: "Civic", writable: true, configurable: true },
        year: { value: 2020, writable: true, configurable: true }
    });

    Object.defineProperties(car, {
        make: { writable: false, configurable: false },
        model: { writable: false, configurable: false },
        year: { writable: false, configurable: false }
    });

    console.log(car);
}

///////////////// Задание 9 ///////////////
console.log('\n==== Задание 9 ====\n');

let numbers = [1, 2, 3];
Object.defineProperty(numbers, "sum", {
    get() {
        return this.reduce((acc, num) => acc + num, 0);
    },
    configurable: false,
    enumerable: false
});

console.log(numbers.sum);


///////////////// Задание 10 ///////////////
console.log('\n==== Задание 10 ====\n');

let rectangle = {
    _width: 5,
    _height: 10,
    get width() {
        return this._width;
    },
    set width(value) {
        this._width = value;
    },
    get height() {
        return this._height;
    },
    set height(value) {
        this._height = value;
    },
    get area() {
        return this._width * this._height;
    }
};

console.log(rectangle.area);
rectangle.width = 7;
console.log(rectangle.area);

///////////////// Задание 11 ///////////////
console.log('\n==== Задание 10 ====\n');

let user = {
    firstName: "John",
    lastName: "Doe",
    get fullName() {
        return `${this.firstName} ${this.lastName}`;
    },
    set fullName(value) {
        [this.firstName, this.lastName] = value.split(" ");
    }
};

console.log(user.fullName);
user.fullName = "Jane Smith";
console.log(user.fullName); 