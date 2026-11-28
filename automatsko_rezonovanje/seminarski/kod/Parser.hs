module Parser (Formula(..), parseFormula) where

import Data.Char (isDigit)
import Text.ParserCombinators.ReadP

data Formula = TRUE
    | FALSE
    | Var Int
    | Not Formula
    | And Formula Formula
    | Or Formula Formula
    | Imp Formula Formula
    | Iff Formula Formula
    deriving (Eq)

instance Show Formula where
    show TRUE      = "T"
    show FALSE     = "F"
    show (Var n)   = "p" ++ show n
    show (Not f)   = "~" ++ show f
    show (And f g) = "(" ++ show f ++ " & " ++ show g ++ ")"
    show (Or f g)  = "(" ++ show f ++ " | " ++ show g ++ ")"
    show (Imp f g) = "(" ++ show f ++ " -> " ++ show g ++ ")"
    show (Iff f g) = "(" ++ show f ++ " <-> " ++ show g ++ ")"

-- prioritet, od najvišeg ka najnižem: ~, &, |, ->, <->

parseFormula :: String -> Either String Formula
parseFormula input =
    case [formula | (formula, "") <- readP_to_S parser input] of
        []       -> Left ("Invalid propositional formula: " ++ input)
        formulas -> Right (last formulas)
        
    where
        parser = skipSpaces *> iffParser <* skipSpaces <* eof

iffParser :: ReadP Formula
iffParser = chainr1 implicationParser (Iff <$ symbol "<->")

implicationParser :: ReadP Formula
implicationParser = chainr1 disjunctionParser (Imp <$ symbol "->")

disjunctionParser :: ReadP Formula
disjunctionParser = chainl1 conjunctionParser (Or <$ symbol "|")

conjunctionParser :: ReadP Formula
conjunctionParser = chainl1 negationParser (And <$ symbol "&")

negationParser :: ReadP Formula
negationParser = (symbol "~" *> (Not <$> negationParser)) <++ atomicParser

atomicParser :: ReadP Formula
atomicParser = (TRUE <$ symbol "T") 
    <++ (FALSE <$ symbol "F")
    <++ variableParser
    <++ between (symbol "(") (symbol ")") iffParser

variableParser :: ReadP Formula
variableParser = lexeme $ do
    _ <- char 'p'
    digits <- munch1 isDigit
    return (Var (read digits))

symbol :: String -> ReadP String
symbol = lexeme . string

lexeme :: ReadP a -> ReadP a
lexeme p = p <* skipSpaces