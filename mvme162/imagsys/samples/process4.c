//ce test ne marche plus au dela d'une certaine vitesse d'arrivée des
//données
int start()
{
int f1,f2,n=30;
char buff[250];
char buff2[10];

f2=open("tty0","r");
//ioctl(f2,RAW);
f1=open("tty0","w");
while(1) {
	read(f2,buff,0);
	//strcat(p,buff);
	writeln(f1,buff);
	}
return 0;
}
