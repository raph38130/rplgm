/*
 * RP 04 / 1998
 *
 * PB : comment passer une adresse de la pile U à OUTSTR
 */
#define print(s) outstr(s,s+strlen(s))

void msg_S()
{
print("mode S OK");
}
//ici on est en mode S
void msg_U()
{
char *s;

//recupérer sur USP l'adresse
asm volatile("move.l %%usp,%%a0;move.l (%%a0),%0" : "=a" (s) : : "a0" );
print(s);
}
void test_S() 
{
int i;
for(i=0;i<5;i++) ;asm("trap #4");
}

//ici on passe par la pile USP l'adresse de la chaine à afficher
char *msg[] = {"HELLO", "Folks", "enfin", "CA", "marche"};
void test_U()
{
int i;

for(i=0;i<5;i++) 
asm volatile("move.l %0,-(%%sp);trap #5;addq.l #4,%%sp"
		: :
		"a" (msg[i]) : "memory");
}


main()
{
SetHandler(VBR+36*4,msg_S);//trap 4
SetHandler(VBR+37*4,msg_U);//trap 5


test_S();
//passer en mode U
asm("
   move.l #0x300000,%a0
   move.l %a0,%usp
   move.w #0,%sr
   ");
   
test_U();
//il faudrait restorer le mode S pour terminer correctement
}

