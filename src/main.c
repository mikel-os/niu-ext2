#include <limits.h>
#include <time.h>
#include "system/u.h"
#include "niu/ext2/superblock.h"
#include "niu/ext2/block_group_descriptor.h"
#include "system/macros.h"
#include "system/math.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
	ulong i;
	int index;
	int errcode;
	ssize_t bytes;

	struct niu_ext2_superblock block;
	struct niu_ext2_bgd descriptor;
	uchar memory[2048];
	char mtime_str[50];
	char wtime_str[50];
	char lastcheck_str[50];
	char checkinterval_str[50];

	time_t interval;

	struct tm *tm_ptr;
	struct tm epoch;

	/* FIXME
	This code expect read the superblock and the bgdt concatenated.
	*/
	bytes = read(STDIN_FILENO, memory, 2048);
	if(bytes < 0){
		errcode = errno;
		fprintf(stderr, "%s\n", strerror(errcode));
		return errcode;
	}

	if(bytes != 2048){
		fprintf(stderr, "Tamaño de entrada incorrecto "
		"(%zd, %d expected)\n", bytes, 2048);
		return 1;
	}

	errcode = niu_ext2_get_superblock(&block, memory);

	tm_ptr = localtime(&block.s_mtime);
	epoch = *tm_ptr;

	strftime(mtime_str, sizeof(mtime_str), "%a, %d %b %Y %T %z", &epoch);

	tm_ptr = localtime(&block.s_wtime);
	epoch = *tm_ptr;

	strftime(wtime_str, sizeof(wtime_str), "%a, %d %b %Y %T %z", &epoch);

	tm_ptr = localtime(&block.s_lastcheck);
	epoch = *tm_ptr;

	strftime(lastcheck_str, sizeof(lastcheck_str)
	, "%a, %d %b %Y %T %z", &epoch);

	if(block.s_checkinterval == 0){
		interval = block.s_lastcheck + block.s_checkinterval;

		tm_ptr = localtime(&interval);
		epoch = *tm_ptr;

		strftime(checkinterval_str, sizeof(checkinterval_str)
		, "%a, %d %b %Y %T %z", &epoch);
	}

	printf(
	"inodes %lu\n"
	"blocks %lu\n"
	"super user reserved blocks %lu\n"
	"free blocks %lu\n"
	"free inodes %lu\n"
	"first data block %lu\n"
	"log2 block size %lu\n"
	"log2 frag size %ld\n"
	"blocks per group %lu\n"
	"frags per group %lu\n"
	"inodes per group %lu\n"
	"last mount time %s\n"
	"last write time %s\n"

	"Mounted %u times from last fs full check\n"
	"(Max times allowed: %d, negative means \"disabled\")\n"
	"EXT2_SUPER_MAGIC 0xEF53 == %x\n"
	"%u (Valid = 1, Error = 0)\n"
	"%u (On error: Continue = 1, Remnt RO = 2, Panic! = 3)\n"
	"Minor rev. level %u\n"

	"last time checked %s\n"
	"(Next time checking will be required: %s)\n"
	"%lu (0~4 Linux, GNU HURD, MASIX, FreeBSD, Lites)\n"
	"%lu (Revision 0 vs 1, with variable inode sizes, "
		"extended attributes, etc...)\n"

	"Default user id for reserved blocks (0 is default on Linux) %d\n"
	"Default group (...) %d\n",

	block.s_inodes_count,
	block.s_blocks_count,
	block.s_r_blocks_count,
	block.s_free_blocks_count,
	block.s_free_inodes_count,
	block.s_first_data_block,
	block.s_log_block_size,
	block.s_log_frag_size,
	block.s_blocks_per_group,
	block.s_frags_per_group,
	block.s_inodes_per_group,
	mtime_str,
	wtime_str,

	block.s_mnt_count,
	block.s_max_mnt_count,
	block.s_magic,
	block.s_state,
	block.s_errors,
	block.s_minor_rev_level,

	lastcheck_str,
	(block.s_checkinterval != 0)? checkinterval_str: "disabled",
	block.s_creator_os,
	block.s_rev_level,

	block.s_def_resuid,
	block.s_def_resgid);

	if(errcode != 0){
		fprintf(stderr, "No se obtuvo el superbloque: %d\n", errcode);
		return 1;
	}

	ulong const
	bg_count = DIV_ROUNDED_UP(block.s_blocks_count,
	                          block.s_blocks_per_group);
	ulong const
	bg_count_check = DIV_ROUNDED_UP(block.s_inodes_count,
	                                block.s_inodes_per_group);

	if(bg_count != bg_count_check){
		fprintf(stderr, "Número de bloques inconsistente %lu != %lu\n",
		        bg_count, bg_count_check);
		return -1;
	}

	printf("Número de bloques %lu\n", bg_count);

	index = 1024;
	for(i = 0; i < bg_count; i++){
		if(niu_ext2_get_bgd(&descriptor, &memory[index])){
			fprintf(stderr, "Error en el descriptor %lu\n", i);
			return -1;
		}

		index += BGD_SIZE;

		printf("\tGrupo de bloques %lu\n", i);
		printf(
		"Pointer to the blocks bitmap block %lu\n"
		"Pointer to the inodes bitmap block %lu\n"
		"Pointer to the inode table first block %lu\n"
		"Free blocks count %u\n"
		"Free inodes count %u\n"
		"Inodes allocated to directories count %u\n"
		"Padding %u\n"
		"(Unused %lu.%lu.%lu)\n",
		descriptor.bg_block_bitmap,
		descriptor.bg_inode_bitmap,
		descriptor.bg_inode_table,
		descriptor.bg_free_blocks_count,
		descriptor.bg_free_inodes_count,
		descriptor.bg_used_dirs_count,
		descriptor.bg_pad,
		descriptor.bg_reserved[0],
		descriptor.bg_reserved[1],
		descriptor.bg_reserved[2]
		);
	}

	return 0;
}
