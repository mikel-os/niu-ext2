#include "system/u.h"
#include "niu/ext2/block_group_descriptor.h"
#include <stdint.h>
#include "system/field.h"

int
niu_ext2_get_bgd(struct niu_ext2_bgd *descriptor, uchar const *mem)
{
	int i;

	if(descriptor == nil) return -1;
	if(mem == nil) return -1;

	descriptor->bg_block_bitmap = getfield32(&mem);
	descriptor->bg_inode_bitmap = getfield32(&mem);
	descriptor->bg_inode_table = getfield32(&mem);
	descriptor->bg_free_blocks_count = getfield16(&mem);
	descriptor->bg_free_inodes_count = getfield16(&mem);
	descriptor->bg_used_dirs_count = getfield16(&mem);
	descriptor->bg_pad = getfield16(&mem);

	/* FIXME
	The field is unused, but must be stored for compatibility.
	Every element in the array is considered as a single little-endian
	value, so writing it needs convert it on big-endian platforms.
	*/

	for(i = 0; i < 3; i++){
		descriptor->bg_reserved[i] = getfield32(&mem);
	}

	return 0;
}

