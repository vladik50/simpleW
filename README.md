# simpleW — Simple Word Counter

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A lightweight terminal word counter with UTF-8 support.  
Counts lines, words, and characters. More accurate than wc for non-ASCII text.

## Features

- Counts lines, words, and characters
- UTF-8 aware: counts characters, not bytes (unlike wc)
- Counts the last line even without trailing newline (unlike wc)
- Optional --letters flag: counts only letters and digits
- Short flags for scripts: -l, -w, -c
- Zero dependencies: works on any Linux distribution

## Getting Started

### Option 1: Install a Pre-built Package

Download the package for your distribution from the [Releases](https://github.com/vladik50/simpleW/releases) page and install it.

**Debian / Ubuntu / Linux Mint:**
sudo dpkg -i simplew_1.0-1_amd64.deb

**Fedora / RHEL:**
sudo dnf install simplew-1.0-1.fcXX.x86_64.rpm

**Arch Linux / Manjaro:**
sudo pacman -U simplew-1.0-1-x86_64.pkg.tar.zst

### Option 2: Build from Source

**Requirements**
- g++ (GCC) with C++17 support
- make

**Clone and build**
git clone https://github.com/vladik50/simpleW.git
cd simpleW
make

**Compile manually**
g++ -std=c++17 -O2 -o simplew simplew.cpp

## Usage

simplew file.txt              # lines, words, chars

simplew --letters file.txt    # lines, words, letters (no spaces/punctuation)

simplew -l file.txt           # lines only

simplew -w file.txt           # words only

simplew -c file.txt           # chars only

cat file.txt | simplew        # read from stdin

**Example output:**

$ simplew file.txt

Lines:   142

Words:   1087

Chars:   6234

$ simplew --letters file.txt

Lines:   142

Words:   1087

Letters: 5231

## License

GNU GPL v3.0 or later. See LICENSE for full text.
