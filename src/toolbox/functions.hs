-- Funzioni utili

tobinrep :: Int -> [Char]
tobinrep 0 = "0"
tobinrep 1 = "1"
tobinrep x
    | x `mod` 2 == 0 = (tobinrep (x `div` 2)) ++ ['0']
    | otherwise      = (tobinrep (x `div` 2)) ++ ['1']

-- AND tra due stringhe --
binrepAND :: [Char] -> [Char] -> [Char]
binrepAND x y = reverse (binrepANDaux (reverse x) (reverse y)) 

binrepANDaux :: [Char] -> [Char] -> [Char]
binrepANDaux x [] = x
binrepANDaux [] y = y
binrepANDaux (x:xs) (y:ys) = [cAND x y] ++ binrepANDaux xs ys
 where cAND '1' _ = '1'
       cAND _ '1' = '1'
       cAND _  _  = '0'

-- per trovare la "maschera" che rappresenta i nodi da calcolare
hfsmask :: Int -> [Char]
hfsmask x = hfsmaskrec b ((length b) - 1)
 where b = tobinrep x

hfsmaskrec :: [Char] -> Int -> [Char]
hfsmaskrec [] _ = []
hfsmaskrec (s:[]) _ = [s]
hfsmaskrec (s:xs) i
    | s == '1'  = [s] ++ (hfsmaskrec (binrepAND xs (tobinrep i)) (i-1))
    | otherwise = [s] ++ (hfsmaskrec xs (i-1))


onescount :: [Char] -> Int
onescount [] = 0
onescount ('1':xs) = 1 + onescount xs
onescount (_:xs) = onescount xs

tetr 0 = 1
tetr x = exp 2 
-- [ onescount (hfsmask x) | x <- [0..50] ] --
