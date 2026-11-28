module Main where

import Parser (Formula(..), parseFormula)
import Tableau (Tableau(..), buildTableau, prettyTableau)

-- proverava da li je tablo potpuno zatvoren (svaka grana zatvorena)
isTableauClosed :: Tableau -> Bool
isTableauClosed (Closed _ _)     = True
isTableauClosed (Open _)         = False
isTableauClosed (AlphaNode _ t)  = isTableauClosed t
isTableauClosed (BetaNode _ l r) = isTableauClosed l && isTableauClosed r

-- ispituje da li je formula tautologija preko refutacionog tabloa za ~phi
checkTautology :: Formula -> (Bool, Tableau)
checkTautology phi = let tab = buildTableau [Not phi] in (isTableauClosed tab, tab)

main :: IO ()
main = do 
    let sampleInput = "((p1 | ~p2) & p2) -> p1"
    case parseFormula sampleInput of
        Left err -> putStrLn ("Error parsing formula: " ++ err)
        Right phi -> do
            let (isTaut, tab) = checkTautology phi
            putStrLn ("Input formula: " ++ show phi)
            putStrLn ("Tautology: " ++ show isTaut)
            putStrLn ("Tableau:")
            putStr (prettyTableau tab)