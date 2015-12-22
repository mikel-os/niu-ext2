#include <limits.h>
#include <time.h>
#include "system/u.h"
#include "niu/ext2/types.h"
#include "niu/ext2/superblock.h"
#include "system/macros.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int errcode;
	ssize_t bytes;

	struct niu_ext2_superblock block;
	uchar memory[1024];
	char mtime_str[50];
	char wtime_str[50];
	char lastcheck_str[50];
	char checkinterval_str[50];

	time_t interval;

	struct tm *tm_ptr;
	struct tm epoch;

	bytes = read(STDIN_FILENO, memory, 1024);
	if(bytes < 0){
		errcode = errno;
		fprintf(stderr, "%s\n", strerror(errcode));
		return errcode;
	}

	if(bytes != 1024){
		fprintf(stderr, "Tamaño de entrada incorrecto "
		"(%zd, %d expected)\n", bytes, 1024);
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

	interval = block.s_lastcheck + (time_t) 42686148;

	tm_ptr = localtime(&interval);
	epoch = *tm_ptr;

	strftime(checkinterval_str, sizeof(checkinterval_str)
	, "%a, %d %b %Y %T %z", &epoch);

	printf(
	"inodes %d\n"
	"blocks %d\n"
	"super user reserved blocks %d\n"
	"free blocks %d\n"
	"free inodes %d\n"
	"first data block %d\n"
	"log2 block size %d\n"
	"log2 frag size %d\n"
	"blocks per group %d\n"
	"frags per group %d\n"
	"inodes per group %d\n"
	"last mount time %s\n"
	"last write time %s\n"

	"Mounted %d times from last fs full check\n"
	"(Max times allowed: %d)\n"
	"EXT2_SUPER_MAGIC 0xEF53 == %x\n"
	"%d (Valid = 1, Error = 0)\n"
	"%d (On error: Continue = 1, Remnt RO = 2, Panic! = 3)\n"
	"Minor rev. level %d\n"

	"last time checked %s\n"
	"(Next time checking will be required: %s)\n"
	"%d (0~4 Linux, GNU HURD, MASIX, FreeBSD, Lites)\n"
	"%d (Revision 0 vs 1, with variable inode sizes, "
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
	checkinterval_str,
	block.s_creator_os,
	block.s_rev_level,

	block.s_def_resuid,
	block.s_def_resgid);

	if(errcode != 0){
		fprintf(stderr, "No se obtuvo el superbloque\n");
		return 1;
	}

	return 0;
}
