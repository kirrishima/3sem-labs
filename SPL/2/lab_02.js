// Задание 1

function basicOperation(operation, value1, value2) {
    switch (operation) {
        case '+':
            return +value1 + +value2;
        case '-':
            return +value1 - +value2;
        case '*':
            return +value1 * +value2;
        case '/':
            return +value1 / +value2;
        default:
            return undefined;
    }
}

let result = basicOperation('+', 5, 10);
console.log(result);

// Задание 2

function calcPower3Sum(n) {
    let sum = 0;
    for (let x = 1; x <= n; x++) {
        sum += Math.pow(x, 3);
    }
    return sum;
}

let result2 = calcPower3Sum(2);
console.log(result2);

// Задание 3

function calcAvg(arr) {
    let sum = 0;
    for (let i = 0; i < arr.length; i++) {
        sum += arr[i];
    }
    return arr.length ? sum / arr.length : 0;
}

let numbers = [1, 2, 3, 4, 5];
let average = calcAvg(numbers);
console.log(average);

// Задание 4

function reversLatinString(str) {
    let reversedString = "";

    for (let j = str.length - 1; j >= 0; j--) {
        let charCode = str[j].charCodeAt(0);

        if ((charCode >= 65 && charCode <= 90) || (charCode >= 97 && charCode <= 122)) {
            reversedString += str[j];
        }
    }
    return reversedString;
}

let inputString = "JavaScr53э? ipt";
let reversedString = reversLatinString(inputString);
console.log(reversedString);

// Задание 5

function repeateString(n, s) {

    let str = "";
    for (let j = 0; j < n; j++) {
        str += s;
    }
    return str;
}

let string1 = "a";
let result5 = repeateString(5, string1);
console.log(result5);

// Задание 6

function arr1ExcludeArr2(arr1, arr2) {

    let result = [];
    for (let i = 0; i < arr1.length; i++) {
        if (arr2.indexOf(arr1[i]) === -1) {
            result.push(arr1[i]);
        }
    }
    return result;
}

let arr1 = ['biba', 'boba', 'broba', 'bobr'];
let arr2 = ['goose', 'boba', '233', 'bobr'];
let result6 = arr1ExcludeArr2(arr1, arr2);
console.log(result6);