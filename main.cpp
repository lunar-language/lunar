#include <iostream>

#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <cstring>

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

enum class TokenType {
    _exit,
    int_lit,
    semi,
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens {};

    std::string buffer {};
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buffer.push_back(c);
            i++;
            while (std::isalnum(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;

            if (buffer == "exit") {
                tokens.push_back({.type=TokenType::_exit});
                buffer.clear();
                continue;
            } else {
                std::cerr << "at " << str << " A syntax error occurred!" << std::endl << "In the future, Lunarite will have a better error reporting." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if (std::isdigit(c)) {
            buffer.push_back(c);
            i++;
            while (std::isdigit(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type=TokenType::int_lit, .value=buffer});
        } else if (c == ';') {
            tokens.push_back({.type=TokenType::semi});
        } else if (std::isspace(c)) {
            continue;
        } else {
            std::cerr << "at " << str << " A syntax error occurred!" << std::endl << "In the future, Lunarite will add a better error reporting." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    if (strcmp(ARCH, "Unknown") == 0)
        std::cerr << "Unknown or unsupported architecture!" << std::endl;
    std::stringstream output;

    if (strcmp(ARCH, "x86_64") == 0)
        output << "global _start\n_start:\n";
    if (strcmp(ARCH, "ARM64") == 0)
        output << ".section __TEXT,__text\n.globl _main\n\n_main:\n";

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_exit) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    if (strcmp(ARCH, "x86_64") == 0) {
                        output << "    mov rax, 60\n";
                        output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                        output << "    syscall";
                    } if (strcmp(ARCH, "ARM64") == 0) {
                        if (strcmp(OS, "macOS") == 1 || strcmp(OS, "macOS") == -1)
                            output << "    mov x8, #0x2000001\n";
                        else
                            output << "    mov x16, #1\n";

                        output << "    mov x0, #" << tokens.at(i + 1).value.value() << "\n";
                        output << "    svc #0\n";
                    }
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char *argv[]) {
    if (strcmp(OS, "Windows") == 0) {
        std::cerr << "Sorry, Windows is not supported, yet. It will be added in the future" << std::endl;
        return EXIT_FAILURE;
    }

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

    std::vector<Token> tokens = tokenize(contents);
    // std::cout << tokens_to_asm(tokens) << std::endl;


    {
        std::fstream file("./lrite-build/out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    // Call the compiler

    // First check the OS
    if (strcmp(OS, "macOS") == 0)
        system("clang ./lrite-build/out.asm -o ./lrite-build/out");
    else {
        system("as -o ./lrite-build/out.o ./lrite-build/out.asm");
        system("ld -o ./lrite-build/out ./lrite-build/out.o");
    }

    return EXIT_SUCCESS;
}
