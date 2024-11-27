#include <Windows.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

typedef unsigned long long ull;

// Function to convert a number to binary representation
template<typename T>
vector<bool> toBinary(size_t size, T value) {
    vector<bool> bits(size, false);
    for (size_t i = 0; i < size; i++) {
        bits[size - i - 1] = static_cast<bool>(static_cast<ull>(value) & (1ull << i));
    }
    return bits;
}

// Function to convert binary representation to a decimal number
template<typename T>
T fromBinary(const vector<bool>& bits) {
    T result = 0;
    for (size_t i = 0; i < bits.size(); i++) {
        result = (result << 1) | bits[i];
    }
    return result;
}

// Function to print bits
void printBits(const vector<bool>& bits) {
    for (size_t i = 0; i < bits.size(); i++) {
        if (i % 8 == 0 && i != 0) cout << ' '; // Separate bytes for readability
        cout << bits[i];
    }
    cout << '\n';
}

// Main function to process command-line arguments
void processArguments(int argc, char* argv[]) {
    if (argc < 4) {
        throw invalid_argument("Insufficient arguments. Format:\n"
                               "<mode: tb | fb | td | fd > <signed|unsigned> <value>");
    }

    string mode = argv[1];
    string type = argv[2];
    string valueStr = argv[3];

    bool isSigned = (type == "signed") || (type == "s");
    long long value = stoll(valueStr); // Additional validation for unsigned will follow

    if (mode == "tb") {
        // Convert decimal to binary
        size_t size = 0;

        if (isSigned) {
            if (value >= -128 && value <= 127) size = 8;
            else if (value >= -32768 && value <= 32767) size = 16;
            else if (value >= -2147483648LL && value <= 2147483647LL) size = 32;
            else size = 64;
        } else {
            if (value < 0) size = 64;
            else if (value <= 255) size = 8;
            else if (value <= 65535) size = 16;
            else if (value <= 4294967295ULL) size = 32;
            else size = 64;
        }

        auto bits = toBinary(size, value);
        cout << "Binary representation, " << size << " bits:\n";
        printBits(bits);

    } else if (mode == "fb") {
        // Convert binary to decimal
        vector<bool> bits;
        for (char c : valueStr) {
            if (c == '1') bits.push_back(true);
            else if (c == '0') bits.push_back(false);
            else throw invalid_argument("Binary number can only contain '0' and '1'.");
        }

        if (isSigned) {
            long long result = fromBinary<long long>(bits);
            cout << "Decimal representation (signed): " << result << '\n';
        } else {
            ull result = fromBinary<ull>(bits);
            cout << "Decimal representation (unsigned): " << result << '\n';
        }

    } else {
        throw invalid_argument("Invalid mode. Use 'tb' or 'fb'.");
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(1251); // For proper console output
    if (argc == 1){
        cout << "Usage: <mode: tb | fb | td | fd > <signed|unsigned> <value>\n\n" 
        " tb - converts from decimal to binary\n"
        " fb - converts from binary to decimal\n";
        return 1;
    }
    try {
        processArguments(argc, argv);
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
