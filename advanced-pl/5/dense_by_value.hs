import qualified Data.ByteString.Char8 as BS
import Data.ByteString (ByteString)
import qualified Data.Map as Map

data Phrase = Const String | Output Phrase Phrase | Input Phrase | Var String | Abs String Phrase | App Phrase Phrase
    deriving Show

-- read Input using Byte String
getInput = do 
    s <- BS.getContents
    return (BS.lines s)

--parses input and morphs it in a Phrase form
parse (x:xs) = case x of '-' -> let (n,l) = (parse xs)
                                in (Input n,l)
                         '+' -> let (n,l) = parse xs
                                    (n',l') = parse l
                                in (Output n n',l')
                         '#' -> (Const ((head xs):[]), tail xs)
                         '/' -> let (n,l) = parse (tail xs)
                                in (Abs ((head xs):[]) n, l)
                         '@' -> let (n,l) = parse xs
                                    (n',l') = parse l
                                in (App n n',l')
                         x -> (Var (x:[]), xs)
              
-- substitutes Phrase b with a' if b is in form Var a. Applies that recursively (fixes a whole expr)
-- used for handling App&Abs phrase in app function
subs a a' b = case b of Input x -> Input (subs a a' x)
                        Output x x' -> Output (subs a a' x) (subs a a' x')
                        App x x' -> App (subs a a' x) (subs a a' x')
                        Const x -> Const x
                        Abs x x' -> if x==a then b
                                            else (Abs x (subs a a' x'))
                        Var x -> if (x == a) then a'
                                             else (Var x)

-- app result is ( Phrase expression, Rest Input, Output so far)
-- call by value case 2. Handle input the first time you see it and replace
-- anything it can be replaced by it's result
app::Phrase->String->(Phrase, String, String)
app exp inp = 
    case exp of App (Abs a b) c -> let (b',inp',out) = app c inp
                                       a' = subs a b' b
                                       (b'',inp'',out') = app a' inp'
                                   in (b'', inp'', out++out')
                Input (Abs a b) -> let b' = subs a (Const ((head inp):[])) b
                                   in app b' (tail inp)
                Output (Const x) z -> let (b',inp',out) = app z inp
                                      in (b',inp',x++out)
                exp -> case exp of App a b -> let (a',inp',out1) = app a inp
                                                  (z, inp'', out2) = app (App a' b) inp'
                                              in (z, inp'', out1++out2)
                                   exp -> (exp, inp, [])

-- assigns each variable with unique name
morph::Phrase->Int->Map.Map String String->(Phrase, Int)
morph x n map = case x of (Abs a b) -> let map' = Map.insert a ('v':(show (n+1))) map
                                           (ph, n') = morph b (n+1) map'
                                       in (Abs ('v':(show (n+1))) ph, n')
                          (App a b) -> let (ph, n') = morph a n map
                                           (ph', n'') = morph b n' map
                                       in (App ph ph',n'')
                          (Input a) -> let (ph, n') = morph a n map
                                       in (Input ph, n')
                          (Output a b) -> let (ph, n') = morph a n map
                                              (ph', n'') = morph b n' map
                                          in (Output ph ph',n'')
                          (Var a) -> (Var (map Map.! a), n)
                          (Const a) -> (Const a, n)

-- main: parse/morph/app/print_result
main = do xs <- getInput
          let (x,_) = parse (BS.unpack (head xs))
              (ph,_) = morph x 0 Map.empty
              (exp,_,ls) = app ph (BS.unpack (head (tail xs)))
          putStrLn ls
          

