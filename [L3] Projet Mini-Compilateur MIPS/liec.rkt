#lang racket/base

(require "parser.rkt" "ast.rkt" "compiler.rkt")

(define argv (current-command-line-arguments))
(cond
  ((= (vector-length argv) 1)
   (define in (open-input-string (vector-ref argv 0)))
   (define prog (parse-syntax in))
  
   (mips-eval prog (make-immutable-hash) 0)
      
   (close-input-port in)
   (newline)
  )
  (else
   (eprintf "Usage: racket liec.rkt \"prog\"\n")
   (exit 1)))
