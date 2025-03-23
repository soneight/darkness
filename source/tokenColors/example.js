// Function to add two numbers
function add(x, y) {
    return x + y;
}

// Object with properties and a method
const person = {
    firstName: 'John',
    lastName: 'Doe',
    fullName: function() {
        return this.firstName + ' ' + this.lastName;
    }
};

// Array of numbers
const numbers = [1, 2, 3, 4, 5];

// Loop through the array
for (let i = 0; i < numbers.length; i++) {
    console.log('Number: ' + numbers[i]);
}

// Conditional statement
if (add(2, 3) === 5) {
    console.log('2 + 3 equals 5');
}

// Arrow function
const multiply = (a, b) => a * b;

// Using the arrow function
console.log('3 * 4 equals ' + multiply(3, 4));

// Class with prototypes
function Car(make, model, year) {
    this.make = make;
    this.model = model;
    this.year = year;
}

Car.prototype.getCarInfo = function() {
    return this.year + ' ' + this.make + ' ' + this.model;
};

// Create an instance of the Car class
const myCar = new Car('Toyota', 'Corolla', 2020);

// Using the prototype method
console.log(myCar.getCarInfo());

// Class definition using the class keyword
class Animal {
    constructor(name, species) {
        this.name = name;
        this.species = species;
    }

    getAnimalInfo() {
        return this.name + ' is a ' + this.species;
    }
}

// Create an instance of the Animal class
const myPet = new Animal('Buddy', 'Dog');

// Using the class method
console.log(myPet.getAnimalInfo());
