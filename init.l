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
