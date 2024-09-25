//1

function flattenArray(arr) {
    return arr.reduce((acc, val) => {
        return acc.concat(Array.isArray(val) ? flattenArray(val) : val);
    }, []);
}

const array1 = [1, [1, 2, [3, 4]], [2, 4]];
const array2 = [5, [6, 7]];
const mergedArray = flattenArray(array1).concat(flattenArray(array2));

alert(mergedArray);

// //2

function findArraySum(arr) {
    let sum = 0;

    arr.forEach((element) => {
        if (Array.isArray(element)) {
            sum += findArraySum(element);
        } else if (typeof element === "number") {
            sum += element;
        }
    });

    return sum;
}

const nestedArray = [1, [2, [3, 4], [5, 6]], 7];
const sum = findArraySum(nestedArray);
alert(sum);

// //3
function filterStudentsByAge(students) {
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

const students = [
    { name: 'Студент 1', age: 18, groupId: 4 },
    { name: 'Студент 2', age: 16, groupId: 2 },
    { name: 'Студент 3', age: 19, groupId: 4 }
];

const filteredStudents = filterStudentsByAge(students);
console.log(filteredStudents);

//4
function stringToCode(string) {
    let result = "";
    for (let i = 0; i < string.length; i++)
        result += string.charCodeAt(i);
    return result;
} const total1 = stringToCode("ABC");
const total2 = total1.replace('7', '1');
console.log(`total1 - total2 = ${total1 - total2}`);

//5
function extend(...objects) {
    const result = {};

    for (const obj of objects) {
        for (const key in obj) {
            if (obj.hasOwnProperty(key) && !result.hasOwnProperty(key)) {
                result[key] = obj[key];
            }
        }
    }

    return result;
}

const result1 = extend({ a: 1, b: 2 }, { c: 3 });
console.log(result1); // {a: 1, b: 2, c: 3}

const result2 = extend({ a: 1, b: 2 }, { c: 3 }, { d: 4 });
console.log(result2); // {a: 1, b: 2, c: 3, d: 4}

const result3 = extend({ a: 1, b: 2 }, { a: 3, c: 3 });
console.log(result3); // {a: 1, b: 2, c: 3}

//6
function createTower(numFloors) {
    const tower = [];

    for (let i = 1; i <= numFloors; i++) {
        const spaces = ' '.repeat(numFloors - i);
        const stars = '*'.repeat(2 * i - 1);
        const floor = spaces + stars + spaces;
        tower.push(floor);
    }

    return tower;
}


const tower3 = createTower(3);
tower3.forEach(floor => console.log(floor));