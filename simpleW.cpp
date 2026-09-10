/*
 * simpleW — Simple Word Counter
 * Counts lines, words, and characters in a file or stdin.
 * Supports UTF-8 for accurate character counting (unlike wc which counts bytes).
 * Licensed under GNU GPL v3.0 or later.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

/*
 * isUTF8Continuation — checks if a byte is a UTF-8 continuation byte (10xxxxxx).
 * Used to count characters instead of bytes.
 */
bool isUTF8Continuation(unsigned char c) {
    return (c & 0xC0) == 0x80;
}

/*
 * countUTF8Chars — counts UTF-8 characters in a string.
 * Each character starts with a byte that is NOT a continuation byte.
 */
long long countUTF8Chars(const string &s) {
    long long count = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (!isUTF8Continuation((unsigned char)s[i])) {
            count++;
        }
    }
    return count;
}

/*
 * isUTF8Letter — checks if a byte sequence at position i is a letter or digit.
 * Returns true if the current character is alphanumeric (Latin, Cyrillic, etc.).
 * This is a simplified check: it treats any non-ASCII byte that starts a character
 * as a letter. Not perfect, but works for most European languages.
 */
bool isUTF8Letter(const string &s, size_t i) {
    unsigned char c = s[i];
    
    // ASCII letters and digits
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
        return true;
    }
    
    // UTF-8 multibyte character — treat as letter if it's the start of a sequence
    // This includes Cyrillic (0xD0-0xD1), Greek, Arabic, etc.
    // Skip punctuation and symbols that are encoded in UTF-8 (like em-dash, quotes)
    if (c >= 0xC0) {
        // Check for common punctuation UTF-8 sequences
        // 0xD0 0x90-0xBF, 0xD0 0x80-0x8F — Cyrillic letters
        // 0xD1 0x80-0xBF — Cyrillic letters
        // 0xE2 0x80 ... — dashes, quotes, etc. (skip)
        if (c == 0xE2) {
            // Could be em-dash, quotes, etc. — skip common punctuation
            return false;
        }
        return true;
    }
    
    return false;
}

int main(int argc, char *argv[]) {
    bool showLines = true;
    bool showWords = true;
    bool showChars = true;
    bool onlyLetters = false;
    string filename;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-l" || arg == "--lines") {
            showWords = false;
            showChars = false;
        } else if (arg == "-w" || arg == "--words") {
            showLines = false;
            showChars = false;
        } else if (arg == "-c" || arg == "--chars") {
            showLines = false;
            showWords = false;
        } else if (arg == "--letters") {
            onlyLetters = true;
        } else if (arg == "-h" || arg == "--help") {
            cout << "Usage: simplew [options] [file]" << endl;
            cout << "Options:" << endl;
            cout << "  --letters     Count only letters and digits" << endl;
            cout << "  -l, --lines   Show only lines" << endl;
            cout << "  -w, --words   Show only words" << endl;
            cout << "  -c, --chars   Show only characters" << endl;
            cout << endl;
            cout << "Without arguments, reads from stdin." << endl;
            return 0;
        } else {
            filename = arg;
        }
    }
    
    // Open input source: file or stdin
    istream *input = &cin;
    ifstream file;
    
    if (!filename.empty()) {
        file.open(filename);
        if (!file.is_open()) {
            cout << "Error: cannot open file '" << filename << "'" << endl;
            return 1;
        }
        input = &file;
    }
    
    // Count lines, words, characters, and letters
    long long lines = 0;
    long long words = 0;
    long long chars = 0;
    long long letters = 0;
    
    string line;
    bool inWord = false;
    
    while (getline(*input, line)) {
        lines++;
        
        // Count UTF-8 characters (not bytes) plus 1 for newline
        chars += countUTF8Chars(line) + 1;
        
        for (size_t i = 0; i < line.length(); i++) {
            unsigned char c = line[i];
            
            // Count letters (only at start of UTF-8 sequence)
            if (!isUTF8Continuation(c) && isUTF8Letter(line, i)) {
                letters++;
            }
            
            // Word counting: split by whitespace (ASCII space, tab)
            if (c == ' ' || c == '\t' || c == '\r') {
                inWord = false;
            } else {
                if (!inWord) {
                    words++;
                    inWord = true;
                }
            }
        }
        inWord = false;
    }
    
    // Subtract the extra newline added after the last line
    if (chars > 0) chars--;
    
    // Print results
    if (showLines) cout << "Lines:   " << lines << endl;
    if (showWords) cout << "Words:   " << words << endl;
    
    if (onlyLetters) {
        if (showChars) cout << "Letters: " << letters << endl;
    } else {
        if (showChars) cout << "Chars:   " << chars << endl;
    }
    
    return 0;
}