BMS project 1 - LDPC coder
author Timotej Ponek, xponek00

Project info:
Project uses only 16x16 (or 16x14) parity check matrix H
for encoding, because:
- it cannot create larger matrix (error in generating of 
generator matrix G, because created H is not always invertible)
- when dealing with variable size input, encoding
of input that is not exactly coresponding to size of matrix
(i.e. lenght of input is not the number of collumns of matrix H),
iput would be trimmed, but with usage of matrix 16x16, nothing
is trimmed

Decoding with matrices larger than 16x16 can be slow (like 
extremly slow)... code was not optimized for usage of 
large matrices

When provided with correct H (left side invertible and right
side identity matrix), encoding should work also with larger 
than 16x16 matrices.