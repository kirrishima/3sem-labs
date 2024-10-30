// /////////////// Задание 1 ///////////////
console.log('\n ==== Задание 1 ==== \n')

let numbers = [1, 2, 3, 4, 5, 6, 7];

let [y] = numbers;
console.log(y);

// /////////////// Задание 2 ///////////////
console.log('\n ==== Задание 2 ==== \n')

let user = {
    name: "Oleg",
    age: 52
}

let admin = { admin: "powerfull", ...user };

console.log(admin);

// /////////////// Задание 3 ///////////////
console.log('\n ==== Задание 3 ==== \n')

let store = {
    state: {  //1 уровень
        profilePage: {  //2 уровень
            posts: [  //3 уровень
                { id: 1, massage: 'Hi', likesCount: 12 },
                { id: 2, message: 'By', likesCount: 1 },
            ],
            newPostText: 'About me',
        },
        dialogsPage: {
            dialogs: [
                { id: 1, name: 'Valera' },
                { id: 2, name: 'Andrey' },
                { id: 3, name: 'Sasha' },
                { id: 4, name: 'Viktor' },
            ],
            messages: [
                { id: 1, message: 'hi' },
                { id: 2, message: 'hi hi' },
                { id: 3, message: 'hi hi hi' },
            ],
        },
        sidebar: [],
    },
}

let { state: { profilePage: { posts }, dialogsPage: { dialogs, messages } } } = store;
console.log("likesCount:");

posts.forEach(element => {
    console.log(element.likesCount);
});

console.log(messages);
console.log(dialogs.filter((dialog) => dialog.id % 2 == 0));

let newMessages = messages.map(function (m) {
    return m = { id: m.id, message: "Hello user" };
})

console.log(newMessages);

// /////////////// Задание 4 ///////////////
console.log('\n ==== Задание 4 ====\n')

let tasks = [
    { id: 1, title: "HTML&CSS", isDone: true },
    { id: 2, title: "JS", isDone: true },
    { id: 3, title: "ReactJS", isDone: false },
    { id: 4, title: "Rest API", isDone: false },
    { id: 5, title: "GraphQL", isDone: false },
];

let newTasks = [...tasks, { id: 6, title: "C++", isDone: false }];
console.log(newTasks);


// /////////////// Задание 5 ///////////////
console.log('\n ==== Задание 5 ====\n')

function sumValues(x, y, z) {
    return x + y + z;
}

let nums = [1, 2, 3];

console.log(sumValues(...nums));