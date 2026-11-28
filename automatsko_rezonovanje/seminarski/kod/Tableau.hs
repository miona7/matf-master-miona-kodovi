module Tableau 
    ( RuleType(..)
    , Tableau(..)
    , classifyFormula
    , isClosedBranch
    , buildTableau
    , prettyTableau
    ) where

import Data.List (nub)
import Parser (Formula(..))

data RuleType = Alpha Formula Formula
    | Beta Formula Formula
    | LiteralRule
    deriving (Eq, Show)

data Tableau = Closed [Formula] String
    | Open [Formula]
    | AlphaNode Formula Tableau
    | BetaNode Formula Tableau Tableau
    deriving (Eq, Show)

-- Smalijanova klasifikacija nepotpisanih formula na alfa i beta komponente
classifyFormula :: Formula -> RuleType
classifyFormula f = 
    case f of
        Not (Not a)   -> Alpha a a
        And a b       -> Alpha a b
        Not (Or a b)  -> Alpha (Not a) (Not b)
        Not (Imp a b) -> Alpha a (Not b)
        Iff a b       -> Alpha (Imp a b) (Imp b a)
        
        Or a b        -> Beta a b
        Not (And a b) -> Beta (Not a) (Not b)
        Imp a b       -> Beta (Not a) b
        Not (Iff a b) -> Beta (And a (Not b)) (And (Not a) b)

        _             -> LiteralRule

-- proverava da li grana sadrži kontradikciju (FALSE ili istovremeno f i ~f)
findContradiction :: [Formula] -> Maybe String
findContradiction fs
    | FALSE `elem` fs = Just "Contained constant F (FALSE)"
    | otherwise = go fs
    
    where
        go [] = Nothing
        go (x:xs) 
            | Not x `elem` fs = Just ("Contradiction: " ++ show x ++ " and " ++ show (Not x))
            | otherwise = go xs

-- proverava da li je grana zatvorena (sadrži kontradikciju)
isClosedBranch :: [Formula] -> Bool
isClosedBranch fs = 
    case findContradiction fs of
        Just _  -> True
        Nothing -> False

-- izbor prve formule koja se može razložiti po alfa ili beta pravilu
pickDecomposable :: [Formula] -> Maybe (Formula, RuleType, [Formula])
pickDecomposable [] = Nothing
pickDecomposable (f:fs) = 
    case classifyFormula f of
        LiteralRule -> case pickDecomposable fs of
            Nothing               -> Nothing
            Just (g, rule, rest') -> Just (g, rule, f : rest')
        rule -> Just (f, rule, fs)

-- konstrukcija analitičkog tabloa za zadatu listu formula
buildTableau :: [Formula] -> Tableau
buildTableau initialFs = go (nub initialFs)
    where
        go branchFs = 
            case findContradiction branchFs of
                Just reason -> Closed branchFs reason
                Nothing -> case pickDecomposable branchFs of
                    Nothing -> Open branchFs
                    Just (f, Alpha a b, rest) -> AlphaNode f (go (nub (rest ++ [a, b])))
                    Just (f, Beta a b, rest) -> BetaNode f (go (nub (rest ++ [a]))) (go (nub (rest ++ [b])))

-- lep hijerarhijski prikaz stabla tabloa
prettyTableau :: Tableau -> String
prettyTableau = go ""
    where
        go indent (Closed _ reason)       = indent ++ "+-- [CLOSED] " ++ reason
        go indent (Open fs)               = indent ++ "+-- [OPEN] Remaining: " ++ show fs
        go indent (AlphaNode f child)     = indent ++ "[alpha] " ++ show f ++ "\n" ++ go (indent ++ "  ") child
        go indent (BetaNode f left right) = indent ++ "[beta] " ++ show f ++ "\n" ++ go (indent ++ "  ") left ++ "\n" ++ go (indent ++ "  ") right