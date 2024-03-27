// pretty sure this works. just in case it actually doesn't, i've left chatgpt's comments inside.
#include <fstream>
#include <string>

void bootloader() {
    // String to hold the bootloader code
    std::string code;

    // Open the file containing the bootloader code
    std::ifstream file("bootloader.bin");

    // Read the file contents into the string
    if (file.is_open()) {
        std::getline(file, code, '\0'); // Read until EOF
        file.close();
    } else {
        return;
    }

    // Inline assembly to set up the stack pointer
    asm("mov $0x7c00, %esp");

    // Execute the bootloader code as a string
    asm(
        "mov $0, %eax\n"
        "mov %0, %%esi\n"
        "mov $0x7e00, %%edi\n"
        "mov $512, %%ecx\n"
        "rep movsb\n"
        "jmp $0x7e00"
        : // No output operands
        : "r" (code.c_str()) // Input operand (the string)
        : "eax", "esi", "edi", "ecx" // Clobbered registers
    );
}

int main() {
    bootloader();
    return 0;
}