/*
 * RP 04/1998
 */
#define print(s) outstr(s,s+strlen(s))
#include "net.h"
#include "types.h"

//trouve la chaine de caractere sur la pile S ou U
test_print()
{
asm ("
   move.l #0x300000,%a0
   move.l %a0,%usp
   ");
print("mode S");
asm ("  
   move.w #0,%sr
   ");
print("mode U");
}

//ok
void test_load()
{
netrw_blk_16xbug b;
b.ctrl_lun=b.dev_lun=0;
b.max_length=0;//tout le fichier
b.addr=0x200000;
b.offset=0;
strcpy(b.filename,"hello");
netrd_16xbug(&b);
}

main()
{
print("début");
test_load();
print("fin");
}
