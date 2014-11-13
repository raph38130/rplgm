/*
 * $Id$
 */

// I / O
#define READ		01
#define WRITE		02
#define OPEN		03
#define CLOSE		04
#define IOCTL		05

// primitives
#define SUSPEND		11
#define CREATE		12
#define KILL		13
#define RESUME		14
#define GETPID		15
#define GETPRIO		16
#define CHPRIO		17
#define STATUS		18

// time
#define SLEEP		21

// semaphores
#define SCREATE		31
#define SDELETE		32
#define SIGNAL		33
#define WAIT		34

// memory managment
#define MFREE		41
#define MALLOC		42
#define GETFREEMEM	43
#define LOAD		44
