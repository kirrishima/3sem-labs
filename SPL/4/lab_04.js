// Задание 1
console.log('Задание 1');

let goods = new Set(["apple", "orange", "banana", "kivy"]);

function addGood(good) {
    goods.add(good);
}

function addGoods(goodsArray) {
    for (let good of goodsArray) {
        addGood(good);
    }
}

function removeGoods(goodsArray) {
    for (let good of goodsArray) {
        goods.delete(good);
    }
}

function isGoodInStock(good) {
    return goods.has(good);
}

function goodsCount() {
    return goods.size;
}

addGood("BANANANANAN");
console.log(goods);

removeGoods(["apple", "BANANANANAN"]);
console.log(goods);

console.log(isGoodInStock("apple"));
console.log(isGoodInStock("kivy"));

console.log(goodsCount());

// Задание 2
console.log('Задание 2');

let students = new Set();

function addStudent(student) {
    students.add(student);
}

function removeStudent(index) {
    if (index < students.size) {
        let i = 0;
        for (let student of students) {
            if (i === index) {
                students.delete(student);
                break;
            }
            i++;
        }
    }
}

function findAll(groupID) {
    let newSet = new Set();

    for (const student of students) {
        if (student.groupID === groupID) {
            newSet.add(student);
        }
    }

    return newSet;
}

function sortStudents() {
    return new Set(Array.from(students).sort((a, b) => a.ID - b.ID));
}

addStudent({ ID: 1, name: "John", groupID: 1 });
addStudent({ ID: 2, name: "Alice", groupID: 2 });
addStudent({ ID: 3, name: "Bob", groupID: 1 });

console.log(...students);

removeStudent(1);

console.log(...students);

addStudent({ ID: 2, name: "Alice", groupID: 2 })

console.log(...students);

console.log(...sortStudents());

console.log(...findAll(1));

// Задание 3
console.log('Задание 3');

let cart = new Map();

function addItem(id, item) {
    cart.set(id, item);
}

function removeItem(id) {
    cart.delete(id);
}

function removeItemByName(name) {
    for (const key of cart.keys()) {
        if (cart.get(key).name === name) {
            cart.delete(key);
        }
    }
}

function changeQuantity(id, quantity) {
    cart.get(id).quantity = quantity;
}

function changePrice(id, price) {
    cart.get(id).price = price;
}

function printMap(map) {
    map.forEach((key, val) => console.log(key, val))
}

console.log("Корзина до добавления товаров:", cart);

// Добавление товаров
addItem(1, { name: 'яблоко', price: 1.5, quantity: 3 });
addItem(2, { name: 'банан', price: 0.8, quantity: 5 });
console.log("Корзина после добавления товаров:");
printMap(cart);

// Изменение количества
changeQuantity(1, 10);
console.log("Корзина после изменения количества яблок:");
printMap(cart);

// Изменение цены
changePrice(2, 20);
console.log("Корзина после изменения цены на бананы:");
printMap(cart);

// Удаление товара по ID
removeItem(1);
console.log("Корзина после удаления яблок:");
printMap(cart);

// Удаление товара по имени
removeItemByName('банан');
console.log("Корзина после удаления бананов:");
printMap(cart);

// Задание 4
console.log('Задание 4');

let cache = new WeakMap();

function calculateSum(obj) {
    if (cache.has(obj)) {
        console.log(`значение для ${obj} было взято из кеша`);
        return cache.get(obj);
    } else {
        cache.set(obj, obj.reduce((sum, current) => sum += current));
        return cache.get(obj);
    }
}

let obj = [1, 2, 3, 4, 5];
console.log(calculateSum(obj));
console.log(calculateSum(obj));
