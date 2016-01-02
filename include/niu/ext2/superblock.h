/*!	@file niu/ext2/superblock.h
	@author Mikel Cazorla Pérez
	@date 2015/12/22
*/

/* TODO: These defines and typedefs musn't be here. */
/* TODO: Erase comment when as needed */

/*!	@struct niu_ext2_superblock

	This structure can be padded out by the compiler.
	It's here to facilitate the task and reduce the likelihood of
	errors, must first be filled entirelly, then used, and finally
	flushed again to the format.

	Don't forget the superblock is little-endian.
	When "long" is used stores a 4-byte value.
	When "short" is used stores a 2-byte value.
*/

struct niu_ext2_superblock
{
	ulong	s_inodes_count;
	ulong	s_blocks_count;
	ulong	s_r_blocks_count;
	ulong	s_free_blocks_count;
	ulong	s_free_inodes_count;
	ulong	s_first_data_block;
	ulong	s_log_block_size;
	long	s_log_frag_size;
	ulong	s_blocks_per_group;
	ulong	s_frags_per_group;
	ulong	s_inodes_per_group;
	time_t	s_mtime; /* 4 bytes */
	time_t	s_wtime; /* 4 bytes */
	ushort	s_mnt_count;
	short	s_max_mnt_count;
	ushort	s_magic;
	ushort	s_state;
	ushort	s_errors;
	ushort	s_minor_rev_level;
	time_t	s_lastcheck; /* 4 bytes */
	time_t	s_checkinterval; /* 4 bytes */
	ulong	s_creator_os;
	ulong	s_rev_level;
	short	s_def_resuid;
	short	s_def_resgid;
};

/*!	@brief Fills a struct that represents a superblock from one located
	in the memory address space.

	Note the struct can be modified even on error.

	@param mem The address of a source superblock in memory.
	@param niu_ext2_superblock A pointer to the superblock struct
	expected to be filled.

	@returns On success returns 0.

	@returns
	-1: Invalid arguments.<br/>
	-2: Ext2 block structure not identified.
*/

int niu_ext2_get_superblock(struct niu_ext2_superblock *, uchar const *mem);

/*!	@brief Check if the superblock struct is well formed.

	"Well formed" don't means "reliable".

	@returns On success returns 0.
	@returns On error returns a negative number.
*/

int niu_ext2_check_superblock(struct niu_ext2_superblock const *);

