

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
    console.log(typeof result);
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
    { name: 'John', age: 18, groupId: 1 },
    { name: 'Alice', age: 16, groupId: 2 },
    { name: 'Bob', age: 19, groupId: 1 }]

console.log(groupStudents(students));