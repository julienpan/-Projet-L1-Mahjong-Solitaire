#lang racket/base

(require racket/match "ast.rkt"
					  "mips-printer.rkt")

(provide (all-defined-out))
					  			

(define (mips-eval prog env fp-sp)
  (match prog 
    ((list expr)    			(comp prog env fp-sp))
    ((cons expr instr)  		(comp (list expr) env fp-sp) (mips-eval instr env fp-sp))))


; expression evaluation
(define (eval-expr prog env fp-sp)
	(match prog
		((Pnum n)			n)
		((Pid name)			name)
		((Pdef id expr)		(hash-set env id (eval-expr expr env fp-sp)))
		((Pfunc id)			(hash-set id env fp-sp))
		((PprintNum expr)	'())
		((Pop op v1 v2)		(match op
								('+ (+ (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('- (- (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('* (* (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('/ (floor (/ (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp))))
								('% (modulo (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('< (< (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('> (> (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('<= (<= (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('>= (>= (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))
								('=! ((not (equal? (eval-expr v1 env fp-sp) (eval-expr v2 env fp-sp)))))
								))))


(print-data)	
(mips-data (make-hash '((nl . "\n"))))
(mips-data (make-hash '((f . "\nFalse\n") (t . "\nTrue\n"))))
(mips-data (make-hash '((name . "\nError: this name is not defined\n"))))
						
(main)		
; MIPS expression translation				
(define (comp ast env fp-sp)
	(for-each print-instr
		(append 
			(match ast
			
				; number to display after compilation
				((list (Pnum n))
					(append
						(list (Li 'v0 n))
						(list (Move 'a0 'v0))
						(list (Li 'v0 1))
						(list (Syscall))))
				
				; character string, returns a Compile Error
				((list (Pid name))
					(append
						(list (La 't0 (Lbl 'name)))
						(list (Move 'a0 't0))
						(list (Li 'v0 4))
						(list (Syscall))
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))
						(list (Li 'v0 0))
						(list (Jr 'ra))))
				
				; Variable declaration
				((list (Pdef id expr))
					(mips-data (hash id (eval-expr expr env 0)))
					
					(append 
						(list (Lw 't0 (Lbl id)))
						(list (Move 'a0 't0))
						(list (Li 'v0 4))
						(list (Syscall))))
					
				; Function declaration					
				((list (Pfunc id))
					(list (Label id)))
				
				; Function call	
				((list (Pcall id))
					(append 
						(list (Jal id))
						(list (Li 'v0 10))
						(list (Syscall))))
					
				; Print(integer)	
				((list (PprintNum expr))
					(list (comp (list expr) env 0))
					(append
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))))
					
				; print(string) 
				((list (PprintStr expr))
					(append
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl (eval-expr expr env 0))))
						(list (Syscall))
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))))
				
				; While loop
				((list (Pwhile test expr))
					(match test 								
						((Pop op a b)
							(append
								(match op
									('>
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Ble 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile))))
									('<
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Bge 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile))))
									('>=
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Blt 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile))))
									('<=
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Bgt 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile))))
									('==
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Bne 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile))))
									('!=
										(append 
											(list (Label 'While))
											(list (Li 't0 (eval-expr a env 0)))
											(list (Li 't1 (eval-expr b env 0)))
											(list (Beq 't0 't1 'endWhile))
											(list (Li 'v0 (eval-expr expr env 0)))
											(list (Move 'a0 'v0))
											(list (Li 'v0 1))
											(list (Syscall))
											(list (B 'While))
											(list (Label 'endWhile)))))
											
								(list (Li 'v0 4))
								(list (La 'a0 (Lbl 'nl)))
								(list (Syscall))
								(list (Li 'v0 0))
								(list (Jr 'ra))))
							))
					
				; test if		
				((list (Pif (Pop op a b) expr1 expr2))
					(append
						(match op
							('>
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Sgt 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF))))	
							
							('<
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Slt 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF))))	
									
							('>=
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Sge 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF))))
							
							('<=
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Sle 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF))))	
							
							('==
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Seq 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF))))
									
							('!=
								(append 
									(list (Label 'IF))
									(list (Li 't0 (eval-expr a env 0)))
									(list (Li 't1 (eval-expr b env 0)))
									(list (Sne 't2 't0 't1))
									(list (Beq 't2 '0 'Else))
									(list (Li 'v0 (eval-expr expr1 env 0)))
									(list (B 'IF))
									(list (Label 'Else))
									(list (Li 'v0 (eval-expr expr2 env 0)))
									(list (Label 'EndIF)))))
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))
						(list (Li 'v0 0))
						(list (Jr 'ra))))
					
				; loop for
				((list (Pfor n1 n2 expr))
					(append 
						(list (Li 't0 (eval-expr n1 env 0)))
						(list (Li 't1 (eval-expr n2 env 0)))
						(list (Label 'For))
						(list (Beq 't1 't0 'endFor))
						(list (Addi 't0 't0 1))
						(list (Li 't2 (eval-expr expr env 0)))
						(list (Move 'a0 't2))
						(list (Li 'v0 1))
						(list (Syscall))
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))
						(list (B 'For))
						(list (Label 'endFor))
						(list (Li 'v0 4))
						(list (La 'a0 (Lbl 'nl)))
						(list (Syscall))
						(list (Li 'v0 0))
						(list (Jr 'ra))))
								
				; Arithmetic, logic and comparison operations
				((list (Pop op v1 v2))
					(append
						(match op
							('+ 
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Addi 't0 't0 (eval-expr v2 env 0)))
									(list (Move 'a0 't0))
									(list (Li 'v0 1))
									(list (Syscall))))
							('-
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Sub 't0 't0 (eval-expr v2 env 0)))
									(list (Move 'a0 't0))
									(list (Li 'v0 1))
									(list (Syscall))))
							('*
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
									(list (Mul 't1 't0 't1))
									(list (Move 'a0 't1))
									(list (Li 'v0 1))
									(list (Syscall))))
							('/
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
									(list (Div 't0 't1))
									(list (Mflo 's0))
									(list (Addi 'a0 's0 0))
									(list (Li 'v0 1))
									(list (Syscall))))
							
							('>
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
									(list (Bgt 't0 't1 'TRUE))
									(list (B 'FALSE))
									(list (Label 'TRUE))
						            (list (La 'a0 (Lbl 't)))
									(list (B 'next))
						            (list (Label 'FALSE))						            
						            (list (La 'a0 (Lbl 'f)))						            
						            (list (Label 'next))
						            (list (Li 'v0 4))
									(list (Syscall))))
						    ('<
						    	(append
						    		(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Blt 't0 't1 'TRUE))
						    		(list (B 'FALSE))
						    		(list (Label 'TRUE))
						    		(list (La 'a0 (Lbl 't)))
						    		(list (B 'next))
						    		(list (Label 'FALSE))
						    		(list (La 'a0 (Lbl 'f)))
						    		(list (Label 'next))
						    		(list (Li 'v0 4))
									(list (Syscall))))	
						   ('>=
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Bge 't0 't1 'TRUE))
						    		(list (B 'FALSE))
						    		(list (Label 'TRUE))
						    		(list (La 'a0 (Lbl 't)))
						    		(list (B 'next))
						    		(list (Label 'FALSE))
						    		(list (La 'a0 (Lbl 'f)))
						    		(list (Label 'next))
						    		(list (Li 'v0 4))
									(list (Syscall))))
						    ('<=
						    	(append 
						    		(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Ble 't0 't1 'TRUE))
						    		(list (B 'FALSE))
						    		(list (Label 'TRUE))
						    		(list (La 'a0 (Lbl 't)))
						    		(list (B 'next))
						    		(list (Label 'FALSE))
						    		(list (La 'a0 (Lbl 'f)))
						    		(list (Label 'next))
						    		(list (Li 'v0 4))
									(list (Syscall))))
						    ('==
						    	(append 
						    		(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Beq 't0 't1 'TRUE))
						    		(list (B 'FALSE))
						    		(list (Label 'TRUE))
						    		(list (La 'a0 (Lbl 't)))
						    		(list (B 'next))
						    		(list (Label 'FALSE))
						    		(list (La 'a0 (Lbl 'f)))
						    		(list (Label 'next))
						    		(list (Li 'v0 4))
									(list (Syscall))))
						    ('!=
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Beq 't0 't1 'FALSE))
						    		(list (B 'TRUE))
						    		(list (Label 'TRUE))
						    		(list (La 'a0 (Lbl 't)))
						    		(list (B 'next))
						    		(list (Label 'FALSE))
						    		(list (La 'a0 (Lbl 'f)))
						    		(list (Label 'next))
						    		(list (Li 'v0 4))
									(list (Syscall))))
						    ('%
						    	(append
						    		(list (Li 't0 (eval-expr v1 env 0)))
						    		(list (Li 't1 (eval-expr v2 env 0)))
						    		(list (Div 't0 't1))
						    		(list (Mfhi 's0))
						    		(list (Addi 'a0 's0 0))
						    		(list (Li 'v0 1))
									(list (Syscall))))
							
							('&
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
                        			(list (And 't2 't0 't1))
                        			(list (Move 'a0 't2))
                        			(list (Li 'v0 1))
									(list (Syscall))))
							
							('or 
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
                        			(list (Or 't2 't0 't1))
                        			(list (Move 'a0 't2))
                        			(list (Li 'v0 1))
									(list (Syscall))))
									
							('^ 
								(append
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Li 't1 (eval-expr v2 env 0)))
                        			(list (Xor 't2 't0 't1))
                        			(list (Move 'a0 't2))
                        			(list (Li 'v0 1))
									(list (Syscall))))
									
							('<<
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Sll 't1 't0 (eval-expr v2 env 0)))
									(list (Move 'a0 't1))
									(list (Li 'v0 1))
									(list (Syscall))))
									
							('>>
								(append 
									(list (Li 't0 (eval-expr v1 env 0)))
									(list (Srl 't1 't0 (eval-expr v2 env 0)))
									(list (Move 'a0 't1))
									(list (Li 'v0 1))
									(list (Syscall)))))
									
						(list 
							; next line
							(Li 'v0 4)
							(La 'a0 (Lbl 'nl))
						  	(Syscall)
						  	(Li 'v0 0)
						  	(Jr 'ra))))
			))))
