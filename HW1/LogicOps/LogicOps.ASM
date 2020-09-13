format PE console

entry start

include 'win32a.inc'

section '.data' data readable writeable

        aStr db 'A = 10101010',0
        bStr db 10, 13, 'B = 00000001',0
        andStr db 10, 13, 'A AND B = %d', 0
        orStr db 10, 13, 'A OR B = %d', 0
        xorStr db 10, 13, 'A XOR B = %d', 0
        notStr db 10, 13, 'NOT A = %d', 0

        NULL = 0

section '.code' code readable executable

        start:
                push aStr
                call [printf]

                push bStr
                call [printf]

                mov eax, 10101010b
                and eax, 1b

                push eax
                push andStr
                call [printf]

                mov eax, 10101010b
                or eax, 1b

                push eax
                push orStr
                call [printf]

                mov eax, 10101010b
                xor eax, 1b

                push eax
                push xorStr
                call [printf]

                xor eax, eax
                mov al, 10101010b
                not al

                push eax
                push notStr
                call [printf]


                call [getch]

                push NULL
                call [ExitProcess]

section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'