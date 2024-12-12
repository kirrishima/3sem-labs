// Класс Task
class Task {
    constructor(id, name, isCompleted = false) {
        this.id = id;
        this.name = name;
        this.isCompleted = isCompleted;
    }

    // Метод для изменения названия задачи
    updateName(newName) {
        this.name = newName;
    }

    // Метод для изменения состояния задачи
    toggleStatus() {
        this.isCompleted = !this.isCompleted;
    }
}

// Класс Todolist
class Todolist {
    constructor(id, name) {
        this.id = id;
        this.name = name;
        this.tasks = []; // Список задач
    }

    // Метод для изменения названия списка дел
    updateName(newName) {
        this.name = newName;
    }

    // Метод для добавления новой задачи в список дел
    addTask(task) {
        if (task instanceof Task) {
            this.tasks.push(task);
        } else {
            console.error("Добавляемый элемент должен быть экземпляром класса Task");
        }
    }

    // Метод для фильтрации задач по состоянию
    filterTasksByStatus(isCompleted) {
        return this.tasks.filter(task => task.isCompleted === isCompleted);
    }
}

// Пример работы

// Создание задач
const task1 = new Task(1, "Купить продукты");
const task2 = new Task(2, "Сделать домашнее задание", true);
const task3 = new Task(3, "Позвонить другу");

// Создание списка дел
const todoList1 = new Todolist(1, "Домашние дела");

// Добавление задач в список дел
todoList1.addTask(task1);
todoList1.addTask(task2);
todoList1.addTask(task3);

// Демонстрация изменения задачи
task1.updateName("Купить продукты и напитки");
task3.toggleStatus();

// Фильтрация задач
const completedTasks = todoList1.filterTasksByStatus(true);
const notCompletedTasks = todoList1.filterTasksByStatus(false);

console.log("Завершенные задачи:", completedTasks);
console.log("Незавершенные задачи:", notCompletedTasks);

// Создание еще одного списка дел
const todoList2 = new Todolist(2, "Рабочие дела");
const task4 = new Task(4, "Написать отчет");
todoList2.addTask(task4);

console.log("Список дел 1:", todoList1);
console.log("Список дел 2:", todoList2);
