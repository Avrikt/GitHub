#define _XOPEN_SOURCE 500 //pread()
#include <stdio.h> //printf
#include <fcntl.h> //O_RDONLY
#include <ext2fs/ext2_fs.h> //struct ext2_super_block
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define R_DEVICE "/dev/ram0"
#define ROOT (2)
#define BOOT_OFFSET (1024)
#define BLOCK_OFFSET(block) (block * block_size)
#define BLOCK_BUFFER (4096)

static int OpenFD(void);
static void PrintSuperBlock(void);
static void PrintGroupDesc(void);
static void PrintInode(int inode_no);
static int ReadDir(const char *file);
void ReadFile(void);

unsigned int block_size = 0;
int file_ds = 0;
struct ext2_super_block sb;
struct ext2_group_desc gd;
struct ext2_inode ind;

int main(int argc, char *argv[])
{
    int ind_num = 0;

    if(argc < 2)
    {
        printf("Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    if(0 != OpenFD())
    {
        return 1;
    }

    PrintSuperBlock();
    PrintGroupDesc();
    PrintInode(ROOT);
    ind_num = ReadDir(argv[1]);

    PrintInode(ind_num);
    ReadFile();
    close(file_ds);
    return 0;
}

static int OpenFD(void)
{
    if(0 > (file_ds = open(R_DEVICE, O_RDONLY)))
    {
        perror("open() error\n");
        return 1;
    }

    return 0;
}

static void PrintSuperBlock(void)
{
    pread(file_ds, &sb, sizeof(sb), BOOT_OFFSET);

    if(EXT2_SUPER_MAGIC != sb.s_magic)
    {
        perror("Not Ext2 FS\n");
        exit(1);
    }

    block_size = BOOT_OFFSET << sb.s_log_block_size;
    
    printf("Reading super-block from device " R_DEVICE ":\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
	       sb.s_inodes_count,  
	       sb.s_blocks_count,
	       sb.s_r_blocks_count,     /* reserved blocks count */
	       sb.s_free_blocks_count,
	       sb.s_free_inodes_count,
	       sb.s_first_data_block,
	       block_size,
	       sb.s_blocks_per_group,
	       sb.s_inodes_per_group,
	       sb.s_creator_os,
	       sb.s_first_ino,          /* first non-reserved inode */
	       sb.s_inode_size);
}

static void PrintGroupDesc(void)
{
    pread(file_ds, &gd, sizeof(gd), block_size);

    printf("\nReading first group-descriptor from device " R_DEVICE ":\n"
        "Blocks bitmap block: %u\n"
        "Inodes bitmap block: %u\n"
        "Inodes table block : %u\n"
        "Free blocks count  : %u\n"
        "Free inodes count  : %u\n"
        "Directories count  : %u\n"
        ,
        gd.bg_block_bitmap,
        gd.bg_inode_bitmap,
        gd.bg_inode_table,
        gd.bg_free_blocks_count,
        gd.bg_free_inodes_count,
        gd.bg_used_dirs_count);    /* directories count */
}

static void PrintInode(int inode_no)
{
    pread(file_ds, &ind, sizeof(ind), BLOCK_OFFSET(gd.bg_inode_table) + ((inode_no - 1)*(sb.s_inode_size)));

    printf("\nReading inode\n"
           "File mode: %hu\n"
           "Owner UID: %hu\n"
           "Size     : %u bytes\n"
           "Blocks   : %u\n"
           ,
           ind.i_mode,
           ind.i_uid,
           ind.i_size,
           ind.i_blocks);
}

static int ReadDir(const char* file_name)
{
    void *block = malloc(block_size);
    unsigned int size = 0;
    char buffer[EXT2_NAME_LEN+1] = {0};
    int inode_num = 0;

    if(NULL == block)
    {
        perror("Error allocating memory\n");
        exit(1);
    }
    if(S_ISDIR(ind.i_mode))
    {
        struct ext2_dir_entry_2 *entry = NULL;
        pread(file_ds, block, block_size, BLOCK_OFFSET(ind.i_block[0]));

        for(entry = (struct ext2_dir_entry_2 *)block;
                     size < ind.i_size && entry->inode;
                     entry = (void*) entry + entry->rec_len)
        {
            memcpy(buffer, entry->name, entry->name_len);
            buffer[entry->name_len] = '\0';
            printf("%10u %s\n", entry->inode, buffer);
            size += entry->rec_len;
            if(0 == strcmp(file_name, buffer))
            {
                inode_num = entry->inode;
                break;
            }
        }
    }

    free(block);

    return inode_num;
}

void ReadFile(void)
{
    char buffer[BLOCK_BUFFER] = {0};

    pread(file_ds, buffer, block_size, BLOCK_OFFSET(ind.i_block[0]));
    printf("\n\x1B[1;32m%s\x1B[0m\n", buffer);
}