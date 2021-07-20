;file ps.as
;sample source code


.entry  Next  r
.extern wNumber 4	 	
STR:    .asciz  "aBcd"
f1234567890123456789012345678901:   add 	$3,$5,$9
LOOP^: 	ori 	$9,-5,$2
 	la 	val1
 	jmp 	Next
Next: 	move 	$20,$4	 r	f
add: 	.db 	6,-9
 	bgt 	$4,$2,END
 	la 	K
 	sw 	$0,4,$10
 	bne 	$31,$9, LOOP
 	call 	val1
 	jmp 	$4
 	la 	wNumber
.extern val1
 	.dh 	27056
K: 	.dw 	31,-12
END: 	stop 	f	   
.entry 	K 
