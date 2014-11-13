/*
 * $Id$
 */



//modes pour "file descriptor" (défini par la valeur d'un bit)
#define	I_READ	1
#define	I_WRITE	2

//modes pour device "tty" 
#define ECHO	4
#define RAW	5

#define FDUSED	10
#define FDFREE	11

#define Taille_Buffer	256
#define NDEV            3
#define NTTY            2
#define NFD		32


#define CR	0xD
#define LF	0xA
#define BEEP	0x7


typedef 
struct _fd {
	int mode; //I_READ I_WRITE
	int device;
	char libre;
	} fd;
extern fd fdtab[];

#define stdin	0
#define stdout  1
#define stderr  2

typedef
struct device {
	int devnum;
	char *name;
        int (*o)(int,int);
        int (*c)(int);
        int (*r)(int,char*,int);
        int (*w)(int,char*,int);
        int (*i)(int,int);
	int major;	//classe de device
	int minor;	//identifiant du device dans la classe ci-dessus
	} device;

typedef
struct tty {
        int port;
	char *idata;
	char *odata;
	int iwrite,	//prochain caractère à afficher
	    iread;	//dernier car lu
	SEMAPHORE *full,
		  *empty;
        char echo;	//echo demandé ?
	char dispo;	//mais est-ce possible ?
        } _tty;

extern device devtab[];
extern _tty tty[];

extern int open_tty();
extern int close_tty();
extern int read_tty();
extern int write_tty();
extern int ioctl_tty();
extern int open_fs();
extern int close_fs();
extern int read_fs();
extern int write_fs();
extern int ioctl_fs();
