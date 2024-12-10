#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <bitset>
#include <array>

// Global variables
std::unordered_map<std::string, char> wolfram_ruleset;
std::map<int, std::string> value_of_rule_columns;

// Function prototypes
void Define_Next_Cell_Values();
void Create_Ruleset(int rule_number);
void Generative_Algorithm(std::vector<char>& current_array, std::vector<char>& next_array);
void Initialize_Initial_Array(std::vector<char> &initial_array);
void DisplayCenteredRow(const std::vector<char>& row);

int main() {

    int size;
    int rule_number;
    std::cout<<"Please write the size of the grid to display"<<std::endl;
    std::cin>>size;

    std::cout<<"Please write the rule you want to display"<<std::endl;
    std::cin>>rule_number;


    std::vector<char> current_array(size) ;
    std::fill(current_array.begin(),current_array.end(),'0');

    std::vector<char> next_array(size);
    std::fill(next_array.begin(),next_array.end(),'0');

    Initialize_Initial_Array(current_array);

    // Create rule permutations and initialize rule 1 (example)
    Define_Next_Cell_Values();
    Create_Ruleset(rule_number);  // Example: Rule 30

    // Generate and display 10 generations
    for (int i = 0; i < 100; ++i) {
        Generative_Algorithm(current_array, next_array);
    }

    return 0;
}

void Initialize_Initial_Array(std::vector<char> &initial_array){
    //we need to put a 1 at the middle
    int size_of_array=initial_array.size();

    if(size_of_array<20){
        std::cout<<"Please make sure array size is atleast greater than 20 for any meaningful pattern to be seen"<<std::endl;
        exit(1); // Exit the program
    }

    int mid_portion=size_of_array/2;
    initial_array[mid_portion]='1';
}



// Create all 256 binary rule patterns
void Define_Next_Cell_Values() {
    for (int i = 0; i < 256; i++) {
        std::string binary = std::bitset<8>(i).to_string();
        value_of_rule_columns[i] = binary;
    }
}

// Map rule_number to the Wolfram ruleset
void Create_Ruleset(int rule_number) {
    if (rule_number < 0 || rule_number > 255) {
        std::cerr << "Error: Rule number must be between 0 and 255." << std::endl;
        exit(1);
    }

    // Retrieve the binary pattern for the rule_number
    std::string rule_pattern = value_of_rule_columns[rule_number];

    // Map binary pattern to the Wolfram ruleset
    std::vector<std::string> neighborhoods = {"000", "001", "010", "011", "100", "101", "110", "111"};
    for (size_t i = 0; i < neighborhoods.size(); ++i) {
        wolfram_ruleset[neighborhoods[i]] = rule_pattern[7 - i]; // Reverse the rule pattern for correct mapping
    }
}

// Generate the next generation of the cellular automaton
void Generative_Algorithm(std::vector<char>& current_array, std::vector<char>& next_array) {
    // Display the current row (centered)
    // std::cout << "Current row: "; // Commented out
    DisplayCenteredRow(current_array);

    // Compute the next row
    for (size_t i = 0; i < current_array.size(); ++i) {
        std::string value;

        // Determine neighbors using circular boundary conditions
        char left = (i == 0) ? current_array[current_array.size() - 1] : current_array[i - 1];
        char center = current_array[i];
        char right = (i == current_array.size() - 1) ? current_array[0] : current_array[i + 1];

        // Create the neighborhood string
        value.push_back(left);
        value.push_back(center);
        value.push_back(right);

        // Get the next state from the Wolfram ruleset
        next_array[i] = wolfram_ruleset[value];
    }

    // Display the next row (centered)
    // std::cout << "Next row:    "; // Commented out
    DisplayCenteredRow(next_array);

    // Update the current array
    current_array = next_array;
}


void DisplayCenteredRow(const std::vector<char>& row) {
    int console_width = 80; // Adjust based on your terminal width
    int row_width = row.size() * 2; // Each cell is represented by 2 characters

    // Calculate padding to center the row
    int padding = (console_width - row_width) / 2;
    if (padding < 0) padding = 0; // Prevent negative padding

    // Print spaces for padding
    std::cout << std::string(padding, ' ');

    // Print the row
    for (char cell : row) {
        if (cell == '1') {
            std::cout << "# "; // Simpler representation for active cells
        } else {
            std::cout << "  "; // Inactive cells remain empty
        }
    }
    std::cout << std::endl;
}
