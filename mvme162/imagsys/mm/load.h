/*
 * 
 */
#define NBPROGS 20
#define namelength 20
struct prog {
	char name[namelength];
	char *adr;
	};
struct prog PROGS[NBPROGS];
extern void init_load();
