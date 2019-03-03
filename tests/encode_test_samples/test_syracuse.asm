add R0 #20 R8 					; R8 = Uo
add R0 #200 R9					; R9 = Un
add R0 #0 R5 					; R5 = 0
Lsyracuse:  add R8 #0 R1		; R1 = U0 ; R5 = counter
Lstartloop: seq R5 R9 R7 		; R7 = R4 == Nfinal
            branz R7 Lquitloop 	; if end (R7 = 1) reached, jmp to exit
            add R5 #1 R5 		; R5 += 1
            div R1 #2 R2 		; R2 = Un // 2
            mult R2 #2 R3 		; R3 = 2 * (Un // 2)
            sub R1 R3 R4 		; R4 = Un % 2 = R1 - R3
            branz R4 Lodd 		; if Un%2 != 0 jmp
Leven:      add R2 #0 R1 		; Un = Un / 2 if even
            jmp Lstartloop R7 
Lodd:       mult R1 #3 R1
            add R1 #1 R1
            jmp Lstartloop R7
Lquitloop:  scall #1
