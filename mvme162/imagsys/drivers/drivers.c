/*
 * $Id$ 
 */

#include "vme162.h"
#include "process.h"
#include "semaphore.h"
#include "drivers.h"
#define print(s) outstr(s,s+strlen(s))

/*
 * file descriptor
 */
fd fdtab[NFD] = {
	//     		mode		device	libre
	/*stdin	*/	I_READ,		0,	FDUSED,
	/*stdout*/	I_WRITE,	0,	FDUSED,
	/*stderr*/	I_WRITE,	1,	FDUSED
	};

/*
 * device descriptor
 */
device devtab[NDEV] = {
	//name  open      close      read      write      ioctl		major	minor
0,	"tty0", open_tty, close_tty, read_tty, write_tty, ioctl_tty, 	0,	0,
1,	"tty1", open_tty, close_tty, read_tty, write_tty, ioctl_tty, 	0,	1,
2,	"/fs/", open_fs , close_fs , read_fs , write_fs , ioctl_fs, 	1,	0
	};

#define isbadfd(f) ( (f < 0) || (f > NFD) || (fdtab[f].libre == FDFREE) )
void init_fdtab()
{
int j;
for (j=3; j<NFD; j++) fdtab[j].libre = FDFREE;
}

/////////////////////////////////////////////////
//high-level I.O.
//mode = "r" ou "w"
//a faire : "a"
int sys_open(char *file, char *mode)
{
int i,j;

//trouver un emplacement libre dans fdtab
for (j=3; j<NFD; j++) if (fdtab[j].libre == FDFREE) break;
fdtab[j].libre = FDUSED;
switch(mode[0]) {
	case 'r' : fdtab[j].mode = I_READ;
		   break;
	case 'w' : fdtab[j].mode = I_WRITE;
		   break;
	default : return -1;
        }
//lier au device correspondant
for (i=0; i<NDEV; i++)
	if (strncmp(devtab[i].name, file,4) == 0) {
      		fdtab[j].device = i;
		(*devtab[i].o)(devtab[i].minor,fdtab[j].mode);
		return j;
		}
return -1;
}

int sys_close(int f)
{
if (isbadfd(f)) return -1;
fdtab[f].libre = FDUSED;
return (*devtab[fdtab[f].device].c)(fdtab[f].device);
}

int sys_read(int f, char *buffer , int n)
{
if (isbadfd(f)) return -1;
if (!(fdtab[f].mode & I_READ)) return -1;
return (*devtab[fdtab[f].device].r)(fdtab[f].device,buffer,n);
}

int sys_write(int f,char *buffer, int n)
{
if (isbadfd(f)) return -1;
if (!(fdtab[f].mode & I_WRITE)) return -1;
return (*devtab[fdtab[f].device].w)(fdtab[f].device,buffer,n);
}

//A REVOIR
int sys_ioctl(int f,int mode)
{
if (isbadfd(f)) return -1;
switch(mode) {
	case I_READ : fdtab[f].mode = (fdtab[f].mode | I_READ) & ~(I_WRITE);
		   break;
	case I_WRITE : fdtab[f].mode = (fdtab[f].mode | I_WRITE) & ~(I_READ);
		   break;
        }
return (*devtab[fdtab[f].device].i)(fdtab[f].device, mode);
}

