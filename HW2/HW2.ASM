; Вариант - 3

;Разработать программу, которая вводит одномерный массив A[N], формирует из элементов массива A
;новый массив B по правилам, указанным в таблице, и выводит его. Память под массивы может
;выделяться как статически, так и динамически по выбору разработчика.

;Массив B из сумм соседних элементов A ({A[0] + A[1], A[1] + A[2], ...}).

;Разбить решение задачи на функции следующим образом:

;    Ввод и вывод массивов оформить как подпрограммы.
;    Выполнение задания по варианту оформить как процедуру
;    Организовать вывод как исходного, так и сформированного массивов

;Указанные процедуры могут использовать данные напрямую (имитация процедур без параметров).
;Имитация работы с параметрами также допустима.


format PE console

entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize     db 'Enter the vector size: ', 0
        strIncorSize   db 'Incorrect size of vector = %d (must be more than or equal to 2)', 10, 0
        strVecElemI    db '[%d]? ', 0
        strScanInt     db '%d', 0
        strVecElemOut  db '[%d] = %d', 10, 0
        strVecA        db 'VectorA: ', 10, 0
        strVecB        db 'VectorB: ', 10, 0

        vec_size     dd 0
        sum          dd 0
        i            dd ?
        tmp1         dd ?
        tmp2         dd ?
        tmpStack     dd ?
        vecA         rd 100
        vecB         rd 100

;--------------------------------------------------------------------------
section '.code' code readable executable
        start:
                ; 1) vector input
                call VectorInput

                ; 2) Pair sum
                call PairSumVec

                ; 3) vectorA output
                push strVecA
                call [printf]
                mov eax, vecA
                call VectorOut

                mov eax, [vec_size]      ;reduce the vec_size for vecB output
                dec eax
                mov [vec_size], eax

                ; 4) vectorB output
                push strVecB
                call [printf]
                mov eax, vecB
                call VectorOut
        finish:
                call [getch]

                push 0
                call [ExitProcess]

;--------------------------------------------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 1
        jg  getVector
; fail size
        push [vec_size]
        push strIncorSize
        call [printf]
        jmp finish
; else continue...
getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA            ; ebx = &vec
getVecLoop:
        mov [tmp1], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp1]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret

;--------------------------------------------------------------------------
VectorOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, eax            ; ebx = &vec
outputVecLoop:
        mov [tmp1], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp1]
        add ebx, 4
        jmp outputVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret

;--------------------------------------------------------------------------
proc PairSumVec

        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA            ; ebx = vecA
        mov edx, vecB            ; edx = vecB

        mov eax, [vec_size]      ;reduce the number of loops by 1
        dec eax                  ;last element does not have a next element
        mov [vec_size], eax

        pairSumLoop:
                mov [tmp1], ebx
                mov [tmp2], edx
                cmp ecx, [vec_size]
                je endPairSumLoop      ; to end of loop

                mov eax, [sum]
                add eax, [ebx]         ;current element from vector
                add eax, [ebx + 4]     ;next element from vector
                mov [edx], eax         ;append to vecB

                mov [i], ecx

                mov ecx, [i]
                inc ecx
                mov ebx, [tmp1]
                mov edx, [tmp2]
                add ebx, 4
                add edx, 4
                jmp pairSumLoop

endPairSumLoop:

        mov eax, [vec_size]      ;restore vec_size
        inc eax
        mov [vec_size], eax

ret
endp

;--------------------------------------------------------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'