{-|
Module  : Parser
Project Name  : ECDSA
Author  : Timotej Ponek, xponek00
Date    : 31.3.2023
-}

module Parser (ecInfoParse, sOptParse, vOptParse) where

import DataTypes
import Text.ParserCombinators.Parsec

-- option parsers
-- used for options '-i' and '-k'
ecInfoParse :: String -> Curve
ecInfoParse = \str -> (run_parser ecParser str)
sOptParse :: String -> (Curve, Key, Hash)
sOptParse = \str -> (run_parser sOptParser str)
vOptParse :: String -> (Curve, Signature, PublicKey, Hash)
vOptParse = \str -> (run_parser vOptParser str)

-- |Function that starts execution of parser
run_parser :: Parser a -> String -> a
run_parser p str = case parse p "" str of
  Left err -> error $ "parse error at " ++ (show err)
  Right val -> val

ecParser :: Parser Curve
ecParser = do
  _ <- str_curve
  _ <- lbracket
  p <- read <$> parseP
  a <- read <$> parseA
  b <- read <$> parseB
  g <- pointParser
  n <- read <$> parseN
  h <- read <$> parseH
  _ <- rbracket
  return (Curve {p = p, a = a `mod` p, b = b `mod` p,
    g = Point {x = x g `mod` p, y = y g `mod` p}, n = n, h = h})

sOptParser :: Parser (Curve, Key, Hash)
sOptParser = do
  ec <- ecParser
  key <- keyParser
  hash <- hashParser
  return (ec, key, hash)

vOptParser :: Parser (Curve, Signature, PublicKey, Hash)
vOptParser = do
  ec <- ecParser
  signature <- signatureParser
  pubKey <- publicKeyParser
  hash <- hashParser
  return (ec, signature, pubKey, hash)

keyParser :: Parser Key
keyParser = do
  _ <- str_key
  _ <- lbracket
  d <- read <$> parseD
  q <- parseQ
  _ <- rbracket
  return (Key {d = d, q = q})

publicKeyParser :: Parser PublicKey
publicKeyParser = do
  _ <- spaces
  _ <- str_pubKey
  _ <- lbracket
  q <- parseQ
  _ <- rbracket
  return q

signatureParser :: Parser Signature
signatureParser = do
  _ <- str_signature
  _ <- lbracket
  r <- read <$> parseR
  s <- read <$> parseS
  _ <- rbracket
  return (Signature {r = r, s = s})

hashParser :: Parser Hash
hashParser = do
  spaces
  _ <- string "Hash" <|> string "hash"
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return (Hash $ read val)

pointParser :: Parser Point
pointParser = do
  _ <- parseG
  _ <- str_point
  _ <- lbracket
  x <- read <$> parseX
  y <- read <$> parseY
  _ <- rbracket
  return (Point {x = x, y = y})

str_curve :: Parser String
str_curve = do
  spaces
  string "Curve" <|> string "curve"

str_point :: Parser String
str_point = do
  spaces
  string "Point" <|> string "point"

str_key :: Parser String
str_key = do
  spaces
  string "Key" <|> string "key"

str_signature :: Parser String
str_signature = do
  spaces
  string "Signature" <|> string "signature"

str_pubKey :: Parser String
str_pubKey = do
  spaces
  string "PublicKey" <|> string "publickey"

lbracket :: Parser Char
lbracket = do
  spaces
  oneOf ['{']

rbracket :: Parser Char
rbracket = do
  spaces
  oneOf ['}']

parseP :: Parser [Char]
parseP = do
  spaces
  _ <- oneOf ['p', 'P']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseA :: Parser [Char]
parseA = do
  spaces
  _ <- oneOf ['a', 'A']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseB :: Parser [Char]
parseB = do
  spaces
  _ <- oneOf ['b', 'B']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseG :: Parser Char
parseG = do
  spaces
  _ <- oneOf ['g', 'G']
  oneOf [':']

parseX :: Parser [Char]
parseX = do
  spaces
  _ <- oneOf ['x', 'X']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseY :: Parser [Char]
parseY = do
  spaces
  _ <- oneOf ['y', 'Y']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseN :: Parser [Char]
parseN = do
  spaces
  _ <- oneOf ['n', 'N']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseH :: Parser [Char]
parseH = do
  spaces
  _ <- oneOf ['h', 'H']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseD :: Parser [Char]
parseD = do
  spaces
  _ <- oneOf ['d', 'D']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseQ :: Parser PublicKey
parseQ = do
  spaces
  _ <- oneOf ['q', 'Q']
  _ <- oneOf [':']
  spaces
  _ <- string "0x04"
  val <- many alphaNum 
  let (x, y) = splitAt (length val `div` 2) val
  return (PublicKey (Point {x = read $ '0':'x':x, y = read $ '0':'x':y}))

parseR :: Parser [Char]
parseR = do
  spaces
  _ <- oneOf ['r', 'R']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val

parseS :: Parser [Char]
parseS = do
  spaces
  _ <- oneOf ['s', 'S']
  _ <- oneOf [':']
  spaces
  val <- many alphaNum
  return val