#include <iostream>
#include <string>
using namespace std;

double processFine(double amount, string location) {
	double fine = 0.000;
	if (amount <= 60) /* if statement that gives fine when amount is less than 60M*/
		fine = (amount * 0.1111);
	else {
		amount -= 60; /* subtracts first 60M from total amount */
		fine += (60 * 0.1111); /* adds the fine for first 60M */
		double decimal;
		if (location == "florida" || location == "new york") /* if-else that determines which percent to multiply for the next 90M */
			decimal = 0.18;
		else
			decimal = 0.153;
		if (amount <= 90) /* if the remaining amount is less than or equal to 90, then just add that to the fine, no need to continue */
			fine += (amount * decimal);
		else {
			amount -= 90; /* get remaining amount after first 150M */
			fine += (90 * decimal); /* adds fine for next 90M */
			fine += (amount * 0.21); /* adds fine for remaining amount */
		}
	}
	return fine;
}

int main() {
	string ident;
	double amount;
	string location;
	cout << "Identification: "; 
	getline(cin, ident); /* asks for string input for identification */
	cout << "Overstated amount (in millions): "; 
	cin >> amount; /* ask for an input double for overstated amount */
	cin.ignore(10000, '\n'); /* makes it so it doesn't count the extra endl at the end of cin as an input */
	cout << "Location: ";
	getline(cin, location); /* ask for a string input for location */
	cout << "---" << endl;
	if (ident == "") { /* if the identification is an empty string, print the following error message */
		cout << "You must enter a property identification." << endl;
		return -1; /* ends the program */
	}
	else if (amount <= 0) { /* if the amount is negative or 0, print out the following error message */
		cout << "The overstated amount must be positive." << endl;
		return -1; /* ends the program */
	}
	else if (location == "") { /* if the location is an empty string, print the following error message */
		cout << "You must enter a location." << endl;
		return -1; /* ends the program */
	}
	else {
		double fine = processFine(amount, location); /* calls function to calculate the fine amount */
		cout.setf(ios::fixed);
		cout.precision(3); /* sets the number of decimal places to 3 */
		cout << "The fine for " << ident << " is $" << fine << " million." << endl; /* print out the final statement if all inputs are valid */
	}
	return 0; /* ends the program */
}