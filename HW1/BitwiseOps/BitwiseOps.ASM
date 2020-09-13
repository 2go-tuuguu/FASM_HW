format PE console

entry start

include 'win32a.inc'

section '.data' data readable writeable

        aStr db 'A = 10100011',0
        shlStr db 10, 13, 'Result shl: %d', 0
        shrStr db 10, 13, 'Result shr: %d', 0
        rorStr db 10, 13, 'Result ror: %d', 0
        rolStr db 10, 13, 'Result rol: %d', 0

        NULL = 0

section '.code' code readable executable

        start:
                push aStr
                call [printf]

                mov ecx, 10100011b
                shl ecx, 1

                push ecx
                push shlStr
                call [printf]

                mov ecx,  10100011b
                shr ecx, 1

                push ecx
                push shrStr
                call [printf]

                mov ecx, 10100011b
                rol ecx, 1

                push ecx
                push rolStr
                call [printf]

                mov ecx,  10100011b
                ror ecx, 1

                push ecx
                push rorStr
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