#pragma once

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

class Generator {
public:
    inline explicit Generator(NodeExit root)
        : root_(std::move(root)) {}

    [[nodiscard]] std::string generate() const {
        if (strcmp(ARCH, "Unknown") == 0)
            std::cerr << "Unknown or unsupported architecture!" << std::endl;
        std::stringstream output;

        if (strcmp(ARCH, "x86_64") == 0)
            output << "global _start\n_start:\n";
        if (strcmp(ARCH, "ARM64") == 0 && strcmp(OS, "macOS") == 0)
            output << ".section __TEXT,__text\n.globl _main\n\n_main:\n";

        if (strcmp(ARCH, "x86_64") == 0) {
            output << "    mov rax, 60\n";
            output << "    mov rdi, " << root_.expr.int_lit.value.value() << "\n";
            output << "    syscall";
        } if (strcmp(ARCH, "ARM64") == 0) {
            if (strcmp(OS, "macOS") == 1 || strcmp(OS, "macOS") == -1)
                output << "    mov x8, #0x2000001\n";
            else
                output << "    mov x16, #1\n";

            output << "    mov x0, #" << root_.expr.int_lit.value.value() << "\n";
            output << "    svc #0\n";
        }

        return output.str();
    }
private:
    const NodeExit root_;
};
