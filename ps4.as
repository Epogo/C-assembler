;file ps.as
;sample source code


.extern val1
K: 	.dw 	31,-12

.entry  Next
.extern wNumber	 
STR:    .asciz  "aBcd"
MAIN:   add 	$3,$5,$9
LOOP:		 	ori 	$9,-5,$2
 	la 	val1
 	jmp 	Next
Next: 	move 	$20,$4


LIST: 	.db 	6,-9
 	bgt 	$4,$2,END
 	la 	K


 	sw 	$0,4,$10
 	bne 	$31,$9, LOOP
 	call 	val1
 	jmp 	$4
 	la 	wNumber
 	.dh 	27056
END: 	stop	
.entry val1   
.entry 	K
