// Задание 1

let goods = new Set(["apple", "orange", "banana", "kivy"]);

function addGoods(goodsArray) {
    for (let good of goodsArray) {
        goods.add(good);
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

addGoods(["BANANANANAN"]);
console.log(goods);

removeGoods(["apple", "BANANANANAN"]);
console.log(goods);

console.log(isGoodInStock("apple"));
console.log(isGoodInStock("kivy"));

console.log(goodsCount());

// Задание 2

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

