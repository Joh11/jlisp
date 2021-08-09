(define 't 't)

(define 'list
    (lambda (&rest args)
      args))

(define 'defun
    (macro (name args &rest body)
	   (list 'define (list 'quote name)
		 (cons 'lambda (cons args body)))))

(define 'defmacro
    (macro (name args &rest body)
	   (list 'define (list 'quote name)
		 (cons 'macro (cons args body)))))
