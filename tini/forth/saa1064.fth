( display data on saa1064 seven seg led )
: half 10 0 DO LOOP ;

: i2c-start 
( with SCL highchange SDA from 1 to 0 )
1 >SDAx half SCLx-1 half 
0 >SDAx half SCLx-0 ;

: i2c-stop 
( with SCL highchange SDA from 0 to 1 )
0 >SDAx half SCLx-1 half 
1 >SDAx half ;

: i2c-rx-bit 
1 >SDAx half SCLx-1
half SDAx> SCLx-0 ;

: i2c-tx-bit 
0 <> >SDAx half SCLx-1
half SCLx-0 ;

: i2c-tx 
8 0 DO DUP 128 AND i2c-tx-bit 2* LOOP 
DROP i2c-rx-bit DROP ;

: i2c-rx    ( nak -- b )
0 8 0 DO 2* i2c-rx-bit + LOOP 
SWAP i2c-tx-bit ;

HEX
( saa1064 addr 0x72 )
( reg : 00 crtl : D1D2D3D4 )
: saa ( d4 d3 d2 d1 -- )
i2c-start
72 i2c-tx
0  i2c-tx 
27 i2c-tx 
i2c-tx 
i2c-tx 
i2c-tx 
i2c-tx 
i2c-stop ;

VARIABLE digits
IHERE digits !
 3F IC,
 06 IC,
 5B IC,
 4F IC,
 66 IC,
 6D IC,
 7D IC,
 07 IC,
 7F IC,
 6F IC,

DECIMAL
: div 
10 /MOD SWAP 
digits @ + IC@ 
SWAP 
; 

: delay 
10000 0 DO 
1 0 DO LOOP LOOP ;

: counter
2500 0 DO 
I  div div div div
." i=" I . CR DROP saa
delay
LOOP ;

counter






