#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// Glyph language has codes of length 4. C string needs terminator hence 5.
#define CODE_LENGTH 5

#define CODE_NOP        "0000"
#define CODE_INPUT      "0001"
#define CODE_ROT        "0010"
#define CODE_SWAP       "0011"
#define CODE_PUSH       "0012"
#define CODE_RROT       "0100"
#define CODE_DUP        "0101"
#define CODE_ADD        "0102"
#define CODE_LBRACE     "0110"
#define CODE_OUTPUT     "0111"
#define CODE_MULTIPLY   "0112"
#define CODE_EXECUTE    "0120"
#define CODE_NEGATE     "0121"
#define CODE_POP        "0122"
#define CODE_RBRACE     "0123"

void getArgs(int argc, char **argv, FILE** fs, char& base) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Expected: ./<exec> $(input) $(base)\n");
        fprintf(stderr, "Where $(base) is optional. ");
        fprintf(stderr, "If not provided it will be 10.\n");
        exit(1);
    }
    
    *fs = fopen(argv[1], "r");
    if (*fs == nullptr) {
        fprintf(stderr, "Error at opening file %s\n", argv[1]);
        exit(1);
    }
    if (argc == 3) {
        base = atoi(argv[2]);
        if (base < 2 || base > 36) {
            fprintf(stderr, "$(base) must be greater than 1 and ");
            fprintf(stderr, "smaller than 37.\n");
            fclose(*fs);
            exit(1);
        }
    }
}

int to_int(char c) { 
    if (c >= '0' && c <= '9') return (int)c - '0'; 
    else return (int)c - 'A' + 10; 
}

char to_char(int num) { 
    if (num >= 0 && num <= 9) return (char)(num + '0'); 
    else return (char)(num - 10 + 'A'); 
} 

__int128_t my_pow(__int128_t n, int pow) {
    if (pow <= 0) return 1;

    __int128_t res = n;
    for (int i = 0; i < pow - 1; ++i) {
        res *= n;
    }
    return res;
}

std::string to_string(__int128_t n, const char base) {
    if (n == 0)  return "0";

    bool isNegative = false;
    if (n < 0) {
        n = -n;
        isNegative = true;
    }

    // Much faster than std::string.
    char str[40] = { 0 };
    char *s = str + sizeof(str) - 1;    // Start at the end.
    while (n != 0) {
        *--s = to_char(n % base);       // Save last digit.
        n /= base;                      // Drop it.
    }

    return isNegative ? '-' + std::string(s) : std::string(s);
}

__int128_t to_int128(std::string strNum, const char base) {
    if (strNum.size() < 1) return 0;
    __int128_t n = 0;

    bool isNegative = false;
    if (strNum[0] == '-') {
        strNum.erase(strNum.begin());
        isNegative = true;
    }
    for (int i = 0; i < strNum.size(); ++i) {
        n += to_int(strNum[i]) * my_pow(base, strNum.size() - 1 - i);
    }

    return isNegative ? -n : n;
}

int binarySearch(std::vector<int>& arr, int l,
                            int r, int x) { 
    while (l <= r) { 
        int m = l + (r - l) / 2; 
  
        // Check if x is present at mid.
        if (arr[m] == x)  return m; 
  
        // If x greater, ignore left half. 
        if (arr[m] < x) l = m + 1; 
  
        // If x is smaller, ignore right half. 
        else r = m - 1; 
    } 
  
    // If we reach here, then element was not present.
    return -1; 
} 

void decodeNextChar(char* code, char* decodedCode, unsigned char& counter,
                    char& nextDecodedNum) {
    bool decoded = false;
    for (int i = 0; i < counter; ++i) {
        if (code[i] == code[counter]) {
            decodedCode[counter] = decodedCode[i];
            decoded = true;
            break;
        }
    }
    if (!decoded) {
        decodedCode[counter] = nextDecodedNum + '0';
        ++nextDecodedNum;
    }
    ++counter;
}

// Return 0 if succeeded. Error code otherwise.
int evaluate(std::vector<std::string>& stack, const char *decodedCode,
                const char base,
                int& currentDecodeIndex,
                std::vector<int>& leftBraceIndex,
                std::vector<int>& rightBraceIndex,
                const bool ranByExecuteCode = false) {
    if (!strcmp(decodedCode, CODE_NOP)) {
        // Do nothing.
    } else if (!strcmp(decodedCode, CODE_INPUT)) {
        std::string strNum;
        std::cin >> strNum;
        // Number read from keyboard is not in the given base check.
        if (base > 10) {
            for (int i = 0; i < strNum.size(); ++i) {
                if (strNum[i] != '-'
                    && !((strNum[i] >= '0' && strNum[i] <= '9')
                        || (strNum[i] >= 'A'
                            && strNum[i] < 'A' + base - 10))) {
                    return -2;
                }
            }
        } else {
            for (int i = 0; i < strNum.size(); ++i) {
                if (strNum[i] != '-'
                    && (strNum[i] < '0' || strNum[i] >= '0' + base)) {
                    return -2;
                }
            }
        }
        // --------------------------------------------------------------------
        
        stack.push_back(strNum);
    } else if (!strcmp(decodedCode, CODE_ROT)) {
        if (stack.size() < 1) return -2;
        stack.insert(stack.begin(), stack[stack.size() - 1]);
        stack.pop_back();
    } else if (!strcmp(decodedCode, CODE_SWAP)) {
        if (stack.size() < 2) return -2;
        std::string tmp = stack[stack.size() - 2];
        stack[stack.size() - 2] = stack[stack.size() - 1];
        stack[stack.size() - 1] = tmp;
    } else if (!strcmp(decodedCode, CODE_PUSH)) {
        stack.push_back("1");
    } else if (!strcmp(decodedCode, CODE_RROT)) {
        if (stack.size() < 1) return -2;
        stack.push_back(stack[0]);
        stack.erase(stack.begin());
    } else if (!strcmp(decodedCode, CODE_DUP)) {
        if (stack.size() < 1) return -2;
        stack.push_back(stack[stack.size() - 1]);
    } else if (!strcmp(decodedCode, CODE_ADD)) {
        if (stack.size() < 2) return -2;
        __int128_t num1 = to_int128(stack[stack.size() - 1], base);
        stack.pop_back();
        __int128_t num2 = to_int128(stack[stack.size() - 1], base);
        stack.pop_back();
        stack.push_back(to_string(num1 + num2, base));
    } else if (!strcmp(decodedCode, CODE_LBRACE) && !ranByExecuteCode) {
        if (stack.size() < 1) return -2;
        if (stack[stack.size() - 1] == "0") {  // Jump outside
            // Search corresponding index to jump to.
            for (int i = 0; i < leftBraceIndex.size(); ++i) {
                if (leftBraceIndex[i] == currentDecodeIndex) {
                    /* Jump over it. Index will be incremented straight away
                    when we exit this function. */
                    currentDecodeIndex = rightBraceIndex[i];
                    break;
                }
            }
        }
    } else if (!strcmp(decodedCode, CODE_OUTPUT)) {
        if (stack.size() < 1) return -2;
        std::cout << stack[stack.size() - 1] << '\n';
        stack.pop_back();
    } else if (!strcmp(decodedCode, CODE_MULTIPLY)) {
        if (stack.size() < 2) return -2;
        __int128_t num1 = to_int128(stack[stack.size() - 1], base);
        stack.pop_back();
        __int128_t num2 = to_int128(stack[stack.size() - 1], base);
        stack.pop_back();
        stack.push_back(to_string(num1 * num2, base));
    } else if (!strcmp(decodedCode, CODE_EXECUTE)) {
        if (stack.size() < 4) return -2;

        std::string code[CODE_LENGTH - 1];
        for (int i = 0; i < CODE_LENGTH - 1; ++i) {
            code[i] = stack[stack.size() - 1];
            stack.pop_back();
        }

        char tmpDecodedCode[CODE_LENGTH]{};
        unsigned char counter{ 0 };
        char nextDecodedNum{ 0 };
        
        for (int i = 0; i < CODE_LENGTH - 1; ++i) {
            bool decoded = false;
            for (int i = 0; i < counter; ++i) {
                if (code[i] == code[counter]) {
                    tmpDecodedCode[counter] = tmpDecodedCode[i];
                    decoded = true;
                    break;
                }
            }
            if (!decoded) {
                tmpDecodedCode[counter] = nextDecodedNum + '0';
                ++nextDecodedNum;
            }
            ++counter;
        }


        return evaluate(stack, tmpDecodedCode, base, currentDecodeIndex,
            leftBraceIndex, rightBraceIndex, true);
    } else if (!strcmp(decodedCode, CODE_NEGATE)) {
        if (stack.size() < 1) return -2;

        std::string strNum = stack[stack.size() - 1];

        if (stack[stack.size() - 1][0] != '0') {
            if (stack[stack.size() - 1][0] == '-') {
                stack[stack.size() - 1].erase(stack[stack.size() - 1].begin());
            } else {
                stack[stack.size() - 1] = '-' + stack[stack.size() - 1];
            }
        }


    } else if (!strcmp(decodedCode, CODE_POP)) {
        if (stack.size() < 1) return -2;
        stack.pop_back();
    } else if (!strcmp(decodedCode, CODE_RBRACE) && !ranByExecuteCode) {
        /* Binary search currentDecodeIndex in the ascending sorted
        rightBraceIndex vector. */
        int i = binarySearch(rightBraceIndex, 0, rightBraceIndex.size() - 1,
            currentDecodeIndex);

        /* To previous index because it will be incremented straight away when
        we exit this function. */
        currentDecodeIndex = leftBraceIndex[i] - 1;
        
    } else {
        // Code doesn't exist.
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE* fs;
    char base{ 10 };
	getArgs(argc, argv, &fs, base);

    std::vector<std::string> stack;
    std::vector<char*> decodedCodes;

    unsigned char counter{ 0 };
    char code[CODE_LENGTH]{};         // Initialize the whole string to 0.
    char decodedCode[CODE_LENGTH]{};  // Initialize the whole string to 0.
    char nextDecodedNum{ 0 };
    char **tmpCode = (char**) malloc(sizeof(char*));
    while (true) { // Read and decode codes.
        if (counter != 0 && counter % 4 == 0) {  // Add decoded code.
            *tmpCode = (char*) malloc(CODE_LENGTH * sizeof(char));
            std::copy(decodedCode, decodedCode + CODE_LENGTH, *tmpCode);
            decodedCodes.push_back(*tmpCode);
            
            nextDecodedNum = 0;
            counter = 0;
        }

        code[counter] = fgetc(fs);
        if (feof(fs)) break;
        if (code[counter] < 33 || code[counter] > 126) continue;
        
        decodeNextChar(code, decodedCode, counter, nextDecodedNum);
    }
    if (counter != 0) {
        fprintf(stderr, "Error:%ld\n", decodedCodes.size());
        return -1;
    }
    
    free(tmpCode);
    if (fclose(fs)) {
        fprintf(stderr, "Error at closing file %s\n", argv[1]);   
    }


    std::vector<int> leftBraceIndex;   // Undefined order.
    std::vector<int> rightBraceIndex;  // Will always be ascending.
    std::vector<int> parantheses;
    // Fill left and right brace index.
    for (int i = 0; i < decodedCodes.size(); ++i) {
        if (!strcmp(decodedCodes[i], CODE_LBRACE)) {
            parantheses.push_back(i);
        } else if (!strcmp(decodedCodes[i], CODE_RBRACE)) {
            if (parantheses.size() <= 0) {
                // Right brace has no matching left brace.
                fprintf(stderr, "Error:%d\n", i);
                return -1;
            }
            leftBraceIndex.push_back(parantheses[parantheses.size() - 1]);
            parantheses.pop_back();
            rightBraceIndex.push_back(i);
        }
    }
    if (parantheses.size() != 0) {  // Left brace has no matching right brace. 
        fprintf(stderr, "Error:%ld\n", decodedCodes.size());
        return -1;
    }

    // Evaluate decoded codes.
    for (int i = 0; i < decodedCodes.size(); ++i) {
        int r = evaluate(stack, decodedCodes[i], base, i, leftBraceIndex,
                    rightBraceIndex);
        if (r) {
            fprintf(stderr, "Exception:%d\n", i);
            return r;
        }
    }

    // Free memory.
    for (int i = 0; i < decodedCodes.size(); ++i) {
        free(decodedCodes[i]);
    }
    decodedCodes.clear();

    return 0;
}
