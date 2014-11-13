#define MAX 	100

void writeln(int f, char*s)
{
char b[MAX];

if (strlen(s)>(MAX-2)) return;
strcpy(b,s);
strcat(b,"\012\015");
write(f,b,0);
}
