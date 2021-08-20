.entry Start
.extern MIDcode
  bgt $15,$16,dataLabel
  add $17,$19,$31
  dataLabel: .db 100,-15,127,-128
   .dh +112,32,767,-32768,1
   .dw 10000,-15412,1058,2

add $22,$11,$3
	or $28,  $30,   $7
   sub $15,$17, $31
	and $24, $25 ,$29
	
	nor $19,$1,$0

MAIN:	move $11,$17
	mvhi $18  ,  $25

LABeL1: addi $15,-54, $24
andi $5,123,$16
subi $18,1024,$0
	.entry LABEL2
ori $5,-100,$25
nori $14,400,$3
blt $14,$25,MAIN
.extern OutLabel
Label3: beq $24,$1, LABeL1
LABEL2: bne $17,$18, LABeL1

STRING: .asciz "Authors:Evgeni and David"

 bgt $9,$19,MAIN
Label4: lb $15,311,$17
sb $15,245,$6
lw $31,-1254,$5
 sw $17,314,$15
 lh $30,5123,$17
 jmp OutLabel
 sh $12,-1,$18
Start: jmp $18
jmp MIDcode
la LABeL1
call MIDcode
ENDLABEL: stop
