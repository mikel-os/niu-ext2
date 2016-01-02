#include <limits.h>
#include <time.h>
#include "system/u.h"
#include "niu/ext2/superblock.h"
#include <stdint.h>
#include "system/field.h"

//#include <stdio.h>

int
niu_ext2_get_superblock(struct niu_ext2_superblock *block, uchar const *mem)
{
	if(block == nil) return -1;
	if(mem == nil) return -1;

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

	if(niu_ext2_check_superblock(block)) return -2;

	return 0;
}

int
niu_ext2_check_superblock(struct niu_ext2_superblock const *block)
{
	if(block == nil) return -1;

	/* TODO Don't write the magic number directly as a literal */
	if(block->s_magic != 0xEF53) return -1;
	/* inodes count depends on block groups */
	/* the same for blocks count */
	if(block->s_free_blocks_count < block->s_r_blocks_count) return -1;
	if(block->s_log_block_size == 0) return -1;
	if(block->s_log_block_size == 1){
		if(block->s_first_data_block != 1) return -1;
	}else{
		if(block->s_first_data_block != 0) return -1;
	}

	/* While fragment is not supported both numbers must be equal */
	if(block->s_log_frag_size < 0 || block->s_log_frag_size > INT_MAX){
		return -1;
	}
	uint frag_size = block->s_log_frag_size;
	if(frag_size != block->s_log_block_size) return -1;
	/* FIXME Check out overflow */
	/* FIXME Assumes inode size */
	uint const
	bits_per_block = (1024 << block->s_log_block_size) * 8;
	uint const
	inodes_per_block = (1024 << block->s_log_block_size) / 128;

	if(block->s_inodes_per_group > bits_per_block){
		return -1;
	}

	if(block->s_inodes_per_group % inodes_per_block != 0) return -1;

	/*
	Check if s_mnt_count < s_max_mnt_count have nothing to do with check
	the superblock integrity.
	Anyway, check if s_mnt_count > s_max_mnt_count can be considered so.
	But that issue is not handled here.
	Have in mind s_mnt_count range is different.
	*/

	/* TODO Don't write states and error policies directly as literals */
	if(block->s_state != 1) return -1;
	if(block->s_errors > 3) return -1;

	return 0;
}

