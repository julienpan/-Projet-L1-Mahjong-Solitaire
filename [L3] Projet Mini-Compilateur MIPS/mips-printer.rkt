#lang racket/base

(require racket/match "ast.rkt")
(provide (all-defined-out))
					  
					  
(define (print-loc loc)
  (match loc
  	[(Lbl l)   (format "~a" l)]
    [(Mem b r) (format "~a($~a)" b r)]
	[(? symbol? r) (format "$~a" r)]))			  
		
			
(define (print-instr instr)
	(match instr
		
		[(Move rd rs)   (printf "move $~a, $~a\n" rd rs)]
		[(Li r i)		(printf "li $~a, ~a\n" r i)]
		[(La r a)   	(printf "la $~a, ~a\n" r (print-loc a))]
		
		[(Addi rd rs i) (printf "addi $~a, $~a, ~a\n" rd rs i)]
		[(Sub rd rs i)  (printf "sub $~a, $~a, ~a\n" rd rs i)]
		[(Mul rd rs r)	(printf "mul $~a, $~a, $~a\n" rd rs r)]
		[(Div rd rs)	(printf "div $~a, $~a\n" rd rs)]
		
		[(Bgt r1 r2 lbl)(printf "bgt $~a, $~a, ~a\n" r1 r2 lbl)]
		[(Blt r1 r2 lbl)(printf "blt $~a, $~a, ~a\n" r1 r2 lbl)]
		[(Bge r1 r2 lbl)(printf "bge $~a, $~a, ~a\n" r1 r2 lbl)]
		[(Ble r1 r2 lbl)(printf "ble $~a, $~a, ~a\n" r1 r2 lbl)]
		[(Beq r1 r2 lbl)(printf "beq $~a, $~a, ~a\n" r1 r2 lbl)]
		[(Bne r1 r2 lbl)(printf "bne $~a, $~a, ~a\n" r1 r2 lbl)]
		
		[(And rd r1 r2)(printf "and $~a, $~a, $~a\n" rd r1 r2)]
    	[(Or rd r1 r2) (printf  "or $~a, $~a, $~a\n" rd r1 r2)]
    	[(Xor rd r1 r2)(printf "xor $~a, $~a, $~a\n" rd r1 r2)]
    	[(Sll rd rs s) (printf  "sll $~a, $~a, ~a\n"  rd rs s)]
    	[(Srl rd rs s) (printf  "srl $~a, $~a, ~a\n"  rd rs s)]
		
		[(Sgt r1 r2 r3)(printf "sgt $~a, $~a, $~a\n" r1 r2 r3)]
		[(Slt r1 r2 r3)(printf "slt $~a, $~a, $~a\n" r1 r2 r3)]
		[(Sge r1 r2 r3)(printf "sge $~a, $~a, $~a\n" r1 r2 r3)]
		[(Sle r1 r2 r3)(printf "sle $~a, $~a, $~a\n" r1 r2 r3)]
		[(Seq r1 r2 r3)(printf "seq $~a, $~a, $~a\n" r1 r2 r3)]
		[(Sne r1 r2 r3)(printf "sne $~a, $~a, $~a\n" r1 r2 r3)]
		
		[(Mflo rs)      (printf "mflo $~a\n" rs)]
    	[(Mfhi rs)      (printf "mfhi $~a\n" rs)]
		[(B lbl)		(printf "b ~a\n" lbl)]
		[(Label l)		(printf "\n~a:\n" l)]
		[(Sw r loc)     (printf "sw $~a, ~a\n" r (print-loc loc))]
    	[(Lw r loc)     (printf "lw $~a, ~a\n" r (print-loc loc))]
		[(Syscall)  	(printf "syscall\n")]
		[(Jr r)			(printf "jr $~a\n" r)]
		[(Jal lbl)		(printf "jal ~a\n" lbl)]))
		

(define (print-data)
	(printf "\n.data\n"))


(define (mips-data data)
  	(hash-for-each data
                 (lambda (k v)
                 	(if (number? v)
                   		(printf "~a: .word ~s\n" k v)
                   		(printf "~a: .asciiz ~s\n" k v)))))


(define (main)
	(printf "\n.text\n.globl main\n"))