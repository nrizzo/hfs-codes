;; Funzioni utili
(define number->bin-rep ;val: string
  (lambda (n) ;n: natural
    (cond ((= n 0) "0")
          ((= n 1) "1")
          ((= (modulo n 2) 0) (string-append (number->bin-rep (quotient n 2)) "0"))
          (else (string-append (number->bin-rep (quotient n 2)) "1"))
)))
(define bin->int ; val: int
  (lambda (s) ; s: stringa
    (cond ((string=? s "") 0)
          ((char=? (last-char s) #\0) (* 2 (bin->int (string-split s))))
          (else (+ 1 (* 2 (bin->int (string-split s)))))
)))
(define string-split ; val: stringa
  (lambda (s) ; s: stringa
    (substring s 0 (- (string-length s) 1))
))
(define last-char ; val: char
  (lambda (p) ; p: stringa
    (string-ref p (- (string-length p) 1))
))

(define ones-count
  (lambda (s)
    (cond ((= (string-length s) 0) 0)
          ((char=? #\1 (string-ref s 0)) (+ 1 (ones-count (substring s 1))))
          (else (ones-count (substring s 1)))
)))



(define tetrazione2 ;val: natural
  (lambda (n) ;n: natural
    (tetrazione2-rec 1 n)
))

(define tetrazione2-rec
  (lambda (r n)
    (if (= n 0)
        r
        (tetrazione2-rec (expt 2 r) (- n 1)
))))

(define bin-rep-between
  (lambda (x y)
    (if (= x y)
        (number->bin-rep x)
        (string-append (number->bin-rep x) " " (bin-rep-between (+ x 1) y))
)))

(define highlight-prefix
  (lambda (s1 s2)
    (if (= (string-length s1) (string-length s2))
        (highlight-prefix-rec s1 s2 true)
        ""
)))
(define highlight-prefix-rec
  (lambda (s1 s2 f)
    (cond ((string=? s1 "") "")
          ((not f) (string-append "x" (highlight-prefix-rec (substring s1 1) (substring s2 1) f)))
          ((char=? (string-ref s1 0) (string-ref s2 0)) (string-append (string (string-ref s1 0))
                                                                       (highlight-prefix-rec (substring s1 1)
                                                                                          (substring s2 1)
                                                                                          f)))
          (else (string-append "x" (highlight-prefix-rec (substring s1 1) (substring s2 1) false)))
)))
(define highlight-prefix-between
  (lambda (x y)
    (if (= x y)
        (number->bin-rep x)
        (highlight-prefix (number->bin-rep x) (highlight-prefix-between (+ x 1) y)))
))

(define bin-rep-and
  (lambda (s1 s2)
    (let ((l1 (string-length s1))
          (l2 (string-length s2)))
      (cond ((= l1 0) s2)
            ((= l2 0) s1)
            ((> l1 l2) (string-append (substring s1 0 1) (bin-rep-and (substring s1 1) s2)))
            ((< l1 l2) (string-append (substring s2 0 1) (bin-rep-and s1 (substring s2 1))))
            ((or (char=? #\1 (string-ref s1 0)) (char=? #\1 (string-ref s2 0)))
             (string-append "1" (bin-rep-and (substring s1 1) (substring s2 1))))
            (else
             (string-append "0" (bin-rep-and (substring s1 1) (substring s2 1))))
))))

(define hfs-mask
  (lambda (i)
    (let ((s (number->bin-rep i)))
      (hfs-mask-rec s 0 (- (string-length s) 1))
)))

(define hfs-mask-rec
  (lambda (s i j)
    (cond ((= j 0) s)
          ((char=? #\1 (string-ref s i)) (hfs-mask-rec (bin-rep-and s (number->bin-rep j)) (+ i 1) (- j 1)))
          (else (hfs-mask-rec s (+ i 1) (- j 1))))
))

(define hfs-mask-between
  (lambda (i1 i2)
    (if (<= i1 i2)
        (cons (hfs-mask i1) (hfs-mask-between (+ i1 1) i2))
        '()
)))

(define show-list
  (lambda (l)
    (if (null? l)
        ""
        (string-append (number->string (car l)) ", " (show-list (cdr l)))
)))

(define uniq
  (lambda (l)
    (cond ((null? l) null)
          ((= (length l) 1) l)
          ((= (car l) (cadr l)) (uniq (cdr l)))
          (else (cons (car l) (uniq (cdr l))))
)))

;(show-list (map ones-count (hfs-mask-between 0 50)))
