{-|
Module  : DataTypes
Project Name  : ECDSA
Author  : Timotej Ponek, xponek00
Date    : 31.3.2023
-}

module DataTypes (Point (..), Curve (..), Key(..), Hash(..),
PublicKey(..), Signature(..)) where
import Text.Printf

-- |Prints hexadecimal on 64 digits
printHex :: Integer -> String
printHex n = printf "%0.64x" n

data Point = Point
  { x :: Integer,
    y :: Integer
  } | PointInf
  deriving (Eq)

instance Show Point where
  show (Point x y) = "Point {\n\tx: 0x" ++ printHex x ++ "\n\ty: 0x" ++ printHex y ++ "\n}\n"
  show PointInf = "Point at infinity\n"

data Curve = Curve
  { p :: Integer,
    a :: Integer,
    b :: Integer,
    g :: Point,
    n :: Integer,
    h :: Integer
  }
  deriving (Eq)

instance Show Curve where
  show (Curve p a b g n h) = "Curve {\np: 0x" ++ printHex p ++ "\na: " ++ show a ++ "\nb: "
    ++ show b ++ "\ng: " ++ show g ++ "n: 0x" ++ printHex n ++ "\nh: " ++ show h ++ "\n}\n"

data Key = Key
  { d :: Integer,
    q :: PublicKey
  }
  deriving (Eq)

instance Show Key where
  show (Key d (PublicKey q)) = "Key {\n\td: 0x" ++ printHex d ++ "\n\tQ: 0x04"
    ++ printHex (x q) ++ printHex (y q) ++ "\n}\n"

newtype Hash = Hash { hash :: Integer } deriving (Eq)

instance Show Hash where
  show (Hash h) = "Hash: " ++ "0x" ++ printHex h ++ "\n"

newtype PublicKey = PublicKey { point :: Point } deriving (Eq)

instance Show PublicKey where
  show (PublicKey (Point x y)) = "PublicKey: {\nQ: 0x04" ++ printHex x ++ printHex y ++ "\n}\n"
  show (PublicKey _) = "Error\n"

data Signature = Signature
  { r :: Integer,
    s :: Integer
  }
  deriving (Eq)

instance Show Signature where
  show (Signature r s) = "Signature {\n\tr: 0x" ++ printHex r ++ "\n\ts: 0x"
    ++ printHex s ++ "\n}\n"