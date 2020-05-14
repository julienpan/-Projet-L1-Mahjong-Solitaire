#lang racket/base

(require parser-tools/lex
         (prefix-in : parser-tools/lex-sre))

(provide get-token operators values names keywords)

(define-empty-tokens keywords
	(Leq Lopar Lcpar Lfunc Ldpoint Lsc Lfor Lif Lelse Lwhile Lmain Land Lor Lxor Lleft Lright Lrange Lin Lcomma))

(define-empty-tokens operators
  (Leof Lspace Lplus Lmin Lmul Ldiv Lsup Linf Lseq Lieq Leqeq Lneq Lmod Lprint Lquote))

(define-tokens names
	(Lid))

(define-tokens values
  (Lnum))

(define get-token
  (lexer
   ((eof) (token-Leof))
   (whitespace 		(get-token input-port))
   ("+"				(token-Lplus))
   ("-"				(token-Lmin))
   ("*"				(token-Lmul))
   ("/"				(token-Ldiv))
   (">"				(token-Lsup))
   ("<"				(token-Linf))
   (">="			(token-Lseq))
   ("<="			(token-Lieq))
   ("=="			(token-Leqeq))
   ("!="			(token-Lneq))
   ("%"				(token-Lmod))
   ("="				(token-Leq))
   ("("				(token-Lopar))
   (")"				(token-Lcpar))
   (";"				(token-Lsc))
   ("def"			(token-Lfunc))
   (":"				(token-Ldpoint))
   ("if"			(token-Lif))
   ("else"		(token-Lelse))
   ("for"			(token-Lfor))
   ("while"	  (token-Lwhile))
   ("print"		(token-Lprint))
   ("'"				(token-Lquote))
   ("&"				(token-Land))
   ("or"			(token-Lor))
   ("^"				(token-Lxor))
   ("<<"			(token-Lleft))
   (">>"			(token-Lright))
   ("in"			(token-Lin))
   ("range"			(token-Lrange))
   (","				(token-Lcomma))
   ((:+ numeric) 	(token-Lnum (string->number lexeme)))
   ((:+ alphabetic) (token-Lid (string->symbol lexeme)))))
