## 42-MICROSHELL - EXAMS-04 WORKING SOLUTION

This is a working solution for the microshell exam at 42.

It supports:

- Command execution

- Piping (|)

- Command separation (;)

- Built-in cd with error handling

## Compile
```
gcc -Wall -Wextra -Werror microshell.c -o microshell
```
## Example Test
```
./microshell /bin/echo hello ";" /bin/ls "|" /usr/bin/grep microshell
```
