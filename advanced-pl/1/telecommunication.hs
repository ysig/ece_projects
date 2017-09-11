{-# OPTIONS_GHC -static -rtsopts -O2 -optc-O2 #-}
-- getTree :: IO tree
import Data.List
import System.IO
import Data.Ord
import Data.Map (Map)
import Data.Maybe
import qualified Data.Map as Map
import Debug.Trace
import Data.Ord
import qualified Data.ByteString.Lazy.Char8 as Bs
data Tree a = Node a [Tree a]
   deriving Show
 
takeJust (y) = x
    where (x,_) = fromJust(y)

parseInts :: Bs.ByteString -> [Int]
parseInts bstr = map takeJust (map Bs.readInt (Bs.words bstr))

makeTree::[[Int]]->[Tree Int]
makeTree lst = let
    nodelist = makeNodelist nodelist lst 1
    ran::[Int]->[Tree Int]->[Tree Int]
    ran [] _ = []
    ran (x:xs) nodelist' = let m = nodelist'!!(x-1)
                           in m:(ran xs nodelist')
    makeNodelist::[Tree Int]->[[Int]]->Int->[Tree Int]
    makeNodelist _ [] _ = []
    makeNodelist nodelist' (y':ys') id = 
            let nod = Node id (ran y' nodelist'); s = id + 1
            in nod:(makeNodelist nodelist' ys' s)
    in nodelist

first (x,_,_,_) = x

unfold::[(Int,Int)]->[Int]
unfold [] = []
unfold ((a,b):xs) = (a:b:(unfold xs))

   
dfs_low:: Map Int [Int] -> (Tree (Int,Int,Int,Int), Int, Map Int (Int,Int,Int), Map Int [Int])
dfs_low map = aux 1 (-1) 1 map 1 Map.empty 
  where 
        aux:: Int -> Int -> Int -> Map Int [Int] -> Int -> Map Int (Int,Int,Int) -> (Tree (Int,Int,Int,Int), Int, Map Int (Int,Int,Int), Map Int [Int])
        aux x prev dpth tmap id vlist  = (Node (x,id,low',dpth) ts', id',vlist', tmap')
          --traceShow ("x,id",x,id) 
          where zlist = Map.insert x (low',id,dpth) vlist
                (r,m) = Map.updateLookupWithKey  (\ _ _ -> Nothing ) x tmap
                j = fromJust r;
                inc = id + 1
                dinc = dpth+1
                (ts', id', vlist', tmap') = dpth `seq` inc `seq` auxs x dinc inc j zlist tmap
                low' = if(not (vlist == Map.empty))
                            then let m2 = [let (low'',id'',dpth'') = fromJust m; in if (id''> id) then low'' else dpth'' | a' <- j, m <- ((Map.lookup a' vlist'):[]), ((not (isNothing m)) && (prev/=a') )]
                                 in minimum (dpth:m2)
                            else 1
        auxs:: Int -> Int -> Int -> [Int] ->  Map Int (Int,Int,Int) -> Map Int [Int] -> ([Tree (Int,Int,Int,Int)], Int, Map Int (Int,Int,Int), Map Int [Int])
        auxs _ _ id [] vlist tmap = ([], id, vlist,tmap)
        auxs prev dpth id (t : ts)  vlist tmap= if (Map.member t vlist)
                                                                                        then auxs prev dpth id ts vlist tmap
                                                                                        else let (t',id',vlist',tmap') = aux t prev dpth tmap id vlist ;  (ts',id'',vlist'', tmap'') = auxs prev dpth id' ts vlist' tmap'
                                                                                                    in  (t' : ts', id'',vlist'',tmap'')

ap::(Tree (Int,Int,Int,Int))->[Int]
ap tree = collect tree
    where collect (Node (x,id,low,dpth) ts) = listl
           where lows = [low | Node (_,_,low,_) _<- ts ]
                 lows' = if(lows /= [])
                                    then maximum lows
                                    else (-1)
                 dec = descend ts
                 listl = if(id==1)
                            then if(length (ts) > 1)
                                    then x:dec
                                    else dec
                            else
                                if (lows' >= dpth)
                                    then x:dec
                                    else dec
          descend [] = []
          descend (t:ts) = l1++l2
            where l1 = collect t 
                  l2 = descend ts    

zipy :: [Int]->[(Int,Int)]
zipy [] = []
zipy (a:b:xs) = (a,b):(dup xs) 

dup :: [Int]->[(Int,Int)]
dup [] = []
dup (a:b:xs) = (a,b):(b,a):(dup xs)


h xs [] = xs
h (x:[]) y = x:y

-- fetch input
getInput = do 
    q <- fmap parseInts Bs.getContents
    return q

morph (_:xs)= tomap xs Map.empty

tomap [] map = map
tomap ((a,b):xs) map = map''
                        where map' = Map.insertWith h a (b:[]) map; map'' = tomap (xs) map'
getGraph = do
        u <- getInput
        let n = zipy u
            z = morph n
        return z


articulation_points = do
        grph <- getGraph
        let aps =  ap . first . dfs_low $ grph
        print $ (length aps)

main = articulation_points
