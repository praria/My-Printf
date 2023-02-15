#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void print_char(char c) {
    write(1, &c, 1);
}
void print_string(const char *str){
    for (int i = 0; str[i] != '\0'; i++){
        print_char(str[i]);
    }
}
// The convertTo_baseInteger() is used to print the integer values in the specified base (decimal, octal, or hexadecimal)and sign representation (signed or unsigned). 
void convertTo_baseInteger(int value, int base, int is_signed) {
    // If "is_signed" flag is set and the "value" is negative, The function outputs a minus sign "-"
    if (value < 0 && is_signed) {
        print_char('-');
        value = -value;
    }

    // creates an array "buffer" of size 32 to store the character of the string representation of the number
    // do-while loop computes until value>0. The remainder we get from (value % base) is the index of the character in the string "0123456789ABCDEF"
    // The corresponding character in the string at this index is assigned to buffer[i] and keeps on looping this way until value >0
    char buffer[32];
    int i = 0;
    do {
        buffer[i++] = "0123456789ABCDEF"[value % base];        
        value /= base;
    } while (value > 0);
    // outputting the characters in the buffer array in reverse order which is the string representation of number in specified base
    while (i > 0) {
        print_char(buffer[--i]);
    }
}

// The "my_printf" function takes a format string "format" and a variable number of additional arguments represented by "..." and outputs a formatted string to the standard output.
// The format string specifies the output format and is processed character by character
// variable of type "va_list" named as "args" is used to represent a list of arguments passed to a function using "..." syntax
// "va_start" macro is then called which initializes "args" to point to the first variable argument in the list.
// The "format" argument in va_start  is used to determine the positon of the first variable argument in the list and "args" is set to point to this argument
// After the call to "va_start", the "args" variable can be used in the rest of "my_printf" function to access the remaining each subsequent variable arguments from the list.
// This is typically done by "va_arg" macro with "args" as first argument and a type as second argument to retrieve next argument from list
// If a character is not a format specifier (ie, not a percent sign), it is printed as in using the putchar() function.
// If a character is a percent sign, it is followed by a format specifier that determines the output format of the 
// corresponding argument. The format specifier supported by my_printf() are %d, %o, %u, %x, %c, %s and %p

int digitCount(int value, int base, int sign) {
    int count = 0;
    if(sign && value < 0){
        count++;
        value = -value;
    }
    do {
        value /= base;
        count++;
    } while (value);
    return count;
}

int my_printf(const char *format, ...) {
    int count = 0;
    va_list args;
    va_start(args, format);

    for (const char *p = format; *p != '\0'; p++) {
        if (*p != '%') {
            print_char(*p);
            count++;
            continue;
        }
        p++;
        switch (*p) {
            // The %d specifier is used to print a signed decimal integer. The corresponding argument is retrieved using 
            // va_arg(args, int) and passed to the convertTo_baseInteger() with "base" set to 10 and "is_signed" set to 1
            case 'd': {
                int value = va_arg(args, int);
                convertTo_baseInteger(value, 10, 1);
                count += digitCount(value, 10, 1);
                break;
            }
            // The %o specifier is used to print an unsigned octal integer. The corresponding argument is retrieved using
            // va_arg(args, unsigned int) and passed to the print_interger() function with "base" set to 8 and "is_signed" set to 0
            case 'o': {
                unsigned int value = va_arg(args, unsigned int);
                convertTo_baseInteger(value, 8, 0);
                count += digitCount(value, 8, 0);
                break;
            }
            // The %u specifier is used to print an unsigned decimal integer
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                convertTo_baseInteger(value, 10, 0);
                count += digitCount(value, 10, 0);
                break;
            }
            // The %x specifier is used to print an unsigned hexadecimal integer
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                convertTo_baseInteger(value, 16, 0);
                count += digitCount(value, 16, 0);
                break;
            }
            // The %c specifier is used to print a character. The corresponding argument is retrieved using va_arg(args, int)
            // and printed using the print_char() function
            case 'c': {
                int value = va_arg(args, int);
                print_char(value);
                count++;
                break;
            }
            // The %s specifier is used to print a string
            case 's': {
                char *value = va_arg(args, char *);
                if (value == NULL) {
                    value = "(null)";
                }
                int length = strlen(value);
                for (int i = 0; i < length; i++) {
                    print_char(value[i]);
                }
                count += length;
                break;
            }
            // The %p specifier is used to print a pointer in hexadecimal format.
            // va_arg is used to retrieve the next argument in va_list passed to my_printf. The type 'void *' is used because it can represent the address of any type of object.
            // "0x" is common prefix for hexadecimal values
            // The pointer value ptr is cast to type 'uintptr_t'  which is an unsigned integer type capable of representing a pointer value. The purpose of this is to be able to print the address as a hexadecimal value.
            // If the pointer is not null, the pointer is converted to an integer (uintptr_t value) and then formats the integer as a hexadecimal string
            // The width (total no. of digits) of the hexadecimal string is determined by the size of the pointer
            // The 'for' loop is used to convert the hexadecimal representation of a pointer value to a string and print it. The loop starts from the most significant digit at 'width-1' and goes down to the least significant digit at '0'
            // The digit is extracted from the pointer value by shifting it to the right by '4*i' bits and then masking it with '0xF'
            // If the digit is 0 and the 'leading_zero' flag is set, the code skips this iteration of the loop using 'continue'. The 'leading_zero' flag is used to avoid printing leading zeros
            // if the digit is not 0, the digit is converted to its ASCII representation. It the digit is less than 10, it is represented by the ASCII character with value '0' + 'digit', otherwise it is represented by the ASCII character with value 'a' + '(digit-10)'
            // The ASCII representation of the digit is printed using the 'print_char' function and the 'count' variable is incremented. The loop continues until all digits have been processed and printed.
            case 'p': {
                 void *ptr = va_arg(args, void *);

                 if (ptr == NULL) {
                     print_string("(null)");
                     count += 6; // there are 6 characters in "(null)""
                 }
                //  else {
                //     print_string("0x");
                //     uintptr_t value = (uintptr_t)(ptr);
                //     convertTo_baseInteger(value, 16, 1);
                //     count += 2 + digitCount(value, 16, 1);
                //  }
                 else {
                    uintptr_t value = (uintptr_t) (ptr);
                    int width = sizeof(ptr) * 2;
                    int leading_zero = 1;
                    print_string("0x");
                    count += 2;
                    for (int i = width - 1; i >= 0; i--) {
                        int digit = (value >> (4 * i)) & 0xF;
                        if (digit == 0 && leading_zero) {
                            continue;
                        }
                        else {
                            leading_zero = 0;
                        }
                        char c = digit < 10 ? '0' + digit : 'a' + (digit -10);
                        print_char(c);
                        count++;
                    }

                }
                 
                 break;
             }           
            default:
            print_char(*p);
            count++;
            break;
        }
    }
    va_end(args);
    return count;
}


int main() {
    // Test case 1: signed decimal integer
    my_printf("signed decimal integer: %d\n", -20);

    // Test case 2: unsigned octal integer
    my_printf("unsigned octal integer: %o\n", 30);

    // Test case 3: unsigned decimal integer
    my_printf("unsigned decimal integer: %u\n", 40);

    // Test case 4: unsigned hexadecimal integer
    my_printf("unsigned hexadecimal integer: %x\n", 50);
    printf("unsigned hexadecimal integer using famous printf(): %x\n", 50);

    // Test case 5: character
    my_printf("character: %c\n", 'A');

    // Test case 6: string
    char str[] = "Hello, world!";
    my_printf("String: %s\n", str);
    my_printf("null is handled as: %s\n", NULL);


    // Test case 7: pointer address
    int x = 60;
    int* ptr = &x;
    my_printf("Pointer address using reproduced my_printf: %p\n", ptr);
    printf("pointer address using famous printf is: %p\n", ptr);

    return 0;
}