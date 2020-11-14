/*
Your task is to create an LCD string representation of an
integer value using a 3x3 grid of space, underscore, and
pipe characters for each digit. Each digit is shown below
(using a dot instead of a space)

._.   ...   ._.   ._.   ...   ._.   ._.   ._.   ._.   ._.
|.|   ..|   ._|   ._|   |_|   |_.   |_.   ..|   |_|   |_|
|_|   ..|   |_.   ._|   ..|   ._|   |_|   ..|   |_|   ..|


Example: 910

._. ... ._.
|_| ..| |.|
..| ..| |_|
*/

#include <stdio.h>
void to_lcd(int n){
    char buf[12];
    snprintf(buf, 12, "%d", n);
    char lcd[3][30];
    for (char* p = buf; *p != 0; p++){

    }

}

int main() {

}
