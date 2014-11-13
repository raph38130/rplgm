/* move.c
 *
 * Mon Mar 13 14:59:17 CET 2006 RP
 *
 * copie d'une zone mémoire à un autre emplacement
 * 	m68k-linux-gcc move.c -o move.s.m68 -S
 *	gcc            move.c -o move.s.x86 -S
 */


void memmove(char* deb, char* fin, char *dest)
{
while (deb < fin) 
	*dest++ = *deb++ ;
} 

int main()
{
memmove((char*)0x15000,(char*)0x15050,(char*)0x20000);

return 0;
}
