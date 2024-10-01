function flatArray(arr) {
    let resultArray = [];

    arr.reduce((previousValue, item, index, array) => {
        if (Array.isArray(item)) {
            resultArray = resultArray.concat(flatArray(item));
        }
        else {
            resultArray.push(item);
        }
    }, [])

    return resultArray;
}

// Задание 1

function join2Arrays(arr1, arr2) {
    return flatArray(arr1).concat(flatArray(arr2));
}

let arr1 = [2, 5, [6, 10], [3, 4]];
let arr2 = [1, 4, [7, 9], [6, 1]];

console.log(join2Arrays(arr1, arr2));

// Задание 2

function findSumOfJaggedArray(arr) {
    return flatArray(arr).reduce((sum, item) => sum + item);
}

let jaggedArray = [[1, 2, 3], [4, 5], [6, 7, 8, 9]];
console.log(findSumOfJaggedArray(jaggedArray));

// Задание 3

function groupStudents(students) {
    const result = {};

    for (const student of students) {
        if (student.age > 17) {
            if (!result[student.groupId]) {
                result[student.groupId] = [];
            }
            result[student.groupId].push(student);
        }
    }

    return result;
}

let students = [
    { name: 'Валерий', age: 18, groupId: 1 },
    { name: 'Акакий', age: 16, groupId: 2 },
    { name: 'Сергей', age: 19, groupId: 1 }
]

console.log(groupStudents(students));

// Задание 4

const asciiString = "ABC";

let result1 = "";

for (let i = 0; i < asciiString.length; i++) {
    result1 += asciiString.charCodeAt(i).toString();
}

const result2 = result1.replace('7', '1')

console.log(result1)
console.log(result2);

// Задание 5

function extend(params) {
    return Object.assign({}, ...params);
}

const result11 = extend([{ a: 1, b: 2 }, { c: 3 }]);
console.log(result11);

const result22 = extend([{ a: 1, b: 2 }, { c: 3 }, { d: 4 }]);
console.log(result22);

const result3 = extend([{ a: 1, b: 2 }, { a: 3, c: 3 }]);
console.log(result3);

// Задание 6

function printCenteredStarPyramid(count) {
    for (let i = 1; i <= count; i++) {
        row = "*".repeat(2 * i - 1);
        row = row.padStart(count + i - 1, " ");
        console.log(row);
    }
}

printCenteredStarPyramid(5);