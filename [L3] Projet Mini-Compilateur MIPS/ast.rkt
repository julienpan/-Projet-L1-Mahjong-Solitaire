#lang racket/base

(provide (all-defined-out))

;;; parsed syntax
(struct Pnum (n) #:transparent)
(struct Pop (op v1 v2) #:transparent)
(struct Pid (name) #:transparent)

(struct Pdef (id expr) #:transparent)
(struct Pfunc (id) #:transparent)
(struct Pcall (id) #:transparent)

(struct Pif (test expr1 expr2) #:transparent)
(struct Pwhile (test expr) #:transparent)
(struct Pfor (n1 n2 expr) #:transparent)

(struct PprintNum (expr) #:transparent)
(struct PprintStr (expr) #:transparent)

; instructions
(struct Move (rd rs))
(struct Li (r i))
(struct La (r a))

(struct Addi (rd rs i))
(struct Sub (rd rs i))
(struct Mul (rd rs r))
(struct Div (rd rs))

(struct Bgt (r1 r2 lbl))
(struct Blt (r1 r2 lbl))
(struct Bge (r1 r2 lbl))
(struct Ble (r1 r2 lbl))
(struct Beq (r1 r2 lbl))
(struct Bne (r1 r2 lbl))

(struct And (rd r1 r2))
(struct Or (rd r1 r2))
(struct Xor (rd r1 r2))
(struct Sll (rd rs s))
(struct Srl (rd rs s))

(struct Sgt (r1 r2 r3))
(struct Slt (r1 r2 r3))
(struct Sge (r1 r2 r3))
(struct Sle (r1 r2 r3))
(struct Seq (r1 r2 r3))
(struct Sne (r1 r2 r3))

(struct Mflo (rs))
(struct Mfhi (rs))
(struct B (lbl))
(struct Label (l))
(struct Sw (r Loc))
(struct Lw (r Loc))
(struct Syscall ())
(struct Jr (r))
(struct Jal (lbl))

; locations
(struct Lbl (l))
(struct Mem (b r))

; contants
(define PRINT_INT    1)
(define PRINT_STRING 4)