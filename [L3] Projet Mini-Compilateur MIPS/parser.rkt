#lang racket/base

(require "lexer.rkt"
         parser-tools/yacc
         "ast.rkt")

(provide calc-parse)

(define parse
  (parser
   (tokens operators values names keywords)
   (start prog)
   (end Leof)
   (grammar
   
    (prog
    	((expr)					(list $1))
    	((expr prog)        	(cons $1 $2))
    	((declaration prog)		(cons $1 $2))
    	((declaration)			(list $1))
    	((callFunc)				(list $1)))
   
    (expr
    	((val)					$1)
    	((operations)			$1)
    	((test)					$1)
    	((boucle)				$1)
    	((print)				$1))
    
    (val
    	((Lnum)				(Pnum $1))
    	((Lid)				(Pid $1)))
    	
   	(operations
   		((expr Lplus expr)	(Pop '+ $1 $3))
    	((expr Lmin expr)	(Pop '- $1 $3))
    	((expr Lmul expr)	(Pop '* $1 $3))
    	((expr Ldiv expr)	(Pop '/ $1 $3))
    	((expr Lsup expr)	(Pop '> $1 $3))
    	((expr Linf expr)	(Pop '< $1 $3))
    	((expr Lseq expr)	(Pop '>= $1 $3))
    	((expr Lieq expr)	(Pop '<= $1 $3))
    	((expr Leqeq expr)	(Pop '== $1 $3))
    	((expr Lneq expr)	(Pop '!= $1 $3))
    	((expr Lmod expr)	(Pop '% $1 $3))
    	((expr Land expr)	(Pop '& $1 $3))
    	((expr Lor expr)	(Pop 'or $1 $3))
    	((expr Lxor expr)	(Pop '^ $1 $3))
    	((expr Lleft expr)	(Pop '<< $1 $3))
    	((expr Lright expr)	(Pop '>> $1 $3)))
    	
    (declaration
    	((Lid Leq expr Lsc)					(Pdef $1 $3))
    	((Lfunc Lid Lopar Lcpar Ldpoint)	(Pfunc $2)))
    
    (test
    	((Lif expr Ldpoint expr Lelse Ldpoint expr)	(Pif $2 $4 $7)))
    
    (boucle
    	((Lwhile expr Ldpoint expr)										 (Pwhile $2 $4))
    	((Lfor Lid Lin Lrange Lopar expr Lcomma expr Lcpar Ldpoint expr) (Pfor $6 $8 $11)))
    
    (callFunc
    	((Lid Lopar Lcpar)		(Pcall $1)))
    
    (print
    	((Lprint Lopar expr Lcpar)		(PprintNum $3))
    	((Lprint Lopar Lquote expr Lquote Lcpar)	(PprintStr $4))))
    
   (precs (left Lplus)
   		  (left Lmin)
   		  (left Lmul)
   		  (left Ldiv)
   		  (left Lsup)
   		  (left Linf)
   		  (left Lseq)
   		  (left Lieq)
   		  (left Leqeq)
   		  (left Lneq)
   		  (left Lmod)
   		  (left Land)
   		  (left Lor)
   		  (left Lxor)
   		  (left Lleft)
   		  (left Lright))
     
   (error
    (lambda (ok? name value)
      (error 'Parser "Error?")))))

(define (calc-parse in)
  (parse (lambda () (get-token in))))
