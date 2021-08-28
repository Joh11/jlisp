;; test for logical operations
(assert (not nil))
(assert (and t t))
(assert (not (and t nil)))
(assert (or nil t))
(assert (not (or nil nil)))

;; tests for list?
(assert (list? nil))
(assert (list? (cons 1 2)))
(assert (not (list? 1)))
(assert (not (list? 'a)))

;; tests for mapcar
(assert (eq nil (mapcar (lambda (x) (+ 3 x)) nil)))
(assert (deep-eq (list 4 5 6) (mapcar (lambda (x) (+ 3 x)) (list 1 2 3))))
