///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

class Task {
    constructor(id, name, status) {
        this.id = id;
        this.name = name;
        this.status = status;
    }
    set name(value) {
        this.value = value;
    }
    get name() {
        return this.name;
    }

    set status(value) {
        this.status = value;
    }
    get status() {
        return this.status;
    }
}

///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n');
let todoid = 0;
let taskid = 0;

class Todolsit {
    constructor(id) {
        this.tasks = [];
        this.id = todoid++;
    }
}

///////////////// Задание 3 ///////////////
console.log('\n==== Задание 3 ====\n');



///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n');



