/*
 * =====================================================================
 * ✅ SOLUTION FILE: ENCAPSULATION (Hiding Data)
 * =====================================================================
 * Trainer Note: This solution demonstrates Encapsulation. The main() 
 * function CANNOT directly change the battery. It must use the proper 
 * methods (playGame and chargePhone). This makes the code safe!
 */

// Grab the input/output library for printing
#include <iostream>
// Use the standard namespace
using namespace std;

// 1. Create the SmartPhone Blueprint
class SmartPhone {
// 'private:' ensures that no code outside of this class can touch this data!
// This is the shield of ENCAPSULATION.
private:
    // 'int' is a whole number used for battery percentage
    int batteryLevel;

// 'public:' means the main() function is allowed to use these methods
public:
    // A method to play a game
    // 'void' means it just does an action, it doesn't return data.
    void playGame() {
        // '-=' subtracts 15 from the current battery level
        batteryLevel -= 15;
        // 'cout' prints text. 'endl' drops to a new line.
        cout << "Playing game... Battery is now " << batteryLevel << "%" << endl;
    }

    // A method to charge the phone
    void chargePhone() {
        // Reset the battery safely
        batteryLevel = 100;
        cout << "Phone charged to 100%" << endl;
    }
}; // The crucial semicolon!

// Program execution starts here
int main() {
    cout << "--- ENCAPSULATION TEST ---" << endl;
    
    // 2. We instantiate (create) a physical object from the blueprint
    SmartPhone myiPhone;
    
    // 3. We use the Dot Operator (.) to securely interact with the phone.
    // Notice we CANNOT do: myiPhone.batteryLevel = 5000;
    // The compiler would throw an error because the battery is PRIVATE!
    
    myiPhone.chargePhone();
    myiPhone.playGame();
    myiPhone.playGame();

    // Exit successfully
    return 0;
}
