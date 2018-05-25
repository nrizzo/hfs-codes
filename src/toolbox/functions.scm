;; Funzioni utili
;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname useful_functions) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f () #f)))
(define number->bin-rep ;val: string
  (lambda (n) ;n: natural
    (cond ((= n 1) "1")
          ((= (modulo n 2) 0) (string-append (number->bin-rep (quotient n 2)) "0"))
          (else (string-append (number->bin-rep (quotient n 2)) "1"))
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
