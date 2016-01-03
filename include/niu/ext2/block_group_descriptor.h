/*	@file niu/ext2/block_group_descriptor.h
	@author Mikel Cazorla Pérez
	@date 2016/01/03
*/

#define BGD_SIZE 32

struct niu_ext2_bgd
{
	ulong	bg_block_bitmap;
	ulong	bg_inode_bitmap;
	ulong	bg_inode_table;
	ushort	bg_free_blocks_count;
	ushort	bg_free_inodes_count;
	ushort	bg_used_dirs_count;
	ushort	bg_pad;
	ulong	bg_reserved[3];
};

/*!	@brief Fills a struct that represent a block group descriptor from one
	located in the memory address space.

	The function does not detect if the source is a block descriptor, is up
	to the caller pass a valid one.

	@param mem The address of a source block group descriptor in memory.
	@param niu_ext2_bgd A pointer to the block group descriptor struct
	expected to be filled.

	@returns On success returns 0.
	@returns On error returns a negative number.
*/

int niu_ext2_get_bgd(struct niu_ext2_bgd *descriptor, uchar const *mem);

