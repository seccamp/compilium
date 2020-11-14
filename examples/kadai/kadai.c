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

// "._.   ...   ._.   ._.   ...   ._.   ._.   ._.   ._.   ._.",
// "|.|   ..|   ._|   ._|   |_|   |_.   |_.   ..|   |_|   |_|",
// "|_|   ..|   |_.   ._|   ..|   ._|   |_|   ..|   |_|   ..|",

#include <stdio.h>

char num2str[3][58]= {
  "._.   ...   ._.   ._.   ...   ._.   ._.   ._.   ._.   ._.",
  "|.|   ..|   ._|   ._|   |_|   |_.   |_.   ..|   |_|   |_|",
  "|_|   ..|   |_.   ._|   ..|   ._|   |_|   ..|   |_|   ..|"
};

void to_lcd(char *buf){
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; *(buf + i) != 0; ++i) {
            int v = buf[i]-'0'; // when buf[i] : '5' v = 5
            printf("%.3s ", &(num2str[j][6*v]));
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    to_lcd(argv[1]);
}
