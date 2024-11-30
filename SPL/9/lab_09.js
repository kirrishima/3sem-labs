///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

let Figure = {
    color: "transparent",
    size: null
};

function Square(size, isBig = false) {
    this.__proto__ = Figure;
    this.size = size;
    this.big = isBig
};

function Circle(color, radius) {
    this.__proto__ = Figure;
    this.color = color;
    this.radius = radius;
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

printDiff(triangle1, Figure)
console.log('\n');

let circle1 = new Circle("none", 10);
let circle2 = new Circle("green", 10);

printDiff(circle1, circle2)
console.log('\n');

let square1 = new Square(10, true);
let square2 = new Square(2);

printDiff(square1, square2)


///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n');

class Human {
    constructor(firstName, lastName, birthYear, address) {
        this.firstName = firstName; // Имя
        this.lastName = lastName;   // Фамилия
        this.birthYear = birthYear; // Год рождения
        this.address = address;     // Адрес
    }

    // Геттер для возраста
    get age() {
        return 2024 - this.birthYear;
    }

    // Сеттер для возраста (изменяет год рождения)
    set age(newAge) {
        if (newAge > 0) {
            this.birthYear = 2024 - newAge;
        } else {
            console.error('Возраст должен быть положительным целым числом.');
        }
    }

    // Метод для изменения адреса
    setAddress(newAddress) {
        if (typeof newAddress === 'string' && newAddress.trim() !== '') {
            this.address = newAddress;
        } else {
            console.error('Адрес должен быть непустой строкой.');
        }
    }

    // Метод для отображения полной информации о человеке
    getInfo() {
        return `${this.firstName} ${this.lastName}, ${this.age} лет, адрес: ${this.address}`;
    }
}

class Student extends Human {
    constructor(firstName, lastName, birthYear, address, faculty, course, group, recordBookNumber) {
        super(firstName, lastName, birthYear, address);
        this.faculty = faculty; // Факультет
        this.course = course;   // Курс
        this.group = group;     // Группа
        this.recordBookNumber = recordBookNumber; // № зачетки
    }

    // Метод для изменения курса
    setCourse(newCourse) {
        if (Number.isInteger(newCourse) && newCourse > 0) {
            this.course = newCourse;
        } else {
            console.error('Курс должен быть положительным целым числом.');
        }
    }

    // Метод для изменения группы
    setGroup(newGroup) {
        if (typeof newGroup === 'string' && newGroup.trim() !== '') {
            this.group = newGroup;
        } else {
            console.error('Группа должна быть непустой строкой.');
        }
    }

    // Метод для получения полного имени
    getFullName() {
        return `${this.firstName} ${this.lastName}`;
    }

    // Метод для отображения информации о студенте
    getStudentInfo() {
        return `${this.getFullName()}, факультет: ${this.faculty}, курс: ${this.course}, группа: ${this.group}, 
        № зачетки: ${this.recordBookNumber}`;
    }
}

class Faculty {
    constructor(name, groupCount, studentCount) {
        this.name = name; // Название факультета
        this.groupCount = groupCount; // Количество групп
        this.studentCount = studentCount; // Количество студентов
        this.students = []; // Список студентов
    }

    // Метод для изменения количества групп
    setGroupCount(newGroupCount) {
        if (Number.isInteger(newGroupCount) && newGroupCount > 0) {
            this.groupCount = newGroupCount;
        } else {
            console.error('Количество групп должно быть положительным целым числом.');
        }
    }

    // Метод для изменения количества студентов
    setStudentCount(newStudentCount) {
        if (Number.isInteger(newStudentCount) && newStudentCount >= 0) {
            this.studentCount = newStudentCount;
        } else {
            console.error('Количество студентов должно быть неотрицательным целым числом.');
        }
    }

    // Метод для добавления студента
    addStudent(student) {
        if (student instanceof Student) {
            this.students.push(student);
            this.studentCount = this.students.length;
        } else {
            console.error('Можно добавлять только экземпляры класса Student.');
        }
    }

    // Метод для получения количества студентов специальности ДЭВИ
    getDev() {
        return this.students.filter(student => student.recordBookNumber[1] === '3').length;
    }

    // Метод для получения списка студентов заданной группы
    getGroup(groupName) {
        return this.students.filter(student => student.group === groupName);
    }
}

// Пример использования:
const student = new Student('Анна', 'Петрова', 2002, 'г. Новосибирск', 'ДЭВИ', 3, '6', '73201301');
const faculty = new Faculty('ФИТ', 10, 0);

faculty.addStudent(student);
faculty.addStudent(new Student('Иван', 'Сидоров', 2001, 'г. Москва', 'ФИТ', 4, '6', '71301302'));

console.log(faculty.getDev()); // Выводит количество студентов специальности ДЭВИ
console.log(faculty.getGroup('6')); // Выводит список студентов группы 'П-302'
