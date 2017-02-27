"Night Machine" - interpreter of programming language called "Night".

Author: Arkadiusz Tomczak</br>
Version: 1.0</br>
Date: 20-12-2015</br>
Language:
 - Variable names: Polish
 - Comments: Polish

It is an interpreter of programming language "Night" which actually is a simple assembler. Source code should be passed as program parameter.</br>
It provides assembler features:</br>

Mnemonic - Night syntax</br>
ACC - `@`</br>
IND - `^`</br>
POP - `}`</br>
PSH - `{`</br>
IMM - `#`</br>
ABS - `'`</br>
DIS - `$`</br>
REL - `&`</br>
JSR - `\_`</br>
JMP - `\|`</br>
JPZ - `~`</br>
JNZ - `\~`</br>
NND - `|`</br>
DNN - `_`</br>
INC - `\}`</br>
DEC - `\{`</br>
DDA - `\+`</br>
BUS - `\-`</br>
LUM - `\*`</br>
VID - `\/`</br>
DOM - `\%`</br>
SNE - `\=`</br>
SGE - `\<`</br>
SLE - `\>`</br>
ADD - `+`</br>
SUB - `-`</br>
MUL - `*`</br>
DIV - `/`</br>
MOD - `%`</br>
SEQ - `=`</br>
SLT - `<`</br>
SGT - `>`</br>
LAA - `,`</br>
LAS - Grave accent</br>
LDA - `.`</br>
STA - `:`</br>
ICH - `?`</br>
OCH - `!`</br>
INU - `\?`</br>
ONU - `\!`</br>

You can also use comments (`;` works like `//` in C), named labels (the name should be one letter from English aphabet, definition is with `\`, go to with `_`) and brackets `()` and `[]`).</br>
You can write text if you start with `"` symbol (special characters like `\n` are supported (this will be interpreted as end of line)).

Example code (with translation to C):
```
_c                  ;
    \a 0            ;     int a;
    \b 0            ;     int b;
\c                  ;
\? a                ;     scanf("%d", &a);
\? b                ;     scanf("%d", &b);
\d .a \= b _e       ;     while (a != b) {
    .a > b _f       ;         if (a > b) {
        .b \- a     ;             a -= b;
    _g \e           ;         } else {
        .a \- b     ;             b -= a;
    \g              ;         }
_c \f               ;     }
\!a !#"\n"          ;     printf("%d\n", a);
```

Important note: Interpreter does not check syntax or any other features of source code. Refering to not initialized memory parts on other unexpected command cause undefined behaviour.</br></br>
Important note 2: Interpreter also supports notation based on decimal numbers, not only character symbols. It will stay here for backward compatibility, however recommended way to write
program to interprete is to use syntax showed above. Therefore I'm not going to explain in this readme file how to use syntax based on decimal numbers.
