#include "types.h"

typedef struct netrw_blk_16xbug {
    u8  ctrl_lun;
    u8  dev_lun;
    u16 status;
    u32 addr;
    u32 max_length;
    u32 offset;
    u32 time;
    u32 byte_count;
    net_filename filename;
} netrw_blk_16xbug;

extern int      netrd_16xbug(netrw_blk_16xbug *blk);
extern int      netwr_16xbug(netrw_blk_16xbug *blk);

