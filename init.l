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

(defun last (list)
  (cond ((null? list) nil)
	((null? (cdr list)) (car list))
	(t (last (cdr list)))))

(defun prog1 (&rest body)
  (car body))

(defun progn (&rest body)
  (last body))

(defmacro if (test then &rest else)
  `(cond (,test ,then)
	 (t ,else)))
