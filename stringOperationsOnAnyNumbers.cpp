#include <iostream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <cstdlib>

// ------------------------- Forward declarations -----------------------------
int to_int(char c);

char to_char(int num);

bool isSmaller(std::string str1, std::string str2);

int remainder_of_string (std::string str, int divisor);

std::string sub_two_strings(std::string str1, std::string str2,
    const __int8_t base = 10);

std::string add_two_strings(std::string str1, std::string str2,
    const __int8_t base = 10);
    
std::string string_division_by_int(std::string number, int divisor);

std::string multiply_two_strings(std::string str1, std::string str2,
    const __int8_t base = 10);

std::string my_pow_on_string(std::string strNum, int pow);

std::string from_decimal_string_to_string_base(std::string strNum,
    const __int8_t base);
    
std::string from_string_base_to_string_decimal(std::string strNum,
    const __int8_t base);

std::string from_base_string_to_base_string(std::string strNum,
                                            const __int8_t initBase,
                                            const __int8_t destBase = 10);
// ----------------------------------------------------------------------------

int to_int(char c)  { 
    if (c >= '0' && c <= '9') return (int)c - '0'; 
    else return (int)c - 'A' + 10; 
}

char to_char(int num) { 
    if (num >= 0 && num <= 9) return (char)(num + '0'); 
    else return (char)(num - 10 + 'A'); 
}

// Returns true if str1 is smaller than str2.
bool isSmaller(std::string str1, std::string str2) {
    // Calculate lengths of both string.
    int n1 = str1.length();
    int n2 = str2.length();

    if (n1 < n2) return true;
    if (n2 < n1) return false;
 
    for (int i = 0; i < n1; ++i) {
        if (str1[i] < str2[i])      return true;
        else if (str1[i] > str2[i]) return false;
    }
 
    return false;
}

// It must be positive numbers.
int remainder_of_string (std::string str, int divisor) {
    int len = str.length(); 
    int num, rem = 0; 

    // Loop that finds remainder.
    for (int i = 0; i < len; ++i) {
        num = rem * 10 + to_int(str[i]);
        rem = num % divisor;
    }
  
    return rem;
}

/* Only for positive string numbers that has str1 > str2. Use add_two_strings
with negative arguments instead for proper substraction. */
std::string sub_two_strings(std::string str1, std::string str2,
                            const __int8_t base) {
    // Before proceeding further, make sure str1 is not smaller.
    if (isSmaller(str1, str2)) swap(str1, str2);

    // Take an empty string for storing result.
    std::string str{ "" };

    // Calculate length of both strings.
    int n1 = str1.length();
    int n2 = str2.length();
    int diff = n1 - n2;

    // Initially take carry zero.
    int carry = 0;

    // Traverse from end of both strings.
    for (int i = n2 - 1; i >= 0; --i) {
        /* Do school mathematics, compute difference of current digits and
        carry. */
        int sub = to_int(str1[i + diff]) - to_int(str2[i]) - carry;
        if (sub < 0) {
            sub = sub + base;
            carry = 1;
        } else carry = 0;
 
        str.push_back(to_char(sub));
    }

    // Subtract remaining digits of str1[].
    for (int i = diff - 1; i >= 0; --i) {
        if (str1[i] == '0' && carry) {
            str.push_back(to_char(base - 1));
            continue;
        }
        int sub = to_int(str1[i]) - carry;
        if (i > 0 || sub > 0) str.push_back(to_char(sub));
        carry = 0;
    }
    
    // Remove potential 0's
    for (int i = str.size() - 1; i >= 0; --i) {
        if (str[i] == '0') {
            str.erase(str.end() - 1);
        } else break;
    }

    // Reverse resultant string.
    for (int i = 0; i < str.size() / 2; ++i) {
        char tmp = str[i];
        str[i] = str[str.size() - i - 1];
        str[str.size() - i - 1] = tmp;
    }

    return str;
}

std::string add_two_strings(std::string str1, std::string str2,
                            const __int8_t base) {
    // Make sure we remove the minus from strings if exists.
    bool n1Negative{ false };
    if (str1[0] == '-') {
        str1.erase(str1.begin());
        n1Negative = true;
    }
    bool n2Negative{ false };
    if (str2[0] == '-') {
        str2.erase(str2.begin());
        n2Negative = true;
    }

    // Take an empty string for storing result.
    std::string str{ "" };

    /* Convention:
    Both strings positive or negative
        => Make sure length of str2 is larger. (basic addition)

    str1 negative, str2 positive
        => abs(str1) < abs(str2)
            => strNegative = false
        => abs(str1) > abs(str2)
            => strNegative = true
        
    str1 positive, str2 negative
        => abs(str1) < abs(str2)
            => strNegative = true
        => abs(str1) > abs(str2)
            => strNegative = false
    */
    if (n1Negative && !n2Negative) {
        if (isSmaller(str1, str2)) {
            // First argument is the bigger one.
            str = sub_two_strings(str2, str1);
        } else {
            // First argument is the bigger one.
            str = '-' + sub_two_strings(str1, str2);
        }
    } else if (!n1Negative && n2Negative) {
        if (isSmaller(str1, str2)) {
            // First argument is the bigger one.
            str = '-' + sub_two_strings(str2, str1);
        } else {
            // First argument is the bigger one.
            str = sub_two_strings(str2, str1);
        }
    } else {
        if (str1.length() > str2.length()) std::swap(str1, str2);
        // Calculate length of both strings.
        int n1 = str1.length();
        int n2 = str2.length();
        int diff = n2 - n1;


        // Initially take carry zero.
        int carry{ 0 };

        // Traverse from end of both strings.
        for (int i = n1 - 1; i >= 0; --i) {
            // Do school mathematics, compute sum of current digits and carry.
            int d1 = to_int(str1[i]);

            
            int d2 = to_int(str2[i + diff]);

            int sum = d1 + d2 + carry;
            str.push_back(to_char(sum % base));
            carry = sum / base;
        }

        // Add remaining digits of str2[].
        for (int i = diff - 1; i >= 0; --i) {
            int sum = to_int(str2[i]) + carry;
            str.push_back(to_char(sum % base));
            carry = sum / base;
        } 

        // Add remaining carry.
        if (carry) str.push_back(to_char(carry));

        // Reverse resultant string.
        for (int i = 0; i < str.size() / 2; ++i) {
            char tmp = str[i];
            str[i] = str[str.size() - i - 1];
            str[str.size() - i - 1] = tmp;
        }

        if (n1Negative && n2Negative) str = '-' + str;
    }

    return str; 
}

std::string string_division_by_int(std::string number, int divisor) {
    // As result can be very large store it in string.
    std::string ans;

    // Make sure we remove the minus from strings if exists.
    bool n1Negative{ false };
    if (number[0] == '-') {
        number.erase(number.begin());
        n1Negative = true;
    }
    bool n2Negative{ false };
    if (divisor < 0) {
        divisor = -divisor;
        n2Negative = true;
    }

    // Find prefix of number that is larger than divisor.
    int idx = 0;
    int temp = number[idx] - '0';
    while (temp < divisor) {
        temp = temp * 10 + (number[++idx] - '0');
    }

    /* Repeatedly divide divisor with temp. After every division, update temp
    to include one more digit. */
    while (number.size() > idx) {
        // Store result in answer i.e. temp / divisor.
        ans += (temp / divisor) + '0';

        // Take next digit of number.
        temp = (temp % divisor) * 10 + number[++idx] - '0';
    }

    // If divisor is greater than number.
    if (ans.length() == 0) return "0";

    if ((n1Negative && !n2Negative) || (!n1Negative && n2Negative)) {
        ans = '-' + ans;
    }

    return ans;
}

std::string multiply_two_strings(std::string str1, std::string str2,
                                    const __int8_t base) {
    if (str1.size() == 0 || str2.size() == 0) return "0";

    // Make sure we remove the minus from strings if exists.
    bool n1Negative{ false };
    if (str1[0] == '-') {
        str1.erase(str1.begin());
        n1Negative = true;
    }
    bool n2Negative{ false };
    if (str2[0] == '-') {
        str2.erase(str2.begin());
        n2Negative = true;
    }
    
    int len1 = str1.size();
    int len2 = str2.size();

    // Will keep the result number in vector in reverse order.
    std::vector<int> result(len1 + len2, 0);

    // Below two indexes are used to find positions in result.
    int i_n1 = 0;
    int i_n2 = 0;

    // Go from right to left in str1.
    for (int i = len1 - 1; i >= 0; --i) {
        int carry = 0;
        int n1 = to_int(str1[i]);

        /* To shift position to left after every multiplication of a digit in
        str2. */
        i_n2 = 0;

        // Go from right to left in str2
        for (int j = len2 - 1; j >= 0; --j) {
            // Take current digit of second number.
            int n2 = to_int(str2[j]);

            /* Multiply with current digit of first number and add result to
            previously stored result at current position. */
            int sum = n1 * n2 + result[i_n1 + i_n2] + carry;

            // Carry for next iteration.
            carry = sum / base;

            // Store result.
            result[i_n1 + i_n2] = sum % base;

            ++i_n2;
        } 

        // Store carry in next cell.
        if (carry > 0) {
            result[i_n1 + i_n2] += carry;
        }

        /* To shift position to left after every multiplication of a digit in
        str1. */
        ++i_n1;
    }

    // Ignore '0's from the right.
    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0) --i;

    // If all were '0's - means either both or one of str1 or str2 were '0'.
    if (i == -1) return "0";

    // Generate the result string.
    std::string s = "";

    while (i >= 0) {
        s += std::to_string(result[i--]);
    }

    if (n1Negative && !n2Negative || !n1Negative && n2Negative) {
        s = '-' + s;
    }

    return s;
}

std::string my_pow_on_string(std::string strNum, int pow) {
    if (pow <= 0) return "1";

    std::string s{ strNum };
    for (int i = 0; i < pow - 1; ++i) {
        s = multiply_two_strings(s, strNum);
    }
    return s;
}

// base = strNum's destination base.
std::string from_decimal_string_to_string_base(std::string strNum,
                                                const __int8_t base) {
    if (strNum.size() == 0) return "";
    if (strNum == "0")      return "0";

    bool isNegative = false;
    if (strNum[0] == '-') {
        strNum.erase(strNum.begin());
        isNegative = true;
    }

    std::string s{ "" };
    while (strNum != "0") {
        s = to_char(remainder_of_string(strNum, base)) + s;
        strNum = string_division_by_int(strNum, base);
    }

    return isNegative ? '-' + s : s;
}

// base = strNum's initial base.
std::string from_string_base_to_string_decimal(std::string strNum,
                                                const __int8_t base) {
    if (strNum.size() == 0) return "";
    if (strNum == "0")      return "0";
    std::string s{ "0" };

    bool isNegative = false;
    if (strNum[0] == '-') {
        strNum.erase(strNum.begin());
        isNegative = true;
    }

    for (int i = 0; i < strNum.size(); ++i) {
        s = add_two_strings(s,
            multiply_two_strings(std::to_string(to_int(strNum[i])),
                my_pow_on_string(std::to_string(base),
                                strNum.size() - 1 - i)));
    }

    return isNegative ? '-' + s : s;
}

std::string from_base_string_to_base_string(std::string strNum,
                                            const __int8_t initBase,
                                            const __int8_t destBase) {
    return from_decimal_string_to_string_base(
        from_string_base_to_string_decimal(strNum, initBase),
        destBase);
}

int main() {
    // Examples.
    std::cout << '\n' << multiply_two_strings("A", "A", 16) << '\n';
    std::cout << '\n' << sub_two_strings("80", "A0", 16) << '\n';
    std::cout << '\n' << add_two_strings("20", "-80", 10) << '\n';
    return 0;
}
