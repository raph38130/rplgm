: star [CHAR] * EMIT ;
: line 0 DO star LOOP ;
: tri1 1 DO I line CR LOOP ;
: tri2 DUP 0 DO DUP I - line CR LOOP ;
: demo ." hello lgm" CR CR ;

: emb 
10 0 DO
CR I DUP ." j=" .
   1+ 0 DO I ." i=" .
   LOOP
LOOP ;

: delay 10000 0 DO LOOP ;
: l0 0 !P5 ;
: l1 4 !P5 ;
: blink l0 delay l1 delay ;
: k2000 0 DO blink LOOP ;

CR 10 tri1
CR 10 tri2
demo
emb




100 k2000

