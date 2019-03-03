#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>

#define P system("pause")

char uppercase_letter(char in) {
    if (in > 64 && in < 'J') return in;
    if (in > 'J' && in < 91) return in;
    if (in > 96 && in < 'j') return in-32;
    if (in > 'j' && in < 123) return in-32;
    return '\0';
}

char* receive_key(char* received_key, unsigned int* x) {
    std::cout << "This is for Playfair cipher. Input the key:" << std::endl;
    std::string in;
    std::cin >> in;
    unsigned int length = in.length();
    if (length > 26) {
        throw 2;
        received_key[0] = '\0';
        return received_key;
    }
    char check;
    for (unsigned int i = 0; i < length; i++) {
        check = uppercase_letter(in[i]);
        if (check == '\0') {
            throw 3;
            received_key[0] = '\0';
            return received_key;
        }
        if (x[check-65]) {
            throw 1;
            received_key[0] = '\0';
            return received_key;
        } else {
            x[check-65] = 1;
        }
        received_key[i] = check;
    }
    received_key[length] = '\0';
    if (x[9]) {
        for (unsigned int i = 0; i < length; i++) {
            if (received_key[i] == 'J') {
                if (length == 1) {
                    throw 3;
                    received_key[0] = '\0';
                    return received_key;
                }
                for (; i < length; i++) {
                    received_key[i] = received_key[i+1];
                }
            }
        }
    }
    x[9] = 1;
    return received_key;
}

bool print_keytable(char (&keytable)[5][5]) {
    std::cout << std::endl << "Keytable shows below:" << std::endl;
    for (unsigned int i = 0; i < 5; i++) {
        for (unsigned int j = 0; j < 5; j++) {
            std::cout << ' ' << keytable[i][j];
        }
        std::cout << std::endl;
    }
    return true;
}

bool make_keytable(char* key, char (&keytable)[5][5], unsigned int* x, unsigned int* y) {
    for (unsigned int i = 0, j = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            keytable[i][j] = 0;
        }
    }
    unsigned int length = strlen(key);
    for (unsigned int iter = 0, i = 0, j = 0; i < 5 && iter < length; i++) {
        for (j = 0; j < 5; j++, iter++) {
            if (iter < length) {
                keytable[i][j] = key[iter];
            } else {
                break;
            }
        }
    }
    for (unsigned int i = 0, j = 0, k = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (keytable[i][j] == 0) {
                for (; x[k] && k < 26;) {
                    k++;
                }
                keytable[i][j] = k+65;
                k += 1;
            }
        }
    }
    for (unsigned int i = 0, j = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            char pos = keytable[i][j]-65;
            x[pos] = i;
            y[pos] = j;
        }
    }
    return true;
}

bool mode_select() {
    std::cout << "If encrypt, input 1; If decrypt, input 0:" << std::endl;
    unsigned int mode = 1;
    if (!scanf("%u", &mode)) {
        throw 10;
        return true;
    }
    fflush(stdin);
    if (mode == 1) {
        return true;
    } else if (mode == 0) {
        return false;
    } else {
        throw 10;
        return true;
    }
}

bool is_valid(char to_be_checked) {
    return uppercase_letter(to_be_checked);
}

bool output(char a, char b, bool mode, char (&keytable)[5][5], unsigned int* x, unsigned int* y) {
    int num_a = uppercase_letter(a)-65, num_b = uppercase_letter(b)-65;
    if (mode) {
        if (x[num_a] == x[num_b]) {
            std::cout << (char)(keytable[x[num_a]][(y[num_a]+1)%5] + (a > 96 ? 32 : 0))
            << (char)(keytable[x[num_b]][(y[num_b]+1)%5] + (b > 96 ? 32 : 0));
        } else if (y[num_a] == y[num_b]) {
            std::cout << (char)(keytable[(x[num_a]+1)%5][y[num_a]] + (a > 96 ? 32 : 0))
            << (char)(keytable[(x[num_b]+1)%5][y[num_b]] + (b > 96 ? 32 : 0));
        } else {
            std::cout << (char)(keytable[x[num_a]][y[num_b]] + (a > 96 ? 32 : 0))
            << (char)(keytable[x[num_b]][y[num_a]] + (b > 96 ? 32 : 0));
        }
    } else {
        if (a) std::cout << a;
        if (b) std::cout << b;
    }
    return true;
}

std::string decrypt(std::string buffer, char (&keytable)[5][5], unsigned int* x, unsigned int* y) {
    unsigned int length = buffer.length();
    if (length % 2) {
        throw 21;
        return NULL;
    }
    for (unsigned int i = 0, j = 0, num_a, num_b; i < length; i += 2) {
        j = i+1;
        num_a = uppercase_letter(buffer[i]);
        num_b = uppercase_letter(buffer[j]);
        if (num_a == num_b || !num_a || !num_b) {
            throw 21;
            return NULL;
        }
        num_a -= 65;
        num_b -= 65;
        if (x[num_a] == x[num_b]) {
            buffer[i] = (char)(keytable[x[num_a]][(y[num_a]+4)%5] + (buffer[i] > 96 ? 32 : 0));
            buffer[j] = (char)(keytable[x[num_b]][(y[num_b]+4)%5] + (buffer[j] > 96 ? 32 : 0));
        } else if (y[num_a] == y[num_b]) {
            buffer[i] = (char)(keytable[(x[num_a]+4)%5][y[num_a]] + (buffer[i] > 96 ? 32 : 0));
            buffer[j] = (char)(keytable[(x[num_b]+4)%5][y[num_b]] + (buffer[j] > 96 ? 32 : 0));
        } else {
            buffer[i] = (char)(keytable[x[num_a]][y[num_b]] + (buffer[i] > 96 ? 32 : 0));
            buffer[j] = (char)(keytable[x[num_b]][y[num_a]] + (buffer[j] > 96 ? 32 : 0));
        }
    }
    char a = uppercase_letter(buffer[0]);
    char b = uppercase_letter(buffer[1]);
    if (a == b) {
        throw 21;
        return NULL;
    }
    for (unsigned int i = 2; i < length; i++) {
        a = b;
        b = uppercase_letter(buffer[i]);
        if (a == b) {
            throw 21;
            return NULL;
        } else if (a == 'X') {
            if (uppercase_letter(buffer[i-2]) == b) {
                buffer[i-1] = '\0';
            }
        } else if (a == 'Q') {
            if (uppercase_letter(buffer[i-2]) == 'X' && b == 'X') {
                buffer[i-1] = '\0';
            }
        }
    }
    if (uppercase_letter(buffer[length-1]) == 'X' || uppercase_letter(buffer[length-1]) == 'Q') buffer[length-1] = '\0';
    return buffer;
}

bool analyze(bool mode, char (&keytable)[5][5], unsigned int* x, unsigned int* y) {
    if (mode) {
        std::cout << std::endl << "Input your plaintext and your ciphertext will show up:" << std::endl;
        char last_b = 0, a, b, next_a = 0;
        while (true) {
            if (next_a) {
                a = next_a;
                next_a = 0;
            } else {
                while(!is_valid(a = (char)getc(stdin))) {
                    if (a == '\n') return true;
                    std::cout << '?';
                }
            }
            if (uppercase_letter(a) == uppercase_letter(last_b)) {
                b = a;
                a = 'X';
                output(a, b, mode, keytable, x, y);
            } else {
                while(!is_valid(b = (char)getc(stdin))) {
                    if (b == '\n') break;
                    std::cout << '?';
                }
                if (b == '\n') {
                    if (uppercase_letter(a) == 'X') {
                        b = 'Q';
                    } else {
                        b = 'X';
                    }
                    output(a, b, mode, keytable, x, y);
                    return true;
                } else if (uppercase_letter(a) == uppercase_letter(b)) {
                    next_a = b;
                    if (uppercase_letter(a) == 'X') {
                        b = 'Q';
                    } else {
                        b = 'X';
                    }
                    output(a, b, mode, keytable, x, y);
                } else {
                    last_b = b;
                    output(a, b, mode, keytable, x, y);
                }
            }
        }
    } else {
        std::cout << "\nInput your ciphertext and your plaintext will show up:" << std::endl;
        std::string buffer, to_be_printed;
        std::cin >> buffer;
        if (buffer.empty()) return true;
        try{
            to_be_printed = decrypt(buffer, keytable, x, y);
        } catch (int err) {
            switch (err) {
                case 21: std::cout << std::endl << "Impossible ciphertext!" << std::endl; P; return true;
            }
        }
        for (unsigned int i = 0, length = to_be_printed.length(); i < length; i += 2) {
            output(to_be_printed[i], to_be_printed[i+1], mode, keytable, x, y);
        }
        return true;
    }
    return false;
}

int main() {
    char keytable[5][5];
    unsigned int x[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int y[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char key[27];
    bool mode = true;
    try {
        receive_key(key, x);
    } catch (int err) {
        switch (err) {
            case 1: std::cout << "\nduplicate letters!" << std::endl; P; return 1;
            case 2: std::cout << "\ntoo long for a key!" << std::endl; P; return 2;
            case 3: std::cout << "\ninvalid input!" << std::endl; P; return 3;
        }
    }
    if (!make_keytable(key, keytable, x, y)) {P; return 42;}
    else print_keytable(keytable);
    try {
        mode = mode_select();
    } catch (int err) {
        switch (err) {
            case 10: std::cout << "\nselect 0 or 1!" << std::endl; P; return 10;
        }
    }
    while (true) {
        analyze(mode, keytable, x, y);
    }
    return 0;
}
