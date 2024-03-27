#include <iostream>
// these are for writing to the screen.
#include <iomanip>
#include <sstream>
#include <cstdint>
// these are for file management.
#include <filesystem>
#include <fstream>
// this is just for sin, cos, round, sqrt, and pow.
#include <cmath>
// these are for storage of important files
#include <stack>
#include <vector>
// this is for running programs without freezing the system
#include <future>
// this is for when you don't know the type that will be given
#include <type_traits>
// namespaces
using namespace std;
uint32_t* screenData = reinterpret_cast<uint32_t*>(0xA0000); // you're screwed if the framebuffer isn't at 0xA0000, because the system might crash if it writes to something important
uint32_t convertedColor;
std::string root;
std::deque<std::deque<std::string>> files;
std::deque<std::string> terminal;
int mouseX = 0;
int mouseY = 0;
// all drivers
// keyboard driver
std::deque<char> getKeysPressed() {
    // in order to prevent warnings, this returns a blank deque for now.
    std::deque<char> returned;
    return returned;
}
// mouse driver
// all things for filesystems
std::deque<std::string> getFileContents(std::string fpath) {
    std::ifstream file(fpath);
    std::deque<std::string> text;
    if(!file.is_open()) {
        logToConsole("couldn't get file: " + fpath + "\n");
    }
    std::string line;
    while(std::getline(file, line)) {
        text.push_back(line);
    }
    file.close();
    return text;
}
void addFileToFiles(std::string fpath) {
    std::deque<std::string> file = getFileContents(fpath);
    files.push_back(file);
}
std::stack<std::string> getFilesAndFoldersIn(std::__fs::filesystem::path path) {
    std::stack<std::string> toReturn;
    for(const auto& entry : std::__fs::filesystem::directory_iterator(path)) {
        if(entry.is_regular_file()) {
            toReturn.push("0");
            toReturn.push(entry.path().filename().string());
        } else if(entry.is_directory()) {
            toReturn.push("1");
            toReturn.push(entry.path().filename().string());
        }
    }
    return toReturn; 
}
void replaceFileContents(std::string fpath, std::deque<std::string> fcontents) {
    std::ofstream file(fpath);
    for(int i = 0; i < fcontents.size(); ++i) {
        file << fcontents[i] << "\n";
    }
}
void createFile(std::string fpath, std::deque<std::string> fcontents) {
    // redirects to replaceFileContents to avoid confusion
    replaceFileContents(fpath, fcontents);
}
// function to get things off of stacks, made by chatgpt
template<typename T, typename U>
void getStackElement(int item, T stack, U& output) {
    auto otherStack = stack;
    try {
        for (int i = 0; i < item; ++i) {
            if (otherStack.empty()) {
                logToConsole("not enough elements in stack\n");
                return;
            }
            otherStack.pop();
        }
    } catch (const std::exception& e) {
        logToConsole("unknown error: " << e.what() << "\n");
    }
    if (!otherStack.empty()) {
        output = otherStack.top();
    } else {
        logToConsole("no elements in stack\n");
    }
}
// all the graphics functions that are used by the operating system. (will be deprecated soon, use gpu rendering functions if possible)
void createRect(int x, int y, int width, int height, int screenWidth, int screenHeight) {
    // this function is from chatgpt because i wanted it to be as optimized as possible
    int maxX = std::min(x + width, screenWidth);
    int maxY = std::min(y + height, screenHeight);
    uint32_t* start = screenData + y * screenWidth + x;
    for(int row = 0; row < maxY - y; ++row) {
        std::fill(start + row * screenWidth, start + row * screenWidth + maxX - x, convertedColor);
    }
}
void changeColor(int color) {
    // this entire function is done by chatgpt.
    int red = (color >> 16) & 0xFF;
    int green = (color >> 8) & 0xFF;
    int blue = color & 0xFF;
    convertedColor = (static_cast<uint32_t>(red) << 16) | (static_cast<uint32_t>(green) << 8) | static_cast<uint32_t>(blue);
}
// log to terminal deque
void logToConsole(std::string log) {
    terminal.push_back(log);
}
void startup() {
    logToConsole("currently on pre-alpha v0.00 (this version compiled on 3/22/2024)"); // please update the date and version before compiling
    // file loading
    // get the root of the disk
    root = std::__fs::filesystem::current_path().root_directory().string();
    // add the files to the files deque
    addFileToFiles(root + "/os/usrs.txt");
}
void createWindow(int x, int y, int width, int size, int title, std::deque<std::deque<std::string>> graphics) {

}
int main() {
    startup();
    return 0;
}