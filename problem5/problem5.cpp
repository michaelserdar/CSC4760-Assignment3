#include <iostream> 
#include <iomanip>


int main() {

    double firstSum = 1.0;
    double secondSum = 0.0;

    //calculate the first sum 
    for (int i = 0; i < 100000; i++) {
        firstSum += 1e-18;
    }

    //calculate the second sum 
    for (int i = 0; i < 100000; i++) {
        secondSum += 1e-18;
    }
    //add one to the second sum after the loop 
    secondSum += 1.0;

    //calculate the difference between the two sums 
    double difference = secondSum - firstSum;


    //print the results
    std::cout << std::setprecision(20);
    std::cout << "First Sum from 1.0: " << firstSum << "\n";
    std::cout << "Second Sum from 0.0, with 1.0 added at the end: " << secondSum << "\n";
    std::cout << "Difference between the two sums: " << difference << "\n";

    std::cout << "__________________________________________\n";

    //Display the hex value for the sums 
    std::cout << "Hex value for the first sum: " << std::hexfloat << firstSum << "\n";
    std::cout << "Hex value for the second sum: " << std::hexfloat << secondSum << "\n";
    std::cout << "Hex value for the difference: " << std::hexfloat << difference << "\n";

    return 0;
}