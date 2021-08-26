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

(defun prog1 (&rest body)
  (car body))

(defun progn (&rest body)
  (last body))

(defmacro if (test then &rest else)
  (list 'cond (list test then)
	(list t (cons 'progn else))))

(defun mapcar (fun xs)
  (if (null? xs)
      nil
      (cons (fun (car xs)) (mapcar fun (cdr xs)))))

;; TODO fix this
(defmacro quasiquote (exp)
  (cond ((atom? exp) (list 'quote exp))
	;; in this case it's a list
	(t (list 'quote (mapcar (lambda (x) x) exp)))))
