{-|
Module  : Main
Project Name  : ECDSA
Author  : Timotej Ponek, xponek00
Date    : 31.3.2023
-}

import System.Environment
import System.IO
import Parser
import DataOps

main :: IO ()
main = do
  args <- getArgs
  if length args < 2 then do
    processOption (head args) stdin
  else do
    handle <- openFile (args !! 1) ReadMode
    processOption (head args) handle
    hClose handle

processOption :: String -> Handle -> IO ()
processOption option handle
  | option == "-i" = hGetContents handle >>= putStr . show . ecInfoParse
  | option == "-k" = hGetContents handle >>= generateKeys . ecInfoParse >>= print
  | option == "-s" = do
      contents <- hGetContents handle
      let (ec, keys, hash) = sOptParse contents
      sign ec keys hash >>= print
  | option == "-v" = do
      contents <- hGetContents handle
      let (ec, signature, pubKey, hash) = vOptParse contents
      verify ec signature pubKey hash >>= print
  | otherwise = return() -- bad option, ignore