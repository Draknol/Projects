// Reuben Russell 23004666

// 16bit machine code emulator for 159.102


#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const uint16_t MEMORY_SIZE = 256;
const uint16_t BIT_MASK = 0x000F;
const uint16_t ONE_HEX_DIGIT = 4;
const uint16_t TWO_HEX_DIGIT = 8;
const uint16_t THREE_HEX_DIGIT = 12;

fstream program;
uint16_t memory[MEMORY_SIZE], reg[16], pc = 0, ir;

void read_program();
void run_ir();


int main()
{
    // Read the program into memory
    read_program();

    // Run the code until it halts
    while (pc < MEMORY_SIZE) run_ir();

    // Temp keep program going
    cin >> ir;
}

void run_ir()
{
    // Load Instruction Register
    ir = memory[pc];

    // Declare variables and store hex digits seperatly
    uint16_t digits[4], location;
    digits[0] = ir >> THREE_HEX_DIGIT;
    digits[1] = (ir >> TWO_HEX_DIGIT) & BIT_MASK;
    digits[2] = (ir >> ONE_HEX_DIGIT) & BIT_MASK;
    digits[3] = ir & BIT_MASK;
    
    // Print basic information
    cout << "\nPC: " << setw(2) << pc << "  IR: " << setw(4) << ir << "  ";

    // Check and compute each opperation
    switch (digits[0]) {
    case 1:  // 1  Load R with value
        reg[digits[1]] = (digits[2] << ONE_HEX_DIGIT) + digits[3];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 2:  // 2  Load R with S
        reg[digits[2]] = reg[digits[3]];
        cout << "Register R" << setw(1) << digits[2] << " = " << setw(4) << reg[digits[2]];
        pc++;
        break;
    case 3:  // 3  Load R with XY
        reg[digits[1]] = memory[(digits[2] << ONE_HEX_DIGIT) + digits[3]];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 4:  // 4  Store R to XY
        location = (digits[2] << ONE_HEX_DIGIT) + digits[3];
        memory[location] = reg[digits[1]];
        cout << "Memory[" << setw(2) << location << "] = " << setw(4) << memory[location];
        pc++;
        break;
    case 5:  // 5  Not needed
        pc++;
        break;
    case 6:  // 6  Load R with S + T (int)
        reg[digits[1]] = reg[digits[2]] + reg[digits[3]];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 7:  // 7  Negate R
        reg[digits[2]] = ~reg[digits[2]] + 1;
        cout << "Register R" << setw(1) << digits[2] << " = " << setw(4) << reg[digits[2]];
        pc++;
        break;
    case 8:  // 8  Shift R >> XY times
        reg[digits[1]] = reg[digits[1]] >> ((digits[2] << ONE_HEX_DIGIT) + digits[3]);
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 9:  // 9  Shift R << XY times
        reg[digits[1]] = reg[digits[1]] << ((digits[2] << ONE_HEX_DIGIT) + digits[3]);
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 10: // A  Load R with S & T
        reg[digits[1]] = reg[digits[2]] & reg[digits[3]];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 11: // B  Load R with S | T
        reg[digits[1]] = reg[digits[2]] | reg[digits[3]];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 12: // C  Load R with S ^ T
        reg[digits[1]] = reg[digits[2]] ^ reg[digits[3]];
        cout << "Register R" << setw(1) << digits[1] << " = " << setw(4) << reg[digits[1]];
        pc++;
        break;
    case 13: // D  Jump to XY if R = R0
        if (reg[digits[1]] == reg[0]) {
            location = (digits[2] << ONE_HEX_DIGIT) + digits[3];
            cout << "TRUE - jump to location " << setw(2) << location;
            pc = location;
        }
        else {
            cout << "FALSE - do not jump";
            pc++;
        }
        break;
    case 14: // E  Halt
        cout << "Program halts";
        pc = MEMORY_SIZE;
        break;
    default:
        pc++;
        break;
    }
}

void read_program()
{
    string filename;

    // Get file attempt to open it
    cout << "Enter the file name of the MASSEY machine code: ";
    cin >> filename;
    cout << "\n";
    program.open(filename.c_str());

    // Check it opened

    if (!program.is_open()){

        // Print error, stop anything from running and exit
        cout << "Error opening the file \"" << filename << "\"";
        pc = MEMORY_SIZE;
        return;
    }

    // Store and print program
    for (int i = 0; program >> hex >> memory[i]; i++)
    {
        cout << hex << uppercase << setfill('0') << "Memory[" << setw(2) << i << "] = " << setw(4) << memory[i] << endl;
    }
}