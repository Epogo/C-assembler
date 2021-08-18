;This is a comment

add $4,$5,$8
sub $8,$11,$17
and $7,$8,$20
or $7,$15,$31
move $5,$12
mvhi $17,$22
LOOP: mvlo $25,$1
addi $15,-5,$2
subi $18,-89,$7
andi $0, 122, $8
LABEL1: ori  $7, 101, $15
nori $2, 55,   $15
beq $15,   $12,  LOOP
.entry LABEL1
Label1: bne $18,   $1,  LABEL1
blt $5,$6,Label1
bgt $11,$15,LABEL2
lb $4,11,$6
sb $18,-12,$12
lw $12,-1024,$18
sw $1, 5  ,$22
lh $14, 15  ,$27
sh $7, -100  ,$31
.asciz "hello world"
 LABEL2: .db 7,8,11
 jmp $8
 call LABEL2
 la EXT
 stop
 .extern EXT
 END: .dh 88,102,502
 .dw  112,105,852
 ;Last Line
