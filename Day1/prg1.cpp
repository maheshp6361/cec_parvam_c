/*
 * REAL WORLD SCENARIO: The Car Factory
 * Imagine you are building a system for a car factory (like Toyota).
 * Instead of writing separate functions to handle speed, color, and braking scattered everywhere,
 * we create a "Blueprint" called a Class. This blueprint bundles everything a car IS (data)
 * and everything a car CAN DO (behavior) into one single, organized package.
 */


// #include is a preprocessor directive. It tells the compiler to grab a standard library file before compiling.
// <iostream> stands for Input/Output Stream. We use it because it contains the code needed to print text to the screen (cout).
#include <iostream>


// <string> allows us to use text variables (like words or sentences), rather than just single characters.
#include <string>


// 'std' is the standard namespace. It tells the compiler we want to use the standard C++ library.
// If we didn't write this, we would have to write 'std::cout' and 'std::string' every single time!
using namespace std;


// =====================================================================
// 1. THE BLUEPRINT (CLASS)
// =====================================================================
// 'class' is the keyword used to create our blueprint. 'Car' is the name we chose.
// Notice the capital 'C' in Car. It's best practice to capitalize class names.
class Car {


// 'public:' means everything below this line can be accessed and used from OUTSIDE the class (like in our main function).
// If it were 'private:', only the car itself could change these values.
public:


    // ---------------------------------------------
    // A. ATTRIBUTES (DATA) - What the car "IS"
    // ---------------------------------------------
    // 'string' is the data type for text. 'brand' holds the manufacturer's name (e.g., "Toyota").
    string brand;
   
    // 'color' holds the visual appearance of the car.
    string color;
   
    // 'int' is an integer (whole number). 'speed' tracks how fast the car is currently moving.
    int speed;


    // ---------------------------------------------
    // B. METHODS (BEHAVIOR) - What the car "DOES"
    // ---------------------------------------------
    // 'void' means this function does an action but does NOT return a value back to whoever called it.
    // 'accelerate' is the action name. It takes one input (a whole number called 'amount').
    void accelerate(int amount) {
        // We increase the car's current speed by the 'amount' we passed in.
        // It's the exact same as writing: speed = speed + amount;
        speed += amount;
       
        // 'cout' prints text to the screen. '<<' passes data to cout.
        // We combine text and variables to show a dynamic message.
        // 'endl' moves the cursor to a new line (End Line).
        cout << color << " " << brand << " accelerated by " << amount << " km/h. Current speed: " << speed << " km/h." << endl;
    }


    // A method to simulate pressing the brake pedal. It takes no inputs because braking just stops the car.
    void brake() {
        // We force the speed back to 0.
        speed = 0;
       
        // Print a message proving the car has stopped.
        cout << color << " " << brand << " applied brakes. Car has stopped." << endl;
    }
}; // <-- CRITICAL: A class definition in C++ MUST end with a semicolon!


// =====================================================================
// 2. THE MAIN PROGRAM (EXECUTION STARTS HERE)
// =====================================================================
// Every C++ program must have exactly one 'main' function. The computer starts reading code from here.
// 'int' means this function will return an integer (usually 0) when it finishes successfully.
int main() {
   
    // ---------------------------------------------
    // C. CREATING OBJECTS (THE ACTUAL CARS)
    // ---------------------------------------------
    // We are telling the computer: "Use the 'Car' blueprint to build a new object, and name it 'car1'."
    // This process is called "Instantiation".
    Car car1;
   
    // Now we access the specific attributes of 'car1' using the "dot operator" (.)
    // We are setting the unique characteristics of this exact car.
    car1.brand = "Toyota";
    car1.color = "Red";
    car1.speed = 0; // It starts parked.


    // Let's build a completely separate second car from the exact same blueprint.
    Car car2;
   
    // Even though they use the same blueprint, car2 has its own completely separate memory.
    // Changing car2's color will NOT affect car1.
    car2.brand = "Honda";
    car2.color = "Blue";
    car2.speed = 50; // This car is already moving.


    // ---------------------------------------------
    // D. INTERACTING WITH THE OBJECTS
    // ---------------------------------------------
    cout << "--- Interacting with Car 1 ---" << endl;
   
    // We use the dot operator to call the 'accelerate' behavior on car1.
    // We pass '30' into the 'amount' variable. car1's speed goes from 0 to 30.
    car1.accelerate(30);
   
    // We call it again. car1's speed goes from 30 to 50.
    car1.accelerate(20);


    cout << "\n--- Interacting with Car 2 ---" << endl;
   
    // We call accelerate on car2. Since car2 started at 50, it now goes to 60.
    car2.accelerate(10);
   
    // We call the brake behavior. car2's speed becomes 0.
    car2.brake();


    // ---------------------------------------------
    // E. ENDING THE PROGRAM
    // ---------------------------------------------
    // Returning 0 tells the operating system "The program ran perfectly with no errors."
    return 0;
}



