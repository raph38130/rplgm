/*
 * chargement par tftp (primitive 16xBug)
 */

#include "net.h"
#include "types.h"

//renvoie ... en cas d'erreur
int load(char*adr, char*file)
{
netrw_blk_16xbug b;

b.ctrl_lun=b.dev_lun=0;
b.max_length=0;b.offset=0;//tout le fichier
b.addr=adr;
strcpy(b.filename,file);
return netrd_16xbug(&b);
}
