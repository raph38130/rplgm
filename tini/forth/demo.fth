: half 10 0 DO LOOP ;

: i2c-start 
( with SCL highchange SDA from 1 to 0 )
1 >SDA half SCL-1 half 
0 >SDA half SCL-0 ;

: i2c-stop 
( with SCL highchange SDA from 0 to 1 )
0 >SDA half SCL-1 half 1 >SDA half ;

: i2c-rx-bit 
1 >SDA half SCL-1 half SDA> SCL-0 ;

: i2c-tx-bit 
0 <> >SDA half SCL-1 half SCL-0 ;

: i2c-tx 

8 0 DO DUP 128 AND i2c-tx-bit 2* LOOP 
DROP i2c-rx-bit ;

: i2c-rx    ( nak -- b )
0 8 0 DO 2* i2c-rx-bit + LOOP 
SWAP i2c-tx-bit ;

: device 
( addr -- common i2c preamble)
i2c-start 208 i2c-tx i2c-tx  ;

: device! 
( v addr -- write v to i2c register addr)
device i2c-tx i2c-stop ;

: device@ 
( addr -- v  read i2c register addr)
( 209/208 ds1672 clock)
device i2c-start 209 i2c-tx 
1 i2c-rx i2c-stop ;

: clock CR 
250 0 DO 13 EMIT ." seconds="
0 device@ 10000 0 DO LOOP 
. DROP DROP DROP
LOOP ;

clock


