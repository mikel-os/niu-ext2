#include <limits.h>
#include <time.h>
#include "system/u.h"
#include "niu/ext2/types.h"
#include "niu/ext2/superblock.h"
#include "system/macros.h"
#include <stdint.h>
#include <endian.h>
#include <string.h>

//#include <stdio.h>

static
uint16_t
getfield16(uchar **mem)
{
	uint16_t field;

	memcpy(&field, *mem, 2);
	*mem += BIT16SZ;

	return htole16(field);
}

static
uint32_t
getfield32(uchar **mem)
{
	uint32_t field;

	memcpy(&field, *mem, 4);
	*mem += BIT32SZ;

	return htole32(field);
}

int
niu_ext2_get_superblock(struct niu_ext2_superblock *block, uchar *mem)
{
	if(block == nil) return 1;
	if(mem == nil) return 1;

	block->s_inodes_count = getfield32(&mem);
	block->s_blocks_count = getfield32(&mem);
	block->s_r_blocks_count = getfield32(&mem);
	block->s_free_blocks_count = getfield32(&mem);
	block->s_free_inodes_count = getfield32(&mem);
	block->s_first_data_block = getfield32(&mem);
	block->s_log_block_size = getfield32(&mem);
	block->s_log_frag_size = getfield32(&mem);
	block->s_blocks_per_group = getfield32(&mem);
	block->s_frags_per_group = getfield32(&mem);
	block->s_inodes_per_group = getfield32(&mem);
	block->s_mtime = getfield32(&mem);
	block->s_wtime = getfield32(&mem);

	block->s_mnt_count = getfield16(&mem);
	block->s_max_mnt_count = getfield16(&mem);
	block->s_magic = getfield16(&mem);
	block->s_state = getfield16(&mem);
	block->s_errors = getfield16(&mem);
	block->s_minor_rev_level = getfield16(&mem);

	block->s_lastcheck = getfield32(&mem);
	block->s_checkinterval = getfield32(&mem);
	block->s_creator_os = getfield32(&mem);
	block->s_rev_level = getfield32(&mem);

	block->s_def_resuid = getfield16(&mem);
	block->s_def_resgid = getfield16(&mem);

	return 1;
}
