#include <iostream>

#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    std::string version = "0.0.1";
    std::string lunarVersion = "2025.0";
    // std::cout << "DEBUG: " << argc << std::endl;
    if (argc < 2) {
        std::cerr << "ERROR: Not enough argument. Pass '--help' to show the usage" << std::endl;
        return 20;
    }
    if (argc > 2) {
        std::cerr << "ERROR: Too many arguments. Pass '--help' to show the usage" << std::endl;
        return 21;
    }

    std::string arg1 = argv[1];

    if (arg1 == "--help") {
        std::cout << "MoonMiner version " << version << std::endl;
        std::cout << "The compiler of Lunar programming language" << std::endl;
        std::cout << std::endl;
        std::cout << "mminer [FILENAME]" << std::endl;
        std::cout << std::endl;
        std::cout << "  --help: Show this message and exit" << std::endl;
        std::cout << "  --version: Show the version of MoonMiner" << std::endl;
         return 0;
    }
    if (arg1 == "--version") {
        std::cout << "MoonMiner version " << version << "with Lunar version " << lunarVersion << " standards" << std::endl;
        std::cout << "Lunar, A 'beagleful' programming language" << std::endl;
        std::cout << "Developed by BeagleSoftware, licensed with MIT license" << std::endl;
        return 0;
    }

    // std::cout << arg1 << std::endl;
    std::fstream input(argv[1], std::ios::in);
    return EXIT_SUCCESS3292                                                         ;
}
