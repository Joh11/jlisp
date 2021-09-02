(define 't 't)

(define 'list
    (lambda (&rest args)
      args))

(define 'defmacro
    (macro (name args &rest body)
	   (list 'define (list 'quote name)
		 (cons 'macro (cons args body)))))

(defmacro defun (name args &rest body)
  (list 'define (list 'quote name)
	(cons 'lambda (cons args body))))

(defun null? (exp)
  (eq exp nil))

(defmacro defq (var value)
  (list 'define (list 'quote var) value))

(defmacro setq (var value)
  (list 'set (list 'quote var) value))

;; list manipulation functions
(defun cadr (x) (car (cdr x)))
(defun caar (x) (car (car x)))
(defun cdar (x) (cdr (car x)))
(defun cddr (x) (cdr (cdr x)))
(defun caddr (x) (car (cdr (cdr x))))

(define 'first car)
(define 'second cadr)
(define 'third caddr)

(defun append (a xs)
  (cons a xs))

(defun last (list)
  (cond ((null? list) nil)
	((null? (cdr list)) (car list))
	(t (last (cdr list)))))

;; Replacing xs -> list (shadowing) does not work due to the macro
;; expansion of if, which requires list for now
(defun length (xs)
  (if (null? xs) 0
      (+ 1 (length (cdr xs)))))

(defun prog1 (&rest body)
  (car body))

(defun progn (&rest body)
  (last body))

(defmacro if (test then &rest else)
  (list 'cond (list test then)
	(list t (cons 'progn else))))

;; logical operations
(defun not (a)
  (null? a))

;; TODO remove the and and or primitives

(defun deep-eq (a b)
  (cond ((or (null? a) (null? b)) (eq a b))
	((and (list? a) (list? b))
	 (and (eq (car a) (car b))
	      (deep-eq (cdr a) (cdr b))))
	(t (eq a b))))

(defun error (message)
  (print message)
  (exit 1))

(defun assert (x)
  (if x x
      (error 'assertion-failed)))

(defun mapcar (fun xs)
  (if (null? xs)
      nil
      (cons (fun (car xs)) (mapcar fun (cdr xs)))))

;; TODO fix this
(defmacro quasiquote (exp)
  (quasiquote-fun exp))

(defun quasiquote-fun (exp)
  (cond ((atom? exp) (list 'quote exp))
	;; in this case it's a list
	((eq 'unquote (car exp)) (progn
				   (assert (eq 2 (length exp)))
				   (cdr exp)))
	(t (cons 'list (mapcar quasiquote-fun exp)))))
