let user = {
    name: 'Masha',
    age: 21
};

let numbers = [1, 2, 3];

let user1 = {
    name:
        'Masha',
    age: 23,
    location: {
        city: 'Minsk',
        country: 'Belarus'
    }
};

let user2 = {
    name: 'Masha',
    age: 28,
    skills: ["HTML", "CSS", "JavaScript", "React"]

};

const array = [
    { id: 1, name: 'Vasya', group: 10 },
    { id: 2, name: 'Ivan', group: 11 },
    { id: 3, name: 'Masha', group: 12 },
    { id: 4, name: 'Petya', group: 10 },
    { id: 5, name: 'Kira', group: 11 },
];

let user4 = {
    name: 'Masha',
    age: 19,
    studies: {
        university: 'BSTU',
        speciality: 'designer',
        year: 2020,
        exams: {
            maths: true,
            programming: false
        }
    }
};

let user5 = {
    name: 'Masha',
    age: 22,
    studies: {
        university: 'BSTU',
        speciality: 'designer',
        year: 2020,
        department: {
            faculty: 'FIT',
            group: 10,
        },
        exams: [
            { maths: true, mark: 8 },
            { programming: true, mark: 4 },
        ]
    }
};

let user6 = {
    name: 'Masha',
    age: 21,
    studies: {
        university: 'BSTU',
        speciality: 'designer',
        year: 2020,
        department: {
            faculty: 'FIT',
            group: 10,
        },
        exams: [
            {
                maths: true,
                mark: 8,
                professor: {
                    name: 'Ivan Ivanov',
                    degree: 'PhD'
                }
            },
            {
                programming: true,
                mark: 10,
                professor: {
                    name: 'Petr Petrov',
                    degree: 'PhD'
                }
            }
        ]
    }
};

let user7 = {
    name: 'Masha',
    age: 20,
    studies: {
        university: 'BSTU',
        speciality: 'designer',
        year: 2020,
        department: {
            faculty: 'FIT',
            group: 10,
        },
        exams: [
            {
                maths: true,
                mark: 8,
                professor: {
                    name: 'Ivan Petrov',
                    degree: 'PhD',
                    articles: [
                        { title: "About HTML", pagesNumber: 3 },
                        { title: "About CSS", pagesNumber: 5 },
                        { title: "About JavaScript", pagesNumber: 1 },
                    ]
                }
            },
            {
                programming: true,
                mark: 10,
                professor: {
                    name: 'Petr Ivanov',
                    degree: 'PhD',
                    articles: [
                        { title: "About HTML", pagesNumber: 3 },
                        { title: "About CSS", pagesNumber: 5 },
                        { title: "About JavaScript", pagesNumber: 1 },
                    ]
                }
            },
        ]
    }
};

let store = {
    state: { // 1 уровень
        profilePage: { // 2 уровень
            posts: [ // 3 уровень
                { id: 1, message: 'hi', likesCount: 12 },
                { id: 2, message: 'by', likesCount: 1 }
            ],
            newPostText: 'about me'
        },
        dialogsPage: {
            dialogs: [
                { id: 1, name: 'valera' },
                { id: 2, name: 'andrey' },
                { id: 3, name: 'sasha' },
                { id: 4, name: 'viktor' }
            ],
            messages: [
                { id: 1, message: 'hi' },
                { id: 2, message: 'hi hi' },
                { id: 3, message: 'hi hi hi' }
            ]
        },
        sidebar: []
    }
}

///////////////// Задание 1 ///////////////
console.log('\n==== Задание 1 ====\n');

let userCpy = { ...user };

let numbersCpy = [...numbers];

let user1Cpy = { ...user1 };

let user2Cpy = { ...user2, skills: [...user2.skills] };

let arrayCpy = array.map((item) => ({ ...item }));

let user4Cpy = {
    ...user4,
    studies: {
        ...user4.studies,
        exams: { ...user4.studies.exams }
    }
};

let user5Cpy = {
    ...user5,
    studies: {
        ...user5.studies,
        department: { ...user5.studies.department },
        exams: user5.studies.exams.map(exam => ({ ...exam }))
    }
};
let user6Cpy = {
    ...user6,
    studies: {
        ...user6.studies,
        department: { ...user6.studies.department },
        exams: user6.studies.exams.map(exam => ({
            ...exam,
            professor: { ...exam.professor }
        }))
    }
};
let user7Cpy = {
    ...user7,
    studies: {
        ...user7.studies,
        department: { ...user7.studies.department },
        exams: user7.studies.exams.map(exam => ({
            ...exam,
            professor: {
                ...exam.professor,
                articles: exam.professor.articles.map(article => ({ ...article }))
            }
        }))
    }
};

let storeCpy = {
    ...store,
    state: {
        ...store.state,
        profilePage: {
            ...store.state.profilePage,
            posts: store.state.profilePage.posts.map(post => ({ ...post }))
        },
        dialogsPage: {
            ...store.state.dialogsPage,
            dialogs: store.state.dialogsPage.dialogs.map(dialog => ({ ...dialog })),
            messages: store.state.dialogsPage.messages.map(message => ({ ...message }))
        }
    }
};

///////////////// Задание 2 ///////////////
console.log('\n==== Задание 2 ====\n');

user5Cpy.studies.department.group = 12;
user5Cpy.studies.exams[1].mark = 10;

console.log(user5);
console.log(user5Cpy);

///////////////// Задание 3 ///////////////
console.log('\n==== Задание 3 ====\n');

user6Cpy.studies.exams[0].professor.name = 'Sergey Ivanov';
console.log(user6Cpy);

///////////////// Задание 4 ///////////////
console.log('\n==== Задание 4 ====\n');

user7Cpy.studies.exams[1].professor.articles[1].pagesNumber = 3;

// или можно так

user7Cpy.studies.exams
    .find(exam => exam.programming)
    .professor.articles
    .find(article => article.title === "About CSS")
    .pagesNumber = 3;


///////////////// Задание 5 ///////////////
console.log('\n==== Задание 5 ====\n');

storeCpy.state.profilePage.posts = storeCpy.state.profilePage.posts.map(post => ({
    ...post,
    message: 'Hello'
}));

storeCpy.state.dialogsPage.messages = storeCpy.state.dialogsPage.messages.map(message => ({
    ...message,
    message: 'Hello'
}));

console.log(storeCpy);
