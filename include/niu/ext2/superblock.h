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
*/

struct niu_ext2_superblock
{
	atleast4 s_inodes_count;
	atleast4 s_blocks_count;
	atleast4 s_r_blocks_count;
	atleast4 s_free_blocks_count;
	atleast4 s_free_inodes_count;
	atleast4 s_first_data_block;
	atleast4 s_log_block_size;
	atleast4 s_log_frag_size;
	atleast4 s_blocks_per_group;
	atleast4 s_frags_per_group;
	atleast4 s_inodes_per_group;
	time_t /* atleast4 */ s_mtime;
	time_t /* atleast4 */ s_wtime;
	ushort   s_mnt_count;
	ushort   s_max_mnt_count;
	ushort   s_magic;
	ushort   s_state;
	ushort   s_errors;
	ushort   s_minor_rev_level;
	time_t /* atleast4 */ s_lastcheck;
	time_t /* atleast4 */ s_checkinterval;
	atleast4 s_creator_os;
	atleast4 s_rev_level;
	ushort   s_def_resuid;
	ushort   s_def_resgid;
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

