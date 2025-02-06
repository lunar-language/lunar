#include <iostream>

#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <cstring>

#include "./parser.hpp"
#include "./tokenization.hpp"
#include "./generation.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define OS "Windows"
#elif defined(__APPLE__) || defined(__MACH__)
#define OS "macOS"
#elif defined(__linux__)
#define OS "Linux"
#else
#define OS "Unknown"
#endif


#if defined(__aarch64__)
#define ARCH "ARM64"
#elif defined(__x86_64__)
#define ARCH "x86_64"
#else
#define ARCH "Unknown"
#endif

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (strcmp(OS, "Windows") == 0) {
        std::cerr << "Sorry, Windows is not supported, yet. It will be added in the future" << std::endl;
        return EXIT_FAILURE;
    }

    std::string version = "0.0.1";
    std::string lunarVersion = "2025.0";
    std::string product = "Lunarite";
    if (strcmp(OS, "macOS") == 0) {
        product = "Lunarite for Mac";
    }
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
        std::cout << "Lunarite version " << version << std::endl;
        std::cout << "The compiler of Lunar programming language" << std::endl;
        std::cout << std::endl;
        std::cout << "lrite [FILENAME]" << std::endl;
        std::cout << std::endl;
        std::cout << "  --help: Show this message and exit" << std::endl;
        std::cout << "  --version: Show the version of Lunarite" << std::endl;
         return 0;
    }
    if (arg1 == "--version") {
        std::cout << "Lunarite version " << version << " with Lunar version " << lunarVersion << " standards" << std::endl;
        std::cout << "Lunar, A 'beagleful' programming language" << std::endl;
        std::cout << "Developed by BeagleSoftware, licensed with MIT license" << std::endl;
        return 0;
    }

    std::cout << product << " version " << version << " with Lunar version " << lunarVersion << " standards" << std::endl;
    std::cout << "Compiling: " << arg1 << std::endl;
    std::cout << "System: " << OS << " with " << ARCH << " architecture" << std::endl;

    // std::cout << arg1 << std::endl;
    std::fstream input(argv[1], std::ios::in);
    std::string contents;
    {
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        input.close();
        contents = contents_stream.str();
    }

    struct stat info;
    if (stat("./lrite-build/", &info) != 0) {
        if (mkdir("lrite-build", 0777) == -1)
            std::cerr << "Error:  " << strerror(errno) << std::endl;
    }
    // std::cout << tokens_to_asm(tokens) << std::endl;

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeExit> tree = parser.parse();

    if (!tree.has_value()) {
        std::cerr << "No exit statement found" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(tree.value());
    // std::cout << generator.generate() << std::endl;
    {
        std::fstream file("./lrite-build/out.asm", std::ios::out);
        file << generator.generate();
    }

    // Call the compiler
    // First check the OS
    if (strcmp(OS, "macOS") == 0)
        system("clang ./lrite-build/out.asm -o ./lrite-build/out");
    else {
        system("as -o ./lrite-build/out.o ./lrite-build/out.asm");
        system("ld -o ./lrite-build/out ./lrite-build/out.o");
    }

    std::cout << "Successfully compiled " << arg1 << std::endl;

    return EXIT_SUCCESS;
}
