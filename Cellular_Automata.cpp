#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <bitset>
#include <stdexcept>

// Global variables
std::unordered_map<std::string, char> wolfram_ruleset;
std::map<int, std::string> value_of_rule_columns;

// Function prototypes
void Next_Cell_Values();
void Create_Ruleset(int rule_number);
void Generative_Algorithm(std::vector<char>& current_array, std::vector<char>& next_array, int displayWidth);
void Initialize_Initial_Array(std::vector<char>& initial_array);
void DisplayCenteredRow(const std::vector<char>& row, int displayWidth);

extern "C" {
    void run_automaton(int size, int rule_number, int iterations, int displayWidth);
}

// Main function to run automaton
void run_automaton(int size, int rule_number, int iterations, int displayWidth) {
    try {
        std::cout << "Running automaton with size = " << size
                  << ", rule = " << rule_number
                  << ", iterations = " << iterations << std::endl;

        // Initialize arrays
        std::vector<char> current_array(size, '0');
        std::vector<char> next_array(size, '0');

        Initialize_Initial_Array(current_array);
        Next_Cell_Values();
        Create_Ruleset(rule_number);

        // Display initial row
        DisplayCenteredRow(current_array, displayWidth);

        // Generate iterations
        for (int i = 0; i < iterations; ++i) {
            Generative_Algorithm(current_array, next_array, displayWidth);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Function to initialize the initial array with a '1' at the center
void Initialize_Initial_Array(std::vector<char>& initial_array) {
    if (initial_array.size() < 20) {
        throw std::runtime_error("Array size must be at least 20 for meaningful patterns.");
    }
    initial_array[initial_array.size() / 2] = '1';
}

// Function to generate all 256 binary rule patterns
void Next_Cell_Values() {
    for (int i = 0; i < 256; ++i) {
        value_of_rule_columns[i] = std::bitset<8>(i).to_string();
    }
}

// Function to create the Wolfram ruleset from the given rule number
void Create_Ruleset(int rule_number) {
    if (rule_number < 0 || rule_number > 255) {
        throw std::runtime_error("Rule number must be between 0 and 255.");
    }

    std::string rule_pattern = value_of_rule_columns[rule_number];
    std::vector<std::string> neighborhoods = {"000", "001", "010", "011", "100", "101", "110", "111"};

    for (size_t i = 0; i < neighborhoods.size(); ++i) {
        wolfram_ruleset[neighborhoods[i]] = rule_pattern[7 - i]; // Reverse rule pattern for correct mapping
    }
}

// Function to generate the next generation of the cellular automaton
void Generative_Algorithm(std::vector<char>& current_array, std::vector<char>& next_array, int displayWidth) {
    for (size_t i = 0; i < current_array.size(); ++i) {
        // Determine neighbors using circular boundary conditions
        char left = (i == 0) ? current_array.back() : current_array[i - 1];
        char center = current_array[i];
        char right = (i == current_array.size() - 1) ? current_array.front() : current_array[i + 1];

        std::string neighborhood = {left, center, right};

        // Get the next state from the Wolfram ruleset
        next_array[i] = wolfram_ruleset[neighborhood];
    }

    // Display the next row
    DisplayCenteredRow(next_array, displayWidth);

    // Update current array
    current_array = next_array;
}

// Function to display a row centered in the output
void DisplayCenteredRow(const std::vector<char>& row, int displayWidth) {
    int row_width = row.size() * 2; // Each cell takes up 2 spaces
    int padding = (displayWidth - row_width) / 2;

    if (padding < 0) padding = 0; // Prevent negative padding

    // Print spaces for padding
    std::cout << std::string(padding, ' ');

    // Print the row
    for (char cell : row) {
        std::cout << (cell == '1' ? "■ " : "  ");
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Define your configurations and run the simulation!" << std::endl;
    return 0;
}
