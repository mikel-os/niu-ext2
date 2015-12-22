/* TODO: These defines and typedefs musn't be here. */
/* TODO: Erase comment when as needed */

/* This structure can be padded out by the compiler. It's here to
   facilitate the task and reduce the likelihood of errors, must first be
   filled entirelly, then used, and finally flushed again to the format.

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

int niu_ext2_get_superblock(struct niu_ext2_superblock *, uchar *mem);

