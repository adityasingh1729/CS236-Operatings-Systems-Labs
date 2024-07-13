#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename)
{
	for (int i = 0; i < NUM_INODES; i++) {
		struct inode_t inode;
		simplefs_readInode(i, &inode);
		if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
			return -1;
		}
	}
	struct inode_t inode;
	int ret = simplefs_allocInode();
	if (ret == -1) {
		return -1;
	}
	simplefs_readInode(ret, &inode);
	inode.file_size = 0;
	memset(inode.name, 0, MAX_FILE_SIZE); // I FORGOT THIS
	strcpy(inode.name, filename);
	inode.status = INODE_IN_USE;
	simplefs_writeInode(ret, &inode);   // I forgot this: It is required to push the changes to the disk
	return ret;
}


void simplefs_delete(char *filename)
{
	int inodeNum = -1;
	for (int i = 0; i < NUM_INODES; i++) {
		struct inode_t inode;
		simplefs_readInode(i, &inode);
		if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
			inodeNum = i;
			break;
		}
	}
	if (inodeNum == -1) {
		return;
	}
	struct inode_t inode;
	simplefs_readInode(inodeNum, &inode);
	int datablocks[MAX_FILE_SIZE] = inode.direct_blocks;   // Another way int *datablocks
	for (int i = 0; i < MAX_FILE_SIZE; i++) {
		if (datablocks[i] != -1) {
			simplefs_freeDataBlock(datablocks[i]); // better simplefs_freeDataBlock(inode.direct_blocks[i])
		}
	}

	// I did this: These are not required -> simplefs_freeInode(inodeNum) is enough
	// inode.file_size = 0;
	// inode.status = INODE_FREE;
	simplefs_freeInode(inodeNum);
}

int simplefs_open(char *filename)
{
	int inodeNum = -1;
	for (int i = 0; i < NUM_INODES; i++) {
		struct inode_t inode;
		simplefs_readInode(i, &inode);
		if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
			inodeNum = i;
			break;
		}
	}
	if (inodeNum == -1) {
		return;
	}
	struct inode_t inode;
	simplefs_readInode(inodeNum, &inode);

	int file_handle = -1;
	for (int i = 0; i < MAX_OPEN_FILES; i++) {
		if (file_handle_array[i].inode_number < 0) {
			file_handle = i;
			break;
		}
	}

	if (file_handle == -1) {
		return -1;
	}
	
	file_handle_array[file_handle].inode_number = inodeNum;
	file_handle_array[file_handle].offset = 0;

	return file_handle;
}

void simplefs_close(int file_handle)
{
	// This check is not required
	if (file_handle_array[file_handle].inode_number < 0) {
		return;
	}
	file_handle_array[file_handle].inode_number = -1;
	file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes)
{
	int inodeNum = file_handle_array[file_handle].inode_number;
	int offset = file_handle_array[file_handle].offset;
	struct inode_t inode;
	simplefs_readInode(inodeNum, &inode);

	if (inodeNum < 0 || offset + nbytes > inode.file_size) {
		return -1;
	}
	if (inode.status = INODE_FREE) {
		return -1;
	}
	if (nbytes < 0) {
		return -1;
	}
	int num_blocks = 0;
	for (int i = 0; i < MAX_FILE_SIZE; i++) {
		if (inode.direct_blocks[i] != -1) {
			num_blocks++;
		}
	}
	
	int block = offset / BLOCKSIZE;
	int offset_in_block = offset % BLOCKSIZE;
	int bytes_left_to_read = nbytes;

	while (block < num_blocks && bytes_left_to_read > 0) {
		char block_data[BLOCKSIZE];
		simplefs_readDataBlock(inode.direct_blocks[block], block_data);
		if (offset_in_block + bytes_left_to_read > BLOCKSIZE) {
			memcpy(buf + (nbytes - bytes_left_to_read), block_data + offset_in_block, BLOCKSIZE - offset_in_block);
			block++;
			bytes_left_to_read -= (BLOCKSIZE - offset_in_block);
			offset_in_block = 0;
		} else {
			memcpy(buf + (nbytes - bytes_left_to_read), block_data + offset, bytes_left_to_read);
			bytes_left_to_read = 0;
			break;
		}
	}

    return 0;
}


int simplefs_write(int file_handle, char *buf, int nbytes)
{
	int inodeNum = file_handle_array[file_handle].inode_number;
	int offset = file_handle_array[file_handle].offset;

	if (inodeNum < 0) {
		return -1;
	}
	
	struct inode_t inode;
	simplefs_readInode(inodeNum, &inode);
	if (inode.status == INODE_FREE) {
		return -1;
	}
	if (nbytes < 0) {
		return -1;
	}
	if (nbytes + offset > MAX_FILE_SIZE * BLOCKSIZE) {
		return -1;
	}
	int num_blocks = 0;
	for (int i = 0; i < MAX_FILE_SIZE; i++) {
		if (inode.direct_blocks[i] != -1) {
			num_blocks++;
		}
	}

	if ((offset % BLOCKSIZE == 0) && (offset / BLOCKSIZE < MAX_FILE_SIZE) && (offset == inode.file_size) && (inode.direct_blocks[offset/BLOCKSIZE] == -1)) {
		inode.direct_blocks[num_blocks] = simplefs_allocDataBlock();
		if (inode.direct_blocks[num_blocks] == -1) {
			return -1;
		}
		char nulldata[BLOCKSIZE];
		memset(nulldata, 0, BLOCKSIZE);
		simplefs_writeDataBlock(inode.direct_blocks[num_blocks], nulldata);
		num_blocks++;
	}

	int block = offset / BLOCKSIZE;
	int offset_in_block = offset % BLOCKSIZE;
	int nbytes_avail = (num_blocks - (block + 1)) * BLOCKSIZE + (BLOCKSIZE - offset_in_block);
	int nbytes_req = (nbytes - nbytes_avail > 0) ? nbytes - nbytes_avail : 0;
	int nblocks_req = nbytes_req / BLOCKSIZE + (nbytes_req % BLOCKSIZE ? 1 : 0);
	int new_blocks[nblocks_req];

	for (int i = 0; i < nblocks_req; i++) {
		new_blocks[i] = -1;
	}

	for (int i = 0; i < nblocks_req; i++) {
		new_blocks[i] = simplefs_allocDataBlock();
		if (new_blocks[i] < 0) {
			goto bad;
		}
	}
	goto good;

bad:
	for (int i = 0; i < nblocks_req; i++) {
		if (new_blocks[i] >= 0) {
			simplefs_freeDataBlock(new_blocks[i]);
		}
	}
	return -1;

good:
	for (int i = 0; i < nblocks_req; i++) {
		char nulldata[BLOCKSIZE];
		memset(nulldata, 0, BLOCKSIZE);
		simplefs_writeDataBlock(new_blocks[i], nulldata);
	}

	assert(nblocks_req + num_blocks < MAX_FILE_SIZE);

	for (int i = num_blocks; i < nblocks_req + num_blocks; i++) {
		inode.direct_blocks[i] = new_blocks[i - num_blocks];
	}
	int bytes_left_to_write = nbytes;

	while ((block < MAX_FILE_SIZE) && (bytes_left_to_write > 0)) {
		char block_data[BLOCKSIZE];
		simplefs_readDataBlock(inode.direct_blocks[block], block_data);
		if (offset_in_block + bytes_left_to_write > BLOCKSIZE) {
			memcpy(buf + (nbytes - bytes_left_to_write), offset_in_block + block_data, BLOCKSIZE - offset_in_block);
			simplefs_writeDataBlock(block, block_data);
			block++;
			bytes_left_to_write -= (BLOCKSIZE - offset_in_block);
			offset_in_block = 0;
		} else {
			memcpy(buf + (nbytes - bytes_left_to_write), offset_in_block + block_data, bytes_left_to_write);
			simplefs_writeDataBlock(block, block_data);
			bytes_left_to_write = 0;
			break;
		}
	}
	inode.file_size = (offset + nbytes > inode.file_size) ? (offset + nbytes) : inode.file_size;
	simplefs_writeInode(inodeNum, &inode);
    return 0;
}


int simplefs_seek(int file_handle, int nseek)
{
	if (file_handle_array[file_handle].inode_number < 0) {
		return -1;
	}

	// I missed a check: We also need to check if the corresponding inode is in use or not

	struct inode_t inode;
	simplefs_readInode(file_handle_array[file_handle].inode_number, &inode);
	if (inode.status == INODE_FREE) {
		return -1;
	}

	int offset = file_handle_array[file_handle].offset;
	int newOffset = 0;
	if (nseek >= 0) {
		// Here it is not MAX_FILE_SIZE instead it is inode.file_size
		if (nseek + offset > inode.file_size) {
			newOffset = inode.file_size;
		} else {
			newOffset = nseek + offset;
		}
	} else {
		if (offset + nseek < 0) {
			newOffset = 0;
		} else {
			newOffset = offset + nseek;
		}
	}
	file_handle_array[file_handle].offset = newOffset;
    return 0;
}