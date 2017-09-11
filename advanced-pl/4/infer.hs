import qualified Data.ByteString.Char8 as BS
import Data.ByteString (ByteString)
import qualified Data.Map as Map

data Lamda = Start | Var String | Def String Lamda| App Lamda Lamda
    deriving Show

data Typo = Free String | Arrow Typo Typo | Eq Typo Typo 
    deriving Show

-- read Input using Byte String
-- used in final
getInput = do 
    s <- BS.getContents
    return (BS.lines s)

-- for finding the lamda variable, read until '.'
-- used in parse
todot ('.':xs) = ([], tail xs)
todot (x:xs) = let
    (l,ls) = todot xs
    in (x:l,ls)

-- for finding character constants
-- used in parse
tlchar (x:xs) = case x of ' ' -> ([],(x:xs))
                          '(' -> ([],(x:xs))
                          ')' -> ([],(x:xs))
                          x -> let (x',xs') = tlchar xs
                               in (x:x',xs')

-- function that parse input and morphs it into lamda function style
-- used in absorb
parse (x:xs) c =
      case x of '(' -> let m = head xs
                       in case m of ' ' -> parse (('('):(tail xs)) c
                                    '(' -> let (node,xs') = parse xs Start
                                           in case c of Start -> parse ('(':xs') node
                                                        c -> parse ('(':xs') (App c node)
                                    '\\' -> parse xs c
                                    ')' -> parse xs c
                                    m -> let (z,zs) = tlchar (tail xs)
                                             nod = (Var (m:z))
                                         in case c of Start -> parse ('(':zs) nod
                                                      c -> parse ('(':zs) (App c nod)
                '\\' -> let (vname,ls) = todot xs
                            hl = head ls
                            tl = tail ls
                        in case hl of '(' -> let (node,xs') = parse ls Start
                                             in case c of Start -> (Def vname node, tail xs')
                                                          c -> (App c (Def vname node), tail xs')
                                      hl -> let (z,zs') = tlchar tl
                                            in case c of Start -> (Def vname (Var (hl:z)), tail zs')
                                                         c -> (App c (Def vname (Var (hl:z))), tail zs')
                ' ' -> parse xs c
                ')' -> (c,xs)


-- function that applies la Curry rules
-- used in absorb
laCu::Map.Map String Int -> Lamda -> Int -> ([Typo],Typo,Int) 

laCu gamma (Var a) n = ([],Free ('@':(show (gamma Map.! a))),n)

laCu gamma (App a b) n = let 
    (c1,s,n') = laCu gamma a n
    (c2,t,n'') = laCu gamma b n'
    str = Free ('@':(show (n''+1)))
    in ((Eq s (Arrow t str)):(c1 ++ c2), str,n''+1)

laCu gamma (Def a b) n = let
    gamma' = Map.insert a (n+1) gamma
    (c,t,n') = laCu gamma' b (n+1)
    in (c, Arrow (Free ('@':(show (n+1)))) t,n') 

-- helping function for finding if a Free variable is inside a Typo
-- used in unify
isNot a (Free b) = a/=b
isNot a (Arrow t1 t2) = (isNot a t1) && (isNot a t2)

-- helping function to replace/substitute a free variable with another Typo in a list of Typo's
-- used in unify
replace _ _ [] = []
replace a t (e:exs) = (change a t e):(replace a t exs)

-- helping function to replace/substitute a free variable with another Typo
-- used in replace
change a t (Eq t1 t2) = Eq (change a t t1) (change a t t2)
change a t (Arrow t1 t2) = Arrow (change a t t1) (change a t t2)
change a t (Free t') = if a==t'
                            then t
                            else (Free t')

-- helping function to check for equality between to Typo's
-- used in unify
isEqu (Free a) (Free a') = a'==a
isEqu (Arrow a b) (Arrow a' b') = (isEqu a a') && (isEqu b b')
isEqu (Arrow a b) (Free c) = False
isEqu (Free c) (Arrow a b)  = False

-- W algorithm implementation for finding most general unifier
-- used in absorb
unify [] rez = Just rez
unify ((Eq t1 t2):exs) rez = if isEqu t1 t2 
                                then unify exs rez
                                else case t1 of Arrow s1 s2 -> case t2 of Arrow s1' s2' -> (unify ((Eq s1 s1'):(Eq s2 s2'):exs) rez)
                                                                          Free b -> if isNot b t1
                                                                                          then unify (replace b t1 exs) (rez++[(b,t1)])
                                                                                          else Nothing
                                                                          t2 -> Nothing
                                                Free a -> if isNot a t2
                                                                then unify (replace a t2 exs) (rez++[(a,t2)])
                                                                else Nothing
                                                t1 -> Nothing                            		

-- Applies constraints (~substitutions) created from unify to the final type of the general lamda function
-- used in absorb
applycons [] exp = exp
applycons ((a,t):as) exp = applycons as (change a t exp)


-- suceedes type inference from input
-- used in final
absorb [] = []
absorb (x:xs) = let 
    (node,_) = parse (BS.unpack x) Start; 
    (constraints,t,n) = laCu Map.empty node (-1)
    r = unify constraints []
    in case r of
            Nothing -> Nothing:(absorb xs) 
            Just a -> (Just ((\(l,_,_)-> l) (fix (applycons a t)))):(absorb xs)


-- Prints a valid expression in the appropriate form
-- used in printf
printExp (Arrow a b) = case a of
                            Arrow a' b' -> do putStr "("
                                              printExp a
                                              putStr ")"
                                              putStr " -> "
                                              printExp b
                            a -> do printExp a
                                    putStr " -> "
                                    printExp b

printExp (Free a) = do putStr (a)

-- If a solution arises from the constraints Nothing is return and so we print Type Error
-- In a different case we print the Expression using printExp
-- used in final
printf Nothing = do putStrLn "type error"
printf (Just x) = do printExp x
                     putStrLn ""


-- Applies lexicographic order to the '@' style variables of the final result
-- used in fix
morph map n (Free a)= let q = Map.lookup a map
                      in case q of Just x -> (Free x, map,n)
                                   Nothing -> let str = '@':(show n)
                                              in (Free str, Map.insert a str map,n+1)           

morph map n (Arrow t1 t2) = let
        (t1', map',n') = morph map n t1
        (t2', map'',n'') = morph map' n' t2
        in (Arrow t1' t2', map'',n'')

-- uses initialises morph function in order to apply lexicographic order
-- to the variables of the result
-- used in absorb
fix = morph Map.empty 0

-- function that reads input and gives desired output
-- used in main
final = do 
    input <- getInput
    let 
        a = absorb ((tail input))
    mapM_ (printf) a

-- execute!
main = do final
