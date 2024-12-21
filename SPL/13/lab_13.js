
class Task {
    constructor(id, name, isCompleted = false) {
        this.id = id;
        this.name = name;
        this.isCompleted = isCompleted;
    }

    updateName(newName) {
        this.name = newName;
    }

    toggleStatus() {
        this.isCompleted = !this.isCompleted;
    }
}

class Todolist {
    constructor(id, name) {
        this.id = id;
        this.name = name;
        this.tasks = [];
    }

    addTask(task) {
        if (task instanceof Task) {
            this.tasks.push(task);
        } else {
            console.error("Добавляемый элемент должен быть экземпляром класса Task");
        }
    }

    deleteTask(taskId) {
        this.tasks = this.tasks.filter((task) => task.id !== taskId);
    }

    filterTasksByStatus(isCompleted) {
        return this.tasks.filter((task) => task.isCompleted === isCompleted);
    }

    getAllTasks() {
        return this.tasks;
    }
}

// Инициализация
const todoList = new Todolist(1, "Список дел");
const taskListElement = document.getElementById("taskList");
const taskNameInput = document.getElementById("taskNameInput");
const addTaskButton = document.getElementById("addTaskButton");
let taskIdCounter = 1;

function renderTasks(tasks) {
    taskListElement.innerHTML = "";

    tasks.forEach((task) => {
        const taskElement = document.createElement("li");
        taskElement.className = "task";

        const checkbox = document.createElement("input");
        checkbox.type = "checkbox";
        checkbox.checked = task.isCompleted;
        checkbox.addEventListener("change", () => {
            task.toggleStatus();
            renderTasks(todoList.getAllTasks());
        });

        const taskName = document.createElement("span");
        taskName.textContent = task.name;
        taskName.contentEditable = false;

        const editButton = document.createElement("button");
        editButton.textContent = "Редактировать";
        editButton.addEventListener("click", () => {
            if (editButton.textContent === "Редактировать") {
                taskName.contentEditable = true;
                taskName.focus();
                editButton.textContent = "Сохранить";
            } else {
                task.updateName(taskName.textContent);
                taskName.contentEditable = false;
                editButton.textContent = "Редактировать";
            }
        });

        const deleteButton = document.createElement("button");
        deleteButton.textContent = "Удалить";
        deleteButton.addEventListener("click", () => {
            todoList.deleteTask(task.id);
            renderTasks(todoList.getAllTasks());
        });

        taskElement.appendChild(checkbox);
        taskElement.appendChild(taskName);
        taskElement.appendChild(editButton);
        taskElement.appendChild(deleteButton);

        taskListElement.appendChild(taskElement);
    });
}
 
addTaskButton.addEventListener("click", () => {
    const taskName = taskNameInput.value.trim();
    if (taskName) {
        const newTask = new Task(taskIdCounter++, taskName);
        todoList.addTask(newTask);
        taskNameInput.value = "";
        renderTasks(todoList.getAllTasks());
    }
});

document.getElementById("showAll").addEventListener("click", () => {
    renderTasks(todoList.getAllTasks());
});

document.getElementById("showCompleted").addEventListener("click", () => {
    renderTasks(todoList.filterTasksByStatus(true));
});

document.getElementById("showNotCompleted").addEventListener("click", () => {
    renderTasks(todoList.filterTasksByStatus(false));
});

renderTasks(todoList.getAllTasks());