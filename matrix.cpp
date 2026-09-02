// matrix_rain.cpp
// A "Matrix" style digital rain effect for the terminal.
//
// Build:   g++ -O2 -std=c++17 matrix_rain.cpp -o matrix_rain
// Run:     ./matrix_rain
// Stop:    Ctrl+C   (restores your terminal cleanly)
//
// Works on Linux/macOS terminals, and on Windows 10+ terminals that
// support ANSI escape sequences (Windows Terminal, modern PowerShell/cmd).

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <csignal>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    // Some MinGW header versions don't define this unless a newer Windows
    // version is targeted. Define it ourselves so the build never depends
    // on that: the value below is the real Win32 constant.
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

// ----------------------------- terminal helpers -----------------------------

static void enableAnsiOnWindows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static void getTerminalSize(int &rows, int &cols) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rows = w.ws_row;
    cols = w.ws_col;
#endif
    if (rows <= 0) rows = 24;
    if (cols <= 0) cols = 80;
}

static void hideCursor()  { std::cout << "\033[?25l"; }
static void showCursor()  { std::cout << "\033[?25h"; }
static void clearScreen() { std::cout << "\033[2J\033[H"; }

static void restoreTerminal() {
    showCursor();
    std::cout << "\033[0m\033[2J\033[H";
    std::cout.flush();
}

static void handleSigint(int) {
    restoreTerminal();
    std::exit(0);
}

// Some MinGW builds use the "win32 threads" model, which leaves
// std::this_thread / std::thread undefined at compile time. Sidestep
// that entirely by using the native Win32 sleep on Windows.
static void sleepMs(int ms) {
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

// --------------------------------- program ----------------------------------

int main() {
    enableAnsiOnWindows();
    std::signal(SIGINT, handleSigint);

    int rows, cols;
    getTerminalSize(rows, cols);

    // Character pool for the falling glyphs. Swap this string out
    // (or drop in Unicode katakana) to change the look.
    const std::string glyphs =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()-_=+[]{}<>/\\|~";

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int>    pickGlyph(0, (int)glyphs.size() - 1);
    std::uniform_real_distribution<float> pickSpeed(0.25f, 1.0f);   // rows/frame
    std::uniform_int_distribution<int>    pickTrail(6, 22);         // trail length
    std::uniform_real_distribution<float> pickStart(-40.0f, 0.0f);  // stagger start
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);

    // Per-cell state: what's drawn there, and how much "life" (brightness) is left.
    std::vector<std::vector<char>> glyphAt(rows, std::vector<char>(cols, ' '));
    std::vector<std::vector<int>>  lifeAt (rows, std::vector<int>(cols, 0));

    // Per-column state: each column is one falling drop.
    std::vector<float> pos(cols), speed(cols);
    std::vector<int>   trailLen(cols), lastRow(cols, -1000);

    for (int c = 0; c < cols; ++c) {
        pos[c]      = pickStart(rng);
        speed[c]    = pickSpeed(rng);
        trailLen[c] = pickTrail(rng);
    }

    clearScreen();
    hideCursor();

    std::string frame;
    frame.reserve((size_t)rows * cols * 12);

    while (true) {
        // ---- advance each column's drop ----
        for (int c = 0; c < cols; ++c) {
            pos[c] += speed[c];
            int headRow = (int)pos[c];

            if (headRow != lastRow[c] && headRow >= 0 && headRow < rows) {
                glyphAt[headRow][c] = glyphs[pickGlyph(rng)];
                lifeAt[headRow][c]  = trailLen[c];
            }
            lastRow[c] = headRow;

            // Once the whole trail has fallen off the bottom, respawn it.
            if (headRow - trailLen[c] > rows) {
                pos[c]      = pickStart(rng);
                speed[c]    = pickSpeed(rng);
                trailLen[c] = pickTrail(rng);
                lastRow[c]  = -1000;
            }
        }

        // ---- fade trails + occasional glyph flicker ----
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (lifeAt[r][c] > 0) {
                    lifeAt[r][c]--;
                    if (chance(rng) < 0.03f) glyphAt[r][c] = glyphs[pickGlyph(rng)];
                }
            }
        }

        // ---- render everything in one buffered write (no flicker) ----
        frame.clear();
        frame += "\033[H";
        int lastLevel = -1;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int life = lifeAt[r][c];
                int mx   = trailLen[c];
                int level;
                if (life <= 0)               level = 4;  // blank
                else if (life == mx)         level = 0;  // bright white head
                else if (life > mx * 2 / 3)  level = 1;  // bright green
                else if (life > mx / 3)      level = 2;  // mid green
                else                          level = 3;  // dark green

                if (level != lastLevel) {
                    switch (level) {
                        case 0: frame += "\033[1;38;5;231m"; break; // head: white
                        case 1: frame += "\033[1;38;5;46m";  break; // bright green
                        case 2: frame += "\033[38;5;34m";    break; // mid green
                        case 3: frame += "\033[38;5;22m";    break; // dark green
                        case 4: frame += "\033[0m";           break; // blank
                    }
                    lastLevel = level;
                }
                frame += (level == 4) ? ' ' : glyphAt[r][c];
            }
            if (r != rows - 1) frame += "\r\n";
        }

        std::cout << frame;
        std::cout.flush();
        sleepMs(45);
    }
}
