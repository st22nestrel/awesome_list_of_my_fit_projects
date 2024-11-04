{-|
Module  : DataOps
Project Name  : ECDSA
Author  : Timotej Ponek, xponek00
Date    : 31.3.2023
-}

module DataOps (generateKeys, sign, verify) where
import DataTypes
import Data.Bits
import System.Random
import qualified Control.Monad.IO.Class

-- |Generates private and public keys for ECDSA
generateKeys :: Control.Monad.IO.Class.MonadIO m => Curve -> m Key
generateKeys (Curve p a b g n h) = do
  privateKey <- generatePrivateKey n
  let publicKey = pointMultiplication g privateKey a p
  if publicKey == PointInf then
    generateKeys (Curve p a b g n h)
  else
    return (Key privateKey $ PublicKey publicKey)

-- |Generates random private key
generatePrivateKey :: Control.Monad.IO.Class.MonadIO m => Integer -> m Integer
generatePrivateKey n = randomRIO (0, n-1 :: Integer)

-- |Addition of two points P1 + P2
pointAddition :: Point -> Point -> Integer -> Point
pointAddition (Point x1 y1) (Point x2 y2) p =
  let (divident, divisor) = ((y2 - y1) `mod` p, (x2 - x1) `mod` p)
      s = if divident `mod` divisor == 0 
          then (divident `div` divisor) `mod` p
          else (divident * modularMultiplicativeInverse divisor p) `mod` p
      x3 = (s*s - x1 - x2) `mod` p :: Integer
      y3 = (s*(x1 - x3) - y1) `mod` p :: Integer
   in if divisor == 0 then PointInf else Point x3 y3
pointAddition PointInf (Point x2 y2) _ = Point x2 y2
pointAddition (Point x1 y1) PointInf _ = Point x1 y1
pointAddition _ _ _ = PointInf

-- |Multiplication of point by 2
pointDoubling :: Point -> Integer -> Integer -> Point
pointDoubling PointInf _ _ = PointInf
pointDoubling (Point x y) a p =
  let (divident, divisor) = ((3*(x^(2::Integer)) + a) `mod` p, (2*y) `mod` p)
      s = if divident `mod` divisor == 0 
          then (divident `div` divisor) `mod` p
          else (divident * modularMultiplicativeInverse divisor p) `mod` p
      x2 = ((s*s) - (2*x)) `mod` p
      y2 = ((-y) + s*(x - x2)) `mod` p
   in if divisor == 0 then PointInf else Point x2 y2

-- |Multiplication of point by given konstant 'k'
pointMultiplication :: Point -> Integer -> Integer -> Integer -> Point
pointMultiplication point k a p 
  | k == 0 = PointInf
  | otherwise = pointMultiplicationHelper point point k a p

pointMultiplicationHelper :: Point -> Point -> Integer -> Integer -> Integer -> Point
pointMultiplicationHelper r sourcePoint k a p
  | k == 0 || k == 1 = sourcePoint
  | (k .&. 1) == 1 = pointAddition r' sourcePoint p
  | otherwise = r'
    where
      r' = pointDoubling (pointMultiplicationHelper r sourcePoint (shiftR k 1) a p) a p

-- |Adopted from rosetta code https://rosettacode.org/wiki/Modular_inverse#Haskell
-- Given a and m, return x such that ax = 1 mod m.
-- If there is no such x return 0
modularMultiplicativeInverse :: Integer -> Integer -> Integer
modularMultiplicativeInverse a m
  | 1 == g = mkPos i
  | otherwise = error "Modular multiplicative inverse does not exist"
  where
    (i, _, g) = gcdExt a m
    mkPos x
      | x < 0 = x + m
      | otherwise = x

-- |Extended Euclidean algorithm.
-- Given non-negative a and b, return x, y and g
-- such that ax + by = g, where g = gcd(a,b).
-- Note that x or y may be negative.
gcdExt :: Integer -> Integer -> (Integer, Integer, Integer)
gcdExt a 0 = (1, 0, a)
gcdExt a b =
  let (q, r) = a `quotRem` b
      (s, t, g) = gcdExt b r
  in (t, s - q * t, g)

-- |Sign message hash with given keys
sign :: Control.Monad.IO.Class.MonadIO m => Curve -> Key -> Hash -> m Signature
sign curve keys msg = do
    k <- generatePrivateKey (n curve)
    let r = pointMultiplication (g curve) k (a curve) (p curve)
    if r == PointInf then
      sign curve keys msg
    else do
      let rx = x r `mod` n curve
      let s = (hash msg + d keys * rx ) * modularMultiplicativeInverse k (n curve)
      return (Signature rx s)

-- |Verify message signature message hash with given keys
verify :: Monad m => Curve -> Signature -> PublicKey -> Hash -> m Bool
verify curve signature pubKey msg = do
  -- check whether r and s € (1, n-1), else signature is invalid
  if and [r signature > 0, r signature < n curve,
    s signature > 0, s signature < n curve] then do
    let w = modularMultiplicativeInverse (s signature) (n curve) `mod` n curve
    let rRecovered = pointAddition
          (pointMultiplication (g curve) ((hash msg * w) `mod` n curve) (a curve) (p curve))
          (pointMultiplication  (point pubKey) ((r signature * w) `mod` n curve) (a curve) (p curve))
          (p curve)
    return (x rRecovered `mod` n curve == r signature)
  else do
    _ <- error "r and s are not in the interval"
    return False