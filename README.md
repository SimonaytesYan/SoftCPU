# SoftCPU
There are my own language with assembler to compile it to .sy file and soft processor to execute it.

## Processor architecture

There are
* Stack \
Stack with int elements to save main data 
* RAM \
With size RAM_SIZE (defined in ComandSystem.h)
* 4 registers:\
rax\
rbx\
rcx\
rdx

## How to use

* Write code some file with name something like TheBestProgram.txt
* Compile it using Assembler.exe in comand line:
```
.\Exe\Comp.exe TheBestProgram.txt
```
This comand will create executable file a.sy
* Then use processor to execute a.sy file:
```
.\Exe\Start.exe a.sy
```

## Comand system
Comand system in my language is simple:
There are 23 comands in it. Here they are all:
* **hlt**
Use for end program
* **push**
Use to push data in processor stack. You can push to stack:
    + number:
    ```
    push 100
    ```
    + from register:
    ```
    push rax
    ```
    + from RAM:
    ```
    push [10]
    ```
    or
    ```
    push [rax]
    ```
    or
    ```
    push [10 + rax]
    ```
* **add** \
Pop two elements from stack. Sum up them and push result in stack
* **sub** \
Pop two elements from stack. Subtracts the first from the second and push result in stack
* **mul** \
Pop two elements from stack. Multiply them and push result in stack
* **div** \
Pop two elements from stack. Divide the first by the second and push result in stack
* **out** \
Get element from stack and print it in cosole
* **pop** \
Pop element from stack and put it in
    + register:
    ```
    push rax
    ```
    + RAM:
    ```
    push [10]
    ```
    Push in stack data from RAM[10]
    ```
    push [rax]
    ```
    Push in stack data from RAM[rax]
    ```
    push [rax + 10]
    ```
    Push in stack data from RAM[rax + 10]
* **dump** \
Dump processor structure
* **jmp** \
Analogue go to. Go to label:
```
jmp label1

label1:
``` 

* Ñonditional jumps:
    + ja
    + jae
    + jb
    + jbe
    + je
    + jne \
    All this comands make jump under a certain condition. They pop two elements from stack: a and b, compare them and make jump to label if:
    + ja    a >  b
    + jae   a >= b
    + jb    a <  b
    + jbe   a <= b
    + je    a != b
    + jne   a == b

    ### Exemple
    ```
    push 1
    push 0

    je label1
    push 10
    out
    label1:

    push 20
    out
    ```
    This program will out 20.

* **call** \
This function make jump to label and push its own addres to call stack

* **ret** \
This function make pop from call stack and make jump to addres, derived from call stack 

### Exemple
```
push 1
push 2
push 3

call add_three_num

out

hlt

sum_tree_num:
    add
    add

    ret
```

This program output is 6

* **sqrt** \
Pop element from stack, takes the square root and push it in stack

* **infroot** \
Print in console "Inf roots":
```
infroots
``` 

* **noroots** \
Print in console "No roots":
```
noroots
``` 

* **draw** \
Use this comand to draw in separate window condition of RAM in the form, described in function DrawRam(). 
```
draw
``` 