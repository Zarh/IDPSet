#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/process.h>
#include <sys/prx.h>
#include <sys/string.h>
#include <sys/thread.h>
#include <sys/file.h>
#include <lv2/process.h>
#include <lv2/prx.h>
#include <ppu-lv2.h>
#include <ppu-types.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>
#include <sysmodule/sysmodule.h>
#include <tiny3d.h>
#include <libfont.h>
#include <ft2build.h>
#include <freetype/freetype.h> 
#include <freetype/ftglyph.h>
#include <sys/thread.h>
#include <pngdec/pngdec.h>
#include <sysmodule/sysmodule.h>

#include "pad.h"
#include "ttf_render.h"
#include "libfont2.h"

#include "aes.h"
#include "util.h"
#include "keys.h"
#include "indiv.h"
#include "aes_omac.h"
#include "common.h"
#include "firmware.h"
#include "dumper.h"

#include "payload_421C_bin.h"
#include "payload_421D_bin.h"
#include "payload_430C_bin.h"
#include "payload_430D_bin.h"
#include "payload_431C_bin.h"
#include "payload_440C_bin.h"
#include "payload_441C_bin.h"
#include "payload_441D_bin.h"
#include "payload_446C_bin.h"
#include "payload_446D_bin.h"
#include "payload_450C_bin.h"
#include "payload_450D_bin.h"
#include "payload_453C_bin.h"
#include "payload_453D_bin.h"
#include "payload_455C_bin.h"
#include "payload_455D_bin.h"
#include "payload_460C_bin.h"
#include "payload_465C_bin.h"
#include "payload_465D_bin.h"
#include "payload_470C_bin.h"
#include "payload_470D_bin.h"
#include "payload_475C_bin.h"
#include "payload_475D_bin.h"
#include "payload_476C_bin.h"
#include "payload_476D_bin.h"
#include "payload_478C_bin.h"
#include "payload_478D_bin.h"
#include "payload_480C_bin.h"
#include "payload_480D_bin.h"
#include "payload_481C_bin.h"
#include "payload_481D_bin.h"
#include "payload_482C_bin.h"
#include "payload_482D_bin.h"
#include "payload_483C_bin.h"
#include "payload_484C_bin.h"
#include "payload_484D_bin.h"
#include "payload_485C_bin.h"
#include "payload_485D_bin.h"

#include "NOT_PNG_bin.h"

#define FS_S_IFMT 0170000
#define FS_S_IFDIR 0040000

extern u64 *payload;
extern size_t payload_size;

int position=0;

int prog_bar=-1;

u8 rebug = NO;
u8 debug = NO;

u32 * texture_mem;

pngData ICON0; 
u32 ICON0_offset;

pngData BG; 
u32 BG_offset;

pngData NOT; 
u32 NOT_offset;

static u32 text_size = 0;
u16 * ttf_texture;

static sys_ppu_thread_t loading_id;
static u8 loading = 1;
static char loading_log[10][255];
static char buff[4096];

static char not_msg[255];
static u16 time_not=0;

u32 firmware;

u64 TOC_OFFSET=0;
u64 HV_START_OFFSET=0;
u64 HTAB_OFFSET=0;
u64 SYSCALL_TABLE_OFFSET=0;
u64 MMAP_OFFSET1=0;
u64 MMAP_OFFSET2=0;
u64 SPE_OFFSET=0;
u64 OFFSET_1_IDPS=0;
u64 OFFSET_2_IDPS=0;

u8 IDPS[17];

u8 eid_root_key[EID_ROOT_KEY_SIZE];

void cls()
{
    tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);
    
    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

   // Enable alpha blending.
    tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
        TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
        TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
    reset_ttf_frame();
}

void Init_Graph()
{
    tiny3d_Init(1024*1024);
	u32 * texture_pointer;
	texture_mem = tiny3d_AllocTexture(64*1024*1024); // alloc 64MB of space for textures (this pointer can be global)    
	
	if(!texture_mem) return; 
    texture_pointer = texture_mem;
    ResetFont();
	
	TTFLoadFont(0, "/dev_flash/data/font/SCE-PS3-SR-R-LATIN2.TTF", NULL, 0);
    TTFLoadFont(1, "/dev_flash/data/font/SCE-PS3-DH-R-CGB.TTF", NULL, 0);
    TTFLoadFont(2, "/dev_flash/data/font/SCE-PS3-SR-R-JPN.TTF", NULL, 0);
    TTFLoadFont(3, "/dev_flash/data/font/SCE-PS3-YG-R-KOR.TTF", NULL, 0);
	
    ttf_texture = (u16 *) texture_pointer;
    texture_pointer += 1024 * 16;
    texture_pointer = (u32 *) init_ttf_table((u16 *) texture_pointer);
    text_size = (u32) (u64)(((u8 *) texture_pointer + BIG_PICT * 4096 * 1024 + 1980 * 1080 * 4 + 2048 * 1200 * 4) - ((u8 *) texture_mem));
	
	pngLoadFromFile("/dev_hdd0/game/IDPSET000/USRDIR/BG.PNG", &BG);
	if(BG.bmp_out) {
		memcpy(texture_pointer, BG.bmp_out, BG.pitch * BG.height);
		free(BG.bmp_out);
		BG_offset = tiny3d_TextureOffset(texture_pointer);
		texture_pointer += ((BG.pitch * BG.height + 15) & ~15) / 4;
	}
	
	pngLoadFromFile("/dev_hdd0/game/IDPSET000/ICON0.PNG", &ICON0);
	if(ICON0.bmp_out) {
		memcpy(texture_pointer, ICON0.bmp_out, ICON0.pitch * ICON0.height);
		free(ICON0.bmp_out);
		ICON0_offset = tiny3d_TextureOffset(texture_pointer);
		texture_pointer += ((ICON0.pitch * ICON0.height + 15) & ~15) / 4;
	}
	
	pngLoadFromBuffer(NOT_PNG_bin, NOT_PNG_bin_size, &NOT);
	if(NOT.bmp_out) {
		memcpy(texture_pointer, NOT.bmp_out, NOT.pitch * NOT.height);
		free(NOT.bmp_out);
		NOT_offset = tiny3d_TextureOffset(texture_pointer);
		texture_pointer += ((NOT.pitch * NOT.height + 15) & ~15) / 4;
	}
	
	int videoscale_x = 0;
    int videoscale_y = 0;
	
	double sx = (double) Video_Resolution.width;
    double sy = (double) Video_Resolution.height;
    double psx = (double) (1000 + videoscale_x)/1000.0;
    double psy = (double) (1000 + videoscale_y)/1000.0;
    
    tiny3d_UserViewport(1, 
		(float) ((sx - sx * psx) / 2.0), // 2D position
        (float) ((sy - sy * psy) / 2.0), 
        (float) ((sx * psx) / 848.0),    // 2D scale
        (float) ((sy * psy) / 512.0),
        (float) ((sx / 1920.0) * psx),  // 3D scale
        (float) ((sy / 1080.0) * psy));

}

u64 lv2peek(u64 addr) 
{
    lv2syscall1(6, (u64) addr);
    return_to_user_prog(u64);
}

u64 lv2poke(u64 addr, u64 value) 
{
    lv2syscall2(7, (u64) addr, (u64) value); 
    return_to_user_prog(u64);
}

u8 lv2peek8(u64 addr) {
    return (u8) (lv2peek(addr) >> 56ULL);
}

u64 lv2poke8(u64 addr, u8 value) 
{
    return lv2poke(addr, (((u64) value) <<56) | (lv2peek(addr) & 0xffffffffffffffULL));
}

#define NO_UID		-1
#define SYSTEM_UID	0
#define NO_GID		-1

int sys_fs_chown(char *path, s32 uid, s32 gid)
{
	lv2syscall3(835, (u64) path, (u64) uid, (u64) gid);
	return_to_user_prog(int);
}

int sys_fs_mount(char const* deviceName, char const* deviceFileSystem, char const* devicePath, int writeProt)
{
	lv2syscall8(837, (u64) deviceName, (u64) deviceFileSystem, (u64) devicePath, 0, (u64) writeProt, 0, 0, 0 );
	return_to_user_prog(int);
}

u8 get_units(u8 value)
{
	return value - (value >> 4)*0x10;
}

u8 get_tens(u8  value)
{
	return (value >> 4);
}

u8 SetFilePerms(char *path)
{
	if(sysLv2FsChmod(path, FS_S_IFMT | 0777)==0 && sys_fs_chown(path, NO_UID, NO_GID)==0) return SUCCESS;
	
	return FAILED;
	
}

int CopyFile(char* path, char* path2)
{
    int ret = 0;
    s32 fd = -1;
    s32 fd2 = -1;
    u64 lenght = 0LL;
    
	u64 pos = 0ULL;
    u64 readed = 0;
	u64 writed = 0;
	
	char *mem = NULL;
    
    sysFSStat stat;

    ret=sysLv2FsStat(path, &stat);
    if(ret) goto skip;
    lenght = stat.st_size;
	
    ret = sysLv2FsOpen(path, 0, &fd, S_IRWXU | S_IRWXG | S_IRWXO, NULL, 0);
	if(ret) goto skip;
	
	ret = sysLv2FsOpen(path2, SYS_O_WRONLY | SYS_O_CREAT | SYS_O_TRUNC, &fd2, S_IRWXU | S_IRWXG | S_IRWXO, NULL, 0);
    if(ret) {sysLv2FsClose(fd);goto skip;}
	sysLv2FsChmod(path2, S_IFMT | S_IRWXU | S_IRWXG | S_IRWXO);
	
    mem = malloc(0x100000);
	
	prog_bar=0;
	
	while(pos < lenght) {
	
        readed = lenght - pos; if(readed > 0x100000ULL) readed = 0x100000ULL;
        ret=sysLv2FsRead(fd, mem, readed, &writed);
		
        if(ret<0) goto skip;
        if(readed != writed) {ret = 0x8001000C; goto skip;}

        ret=sysLv2FsWrite(fd2, mem, readed, &writed);
		
        if(ret<0) goto skip;
        if(readed != writed) {ret = 0x8001000C; goto skip;}
        
        pos += readed;
		
		prog_bar=pos*100/lenght;
		
    }
    
skip:
    
	SetFilePerms(path2);
	
	prog_bar=-1;
	
    if(mem) free(mem);
	if(fd>=0) sysLv2FsClose(fd);
    if(fd2>=0) sysLv2FsClose(fd2);
    if(ret>0) ret = 0;
	
    return ret;
}

s32 open_device( u64 device_ID, u32* fd )
{
    lv2syscall4( 600, device_ID, 0, (u64)fd, 0 );
    return_to_user_prog(s32);
}

s32 close_device( u32 fd)
{
    lv2syscall1( 601, fd );
    return_to_user_prog(s32);
}

s32 read_device( u32 fd, u64 start_sector, u64 nb_sector, const void* buffer, u32 *number_byte_read, u64 flags )
{
    lv2syscall7( 602, fd, 0, start_sector, nb_sector, (u64)buffer, (u64)number_byte_read, flags );
    return_to_user_prog(s32);
}

s32 write_device( u32 fd, u64 start_sector, u64 nb_sector, const void* buffer, u32 *number_byte_wrote, u64 flags )
{
    lv2syscall7( 603, fd, 0, start_sector, nb_sector, (u64)buffer, (u64)number_byte_wrote, flags );
    return_to_user_prog(s32);
}

int have_2_kernels()
{
	u32 sourceR, read;
	
	u64 i,j,k;
	u64 offset_ros[2];
	u64 header_ros[2][3][0x40];
	
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
		offset_ros[0]=0x400;
		offset_ros[1]=0x3C00;
	} else {
		offset_ros[0]=0x600;
		offset_ros[1]=0x3E00;
	}
	
	read_device( sourceR, offset_ros[0]  , 0x1, header_ros[0][0], &read, 0x22 );
	read_device( sourceR, offset_ros[0]+1, 0x1, header_ros[0][1], &read, 0x22 );
	read_device( sourceR, offset_ros[0]+2, 0x1, header_ros[0][2], &read, 0x22 );
	
	read_device( sourceR, offset_ros[1]  , 0x1, header_ros[1][0], &read, 0x22 );
	read_device( sourceR, offset_ros[1]+1, 0x1, header_ros[1][1], &read, 0x22 );
	read_device( sourceR, offset_ros[1]+2, 0x1, header_ros[1][2], &read, 0x22 );
	
	close_device( sourceR );
	
	for(k=0; k<2; k++) {
		for(j=0; j<3; j++){
			for(i = 0; i < 0x40; i++) {
				if(!memcmp(&header_ros[k][j][i], "lv2Ckernel.self", 15)) {
					return YES;
				}
				if(!memcmp(&header_ros[k][j][i], "lv2Dkernel.self", 15)) {
					return YES;
				}
			}
		}
	}
	
	return NO;
	
}

int fw_is_rebug()
{
	u64 ret;

	if(have_2_kernels()==NO) {
		return NO;
	}

	sysFSStat s;
    ret = sysLv2FsStat("/dev_flash/vsh/module/vsh.self.cexsp", &s);
	if(ret==0) return YES;
	ret = sysLv2FsStat("/dev_flash/vsh/module/vsh.self.dexsp", &s);
	if(ret==0) return YES;
	
	return NO;

}

int init_IDPSet()
{
		
	OFFSET_1_IDPS = 0;
	OFFSET_2_IDPS = 0;

	if(( lv2peek(FW_DATE_OFFSET_421C    )==FW_DATE_1_421C) &&
	( lv2peek(FW_DATE_OFFSET_421C + 8)==FW_DATE_2_421C) )
	{
		firmware = 0x421C;
		
		TOC_OFFSET = TOC_OFFSET_421C;
		HV_START_OFFSET = HV_START_OFFSET_421C;
		HTAB_OFFSET = HTAB_OFFSET_421C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_421C;
		MMAP_OFFSET1 = MMAP_OFFSET1_421C;
		MMAP_OFFSET2 = MMAP_OFFSET2_421C;
		SPE_OFFSET = SPE_OFFSET_421C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_421C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_421C;
		
		payload_size = payload_421C_bin_size;
		payload = (u64 *) payload_421C_bin;
	} else
	if(( lv2peek(FW_DATE_OFFSET_421D    )==FW_DATE_1_421D) &&
	   ( lv2peek(FW_DATE_OFFSET_421D + 8)==FW_DATE_2_421D) )
	{
		firmware = 0x421D;
		
		TOC_OFFSET = TOC_OFFSET_421D;
		HV_START_OFFSET = HV_START_OFFSET_421D;
		HTAB_OFFSET = HTAB_OFFSET_421D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_421D;
		MMAP_OFFSET1 = MMAP_OFFSET1_421D;
		MMAP_OFFSET2 = MMAP_OFFSET2_421D;
		SPE_OFFSET = SPE_OFFSET_421D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_421D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_421D;

		payload = (u64 *) payload_421D_bin;
		payload_size = payload_421D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_430C    )==FW_DATE_1_430C) &&
	   ( lv2peek(FW_DATE_OFFSET_430C + 8)==FW_DATE_2_430C) )
	{
		firmware = 0x430C;
		
		TOC_OFFSET = TOC_OFFSET_430C;
		HV_START_OFFSET = HV_START_OFFSET_430C;
		HTAB_OFFSET = HTAB_OFFSET_430C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_430C;
		MMAP_OFFSET1 = MMAP_OFFSET1_430C;
		MMAP_OFFSET2 = MMAP_OFFSET2_430C;
		SPE_OFFSET = SPE_OFFSET_430C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_430C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_430C;
		
		payload = (u64 *) payload_430C_bin;
		payload_size = payload_430C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_430D    )==FW_DATE_1_430D) &&
	   ( lv2peek(FW_DATE_OFFSET_430D + 8)==FW_DATE_2_430D) )
	{
		firmware = 0x430D;
		
		TOC_OFFSET = TOC_OFFSET_430D;
		HV_START_OFFSET = HV_START_OFFSET_430D;
		HTAB_OFFSET = HTAB_OFFSET_430D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_430D;
		MMAP_OFFSET1 = MMAP_OFFSET1_430D;
		MMAP_OFFSET2 = MMAP_OFFSET2_430D;
		SPE_OFFSET = SPE_OFFSET_430D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_430D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_430D;
		
		payload = (u64 *) payload_430D_bin;
		payload_size = payload_430D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_431C    )==FW_DATE_1_431C) &&
	   ( lv2peek(FW_DATE_OFFSET_431C + 8)==FW_DATE_2_431C) )
	{
		firmware = 0x431C;
		
		TOC_OFFSET = TOC_OFFSET_431C;
		HV_START_OFFSET = HV_START_OFFSET_431C;
		HTAB_OFFSET = HTAB_OFFSET_431C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_431C;
		MMAP_OFFSET1 = MMAP_OFFSET1_431C;
		MMAP_OFFSET2 = MMAP_OFFSET2_431C;
		SPE_OFFSET = SPE_OFFSET_431C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_431C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_431C;
		
		payload = (u64 *) payload_431C_bin;
		payload_size = payload_431C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_440C    )==FW_DATE_1_440C) &&
	   ( lv2peek(FW_DATE_OFFSET_440C + 8)==FW_DATE_2_440C) )
	{
		firmware = 0x440C;
		
		TOC_OFFSET = TOC_OFFSET_440C;
		HV_START_OFFSET = HV_START_OFFSET_440C;
		HTAB_OFFSET = HTAB_OFFSET_440C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_440C;
		MMAP_OFFSET1 = MMAP_OFFSET1_440C;
		MMAP_OFFSET2 = MMAP_OFFSET2_440C;
		SPE_OFFSET = SPE_OFFSET_440C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_440C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_440C;
		
		payload = (u64 *) payload_440C_bin;
		payload_size = payload_440C_bin_size;
	}	else
	if(( lv2peek(FW_DATE_OFFSET_441C    )==FW_DATE_1_441C) &&
	   ( lv2peek(FW_DATE_OFFSET_441C + 8)==FW_DATE_2_441C) )
	{
		firmware = 0x441C;
		
		TOC_OFFSET = TOC_OFFSET_441C;
		HV_START_OFFSET = HV_START_OFFSET_441C;
		HTAB_OFFSET = HTAB_OFFSET_441C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_441C;
		MMAP_OFFSET1 = MMAP_OFFSET1_441C;
		MMAP_OFFSET2 = MMAP_OFFSET2_441C;
		SPE_OFFSET = SPE_OFFSET_441C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_441C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_441C;
		
		payload = (u64 *) payload_441C_bin;
		payload_size = payload_441C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_441D    )==FW_DATE_1_441D) &&
	   ( lv2peek(FW_DATE_OFFSET_441D + 8)==FW_DATE_2_441D) )
	{
		firmware = 0x441D;
		
		TOC_OFFSET = TOC_OFFSET_441D;
		HV_START_OFFSET = HV_START_OFFSET_441D;
		HTAB_OFFSET = HTAB_OFFSET_441D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_441D;
		MMAP_OFFSET1 = MMAP_OFFSET1_441D;
		MMAP_OFFSET2 = MMAP_OFFSET2_441D;
		SPE_OFFSET = SPE_OFFSET_441D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_441D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_441D;
		
		payload = (u64 *) payload_441D_bin;
		payload_size = payload_441D_bin_size;
	}	else
	if(( lv2peek(FW_DATE_OFFSET_446C    )==FW_DATE_1_446C) &&
	   ( lv2peek(FW_DATE_OFFSET_446C + 8)==FW_DATE_2_446C) )
	{
		firmware = 0x446C;
		
		TOC_OFFSET = TOC_OFFSET_446C;
		HV_START_OFFSET = HV_START_OFFSET_446C;
		HTAB_OFFSET = HTAB_OFFSET_446C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_446C;
		MMAP_OFFSET1 = MMAP_OFFSET1_446C;
		MMAP_OFFSET2 = MMAP_OFFSET2_446C;
		SPE_OFFSET = SPE_OFFSET_446C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_446C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_446C;
		
		payload = (u64 *) payload_446C_bin;
		payload_size = payload_446C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_446D    )==FW_DATE_1_446D) &&
	   ( lv2peek(FW_DATE_OFFSET_446D + 8)==FW_DATE_2_446D) )
	{ 
		firmware = 0x446D;
		
		TOC_OFFSET = TOC_OFFSET_446D;
		HV_START_OFFSET = HV_START_OFFSET_446D;
		HTAB_OFFSET = HTAB_OFFSET_446D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_446D;
		MMAP_OFFSET1 = MMAP_OFFSET1_446D;
		MMAP_OFFSET2 = MMAP_OFFSET2_446D;
		SPE_OFFSET = SPE_OFFSET_446D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_446D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_446D;
		
		payload = (u64 *) payload_446D_bin;
		payload_size = payload_446D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_450C    )==FW_DATE_1_450C) &&
	   ( lv2peek(FW_DATE_OFFSET_450C + 8)==FW_DATE_2_450C) )
	{
		firmware = 0x450C;
		
		TOC_OFFSET = TOC_OFFSET_450C;
		HV_START_OFFSET = HV_START_OFFSET_450C;
		HTAB_OFFSET = HTAB_OFFSET_450C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_450C;
		MMAP_OFFSET1 = MMAP_OFFSET1_450C;
		MMAP_OFFSET2 = MMAP_OFFSET2_450C;
		SPE_OFFSET = SPE_OFFSET_450C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_450C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_450C;
		
		payload = (u64 *) payload_450C_bin;
		payload_size = payload_450C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_450D    )==FW_DATE_1_450D) &&
	   ( lv2peek(FW_DATE_OFFSET_450D + 8)==FW_DATE_2_450D) )
	{
		firmware = 0x450D;
		
		TOC_OFFSET = TOC_OFFSET_450D;
		HV_START_OFFSET = HV_START_OFFSET_450D;
		HTAB_OFFSET = HTAB_OFFSET_450D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_450D;
		MMAP_OFFSET1 = MMAP_OFFSET1_450D;
		MMAP_OFFSET2 = MMAP_OFFSET2_450D;
		SPE_OFFSET = SPE_OFFSET_450D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_450D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_450D;
		
		payload = (u64 *) payload_450D_bin;
		payload_size = payload_450D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_453C    )==FW_DATE_1_453C) &&
	   ( lv2peek(FW_DATE_OFFSET_453C + 8)==FW_DATE_2_453C) )
	{
		firmware = 0x453C;
		
		TOC_OFFSET = TOC_OFFSET_453C;
		HV_START_OFFSET = HV_START_OFFSET_453C;
		HTAB_OFFSET = HTAB_OFFSET_453C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_453C;
		MMAP_OFFSET1 = MMAP_OFFSET1_453C;
		MMAP_OFFSET2 = MMAP_OFFSET2_453C;
		SPE_OFFSET = SPE_OFFSET_453C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_453C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_453C;
		
		payload = (u64 *) payload_453C_bin;
		payload_size = payload_453C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_453D    )==FW_DATE_1_453D) &&
	   ( lv2peek(FW_DATE_OFFSET_453D + 8)==FW_DATE_2_453D) )
	{
		firmware = 0x453D;
		
		TOC_OFFSET = TOC_OFFSET_453D;
		HV_START_OFFSET = HV_START_OFFSET_453D;
		HTAB_OFFSET = HTAB_OFFSET_453D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_453D;
		MMAP_OFFSET1 = MMAP_OFFSET1_453D;
		MMAP_OFFSET2 = MMAP_OFFSET2_453D;
		SPE_OFFSET = SPE_OFFSET_453D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_453D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_453D;
		
		payload = (u64 *) payload_453D_bin;
		payload_size = payload_453D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_455C    )==FW_DATE_1_455C) &&
	   ( lv2peek(FW_DATE_OFFSET_455C + 8)==FW_DATE_2_455C) )
	{
		firmware = 0x455C;
		
		TOC_OFFSET = TOC_OFFSET_455C;
		HV_START_OFFSET = HV_START_OFFSET_455C;
		HTAB_OFFSET = HTAB_OFFSET_455C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_455C;
		MMAP_OFFSET1 = MMAP_OFFSET1_455C;
		MMAP_OFFSET2 = MMAP_OFFSET2_455C;
		SPE_OFFSET = SPE_OFFSET_455C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_455C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_455C;
		
		payload = (u64 *) payload_455C_bin;
		payload_size = payload_455C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_455D    )==FW_DATE_1_455D) &&
	   ( lv2peek(FW_DATE_OFFSET_455D + 8)==FW_DATE_2_455D) )
	{
		firmware = 0x455D;
		
		TOC_OFFSET = TOC_OFFSET_455D;
		HV_START_OFFSET = HV_START_OFFSET_455D;
		HTAB_OFFSET = HTAB_OFFSET_455D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_455D;
		MMAP_OFFSET1 = MMAP_OFFSET1_455D;
		MMAP_OFFSET2 = MMAP_OFFSET2_455D;
		SPE_OFFSET = SPE_OFFSET_455D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_455D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_455D;
		
		payload = (u64 *) payload_455D_bin;
		payload_size = payload_455D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_460C    )==FW_DATE_1_460C) &&
	   ( lv2peek(FW_DATE_OFFSET_460C + 8)==FW_DATE_2_460C) )
	{
		firmware = 0x460C;
		
		TOC_OFFSET = TOC_OFFSET_460C;
		HV_START_OFFSET = HV_START_OFFSET_460C;
		HTAB_OFFSET = HTAB_OFFSET_460C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_460C;
		MMAP_OFFSET1 = MMAP_OFFSET1_460C;
		MMAP_OFFSET2 = MMAP_OFFSET2_460C;
		SPE_OFFSET = SPE_OFFSET_460C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_460C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_460C;
		
		payload = (u64 *) payload_460C_bin;
		payload_size = payload_460C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_465D    )==FW_DATE_1_465D) &&
	   ( lv2peek(FW_DATE_OFFSET_465D + 8)==FW_DATE_2_465D) )
	{
		firmware = 0x465D;
		
		TOC_OFFSET = TOC_OFFSET_465D;
		HV_START_OFFSET = HV_START_OFFSET_465D;
		HTAB_OFFSET = HTAB_OFFSET_465D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_465D;
		MMAP_OFFSET1 = MMAP_OFFSET1_465D;
		MMAP_OFFSET2 = MMAP_OFFSET2_465D;
		SPE_OFFSET = SPE_OFFSET_465D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_465D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_465D;
		
		payload = (u64 *) payload_465D_bin;
		payload_size = payload_465D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_465C    )==FW_DATE_1_465C) &&
	   ( lv2peek(FW_DATE_OFFSET_465C + 8)==FW_DATE_2_465C) )
	{
		firmware = 0x465C;
		
		TOC_OFFSET = TOC_OFFSET_465C;
		HV_START_OFFSET = HV_START_OFFSET_465C;
		HTAB_OFFSET = HTAB_OFFSET_465C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_465C;
		MMAP_OFFSET1 = MMAP_OFFSET1_465C;
		MMAP_OFFSET2 = MMAP_OFFSET2_465C;
		SPE_OFFSET = SPE_OFFSET_465C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_465C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_465C;
		
		payload = (u64 *) payload_465C_bin;
		payload_size = payload_465C_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_470C    )==FW_DATE_1_470C) &&
	   ( lv2peek(FW_DATE_OFFSET_470C + 8)==FW_DATE_2_470C) )
	{
		firmware = 0x470C;
		
		TOC_OFFSET = TOC_OFFSET_470C;
		HV_START_OFFSET = HV_START_OFFSET_470C;
		HTAB_OFFSET = HTAB_OFFSET_470C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_470C;
		MMAP_OFFSET1 = MMAP_OFFSET1_470C;
		MMAP_OFFSET2 = MMAP_OFFSET2_470C;
		SPE_OFFSET = SPE_OFFSET_470C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_470C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_470C;
		
		payload = (u64 *) payload_470C_bin;
		payload_size = payload_470C_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_470D    )==FW_DATE_1_470D) &&
	   ( lv2peek(FW_DATE_OFFSET_470D + 8)==FW_DATE_2_470D) )
	{
		firmware = 0x470D;
		
		TOC_OFFSET = TOC_OFFSET_470D;
		HV_START_OFFSET = HV_START_OFFSET_470D;
		HTAB_OFFSET = HTAB_OFFSET_470D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_470D;
		MMAP_OFFSET1 = MMAP_OFFSET1_470D;
		MMAP_OFFSET2 = MMAP_OFFSET2_470D;
		SPE_OFFSET = SPE_OFFSET_470D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_470D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_470D;
		
		payload = (u64 *) payload_470D_bin;
		payload_size = payload_470D_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_475C    )==FW_DATE_1_475C) &&
	   ( lv2peek(FW_DATE_OFFSET_475C + 8)==FW_DATE_2_475C) )
	{
		firmware = 0x475C;
		
		TOC_OFFSET = TOC_OFFSET_475C;
		HV_START_OFFSET = HV_START_OFFSET_475C;
		HTAB_OFFSET = HTAB_OFFSET_475C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_475C;
		MMAP_OFFSET1 = MMAP_OFFSET1_475C;
		MMAP_OFFSET2 = MMAP_OFFSET2_475C;
		SPE_OFFSET = SPE_OFFSET_475C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_475C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_475C;
		
		payload = (u64 *) payload_475C_bin;
		payload_size = payload_475C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_475D    )==FW_DATE_1_475D) &&
	   ( lv2peek(FW_DATE_OFFSET_475D + 8)==FW_DATE_2_475D) )
	{
		firmware = 0x475D;

		TOC_OFFSET = TOC_OFFSET_475D;
		HV_START_OFFSET = HV_START_OFFSET_475D;
		HTAB_OFFSET = HTAB_OFFSET_475D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_475D;
		MMAP_OFFSET1 = MMAP_OFFSET1_475D;
		MMAP_OFFSET2 = MMAP_OFFSET2_475D;
		SPE_OFFSET = SPE_OFFSET_475D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_475D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_475D;

		payload = (u64 *) payload_475D_bin;
		payload_size = payload_475D_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_476C    )==FW_DATE_1_476C) &&
	   ( lv2peek(FW_DATE_OFFSET_476C + 8)==FW_DATE_2_476C) )
	{
		firmware = 0x476C;
		
		TOC_OFFSET = TOC_OFFSET_476C;
		HV_START_OFFSET = HV_START_OFFSET_476C;
		HTAB_OFFSET = HTAB_OFFSET_476C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_476C;
		MMAP_OFFSET1 = MMAP_OFFSET1_476C;
		MMAP_OFFSET2 = MMAP_OFFSET2_476C;
		SPE_OFFSET = SPE_OFFSET_476C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_476C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_476C;
		
		payload = (u64 *) payload_476C_bin;
		payload_size = payload_476C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_476D    )==FW_DATE_1_476D) &&
	   ( lv2peek(FW_DATE_OFFSET_476D + 8)==FW_DATE_2_476D) )
	{
		firmware = 0x476D;

		TOC_OFFSET = TOC_OFFSET_476D;
		HV_START_OFFSET = HV_START_OFFSET_476D;
		HTAB_OFFSET = HTAB_OFFSET_476D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_476D;
		MMAP_OFFSET1 = MMAP_OFFSET1_476D;
		MMAP_OFFSET2 = MMAP_OFFSET2_476D;
		SPE_OFFSET = SPE_OFFSET_476D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_476D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_476D;

		payload = (u64 *) payload_476D_bin;
		payload_size = payload_476D_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_478C    )==FW_DATE_1_478C) &&
	   ( lv2peek(FW_DATE_OFFSET_478C + 8)==FW_DATE_2_478C) )
	{
		firmware = 0x478C;
		
		TOC_OFFSET = TOC_OFFSET_478C;
		HV_START_OFFSET = HV_START_OFFSET_478C;
		HTAB_OFFSET = HTAB_OFFSET_478C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_478C;
		MMAP_OFFSET1 = MMAP_OFFSET1_478C;
		MMAP_OFFSET2 = MMAP_OFFSET2_478C;
		SPE_OFFSET = SPE_OFFSET_478C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_478C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_478C;
		
		payload = (u64 *) payload_478C_bin;
		payload_size = payload_478C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_478D    )==FW_DATE_1_478D) &&
	   ( lv2peek(FW_DATE_OFFSET_478D + 8)==FW_DATE_2_478D) )
	{
		firmware = 0x478D;

		TOC_OFFSET = TOC_OFFSET_478D;
		HV_START_OFFSET = HV_START_OFFSET_478D;
		HTAB_OFFSET = HTAB_OFFSET_478D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_478D;
		MMAP_OFFSET1 = MMAP_OFFSET1_478D;
		MMAP_OFFSET2 = MMAP_OFFSET2_478D;
		SPE_OFFSET = SPE_OFFSET_478D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_478D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_478D;

		payload = (u64 *) payload_478D_bin;
		payload_size = payload_478D_bin_size;
	} else 
	if(( lv2peek(FW_DATE_OFFSET_480C    )==FW_DATE_1_480C) &&
	   ( lv2peek(FW_DATE_OFFSET_480C + 8)==FW_DATE_2_480C) )
	{
		firmware = 0x480C;
		
		TOC_OFFSET = TOC_OFFSET_480C;
		HV_START_OFFSET = HV_START_OFFSET_480C;
		HTAB_OFFSET = HTAB_OFFSET_480C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_480C;
		MMAP_OFFSET1 = MMAP_OFFSET1_480C;
		MMAP_OFFSET2 = MMAP_OFFSET2_480C;
		SPE_OFFSET = SPE_OFFSET_480C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_480C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_480C;
		
		payload = (u64 *) payload_480C_bin;
		payload_size = payload_480C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_480D    )==FW_DATE_1_480D) &&
	   ( lv2peek(FW_DATE_OFFSET_480D + 8)==FW_DATE_2_480D) )
	{
		firmware = 0x480D;

		TOC_OFFSET = TOC_OFFSET_480D;
		HV_START_OFFSET = HV_START_OFFSET_480D;
		HTAB_OFFSET = HTAB_OFFSET_480D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_480D;
		MMAP_OFFSET1 = MMAP_OFFSET1_480D;
		MMAP_OFFSET2 = MMAP_OFFSET2_480D;
		SPE_OFFSET = SPE_OFFSET_480D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_480D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_480D;

		payload = (u64 *) payload_480D_bin;
		payload_size = payload_480D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_481C    )==FW_DATE_1_481C) &&
	   ( lv2peek(FW_DATE_OFFSET_481C + 8)==FW_DATE_2_481C) )
	{
		firmware = 0x481C;

		TOC_OFFSET = TOC_OFFSET_481C;
		HV_START_OFFSET = HV_START_OFFSET_481C;
		HTAB_OFFSET = HTAB_OFFSET_481C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_481C;
		MMAP_OFFSET1 = MMAP_OFFSET1_481C;
		MMAP_OFFSET2 = MMAP_OFFSET2_481C;
		SPE_OFFSET = SPE_OFFSET_481C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_481C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_481C;

		payload = (u64 *) payload_481C_bin;
		payload_size = payload_481C_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_481D    )==FW_DATE_1_481D) &&
	   ( lv2peek(FW_DATE_OFFSET_481D + 8)==FW_DATE_2_481D) )
	{
		firmware = 0x481D;

		TOC_OFFSET = TOC_OFFSET_481D;
		HV_START_OFFSET = HV_START_OFFSET_481D;
		HTAB_OFFSET = HTAB_OFFSET_481D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_481D;
		MMAP_OFFSET1 = MMAP_OFFSET1_481D;
		MMAP_OFFSET2 = MMAP_OFFSET2_481D;
		SPE_OFFSET = SPE_OFFSET_481D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_481D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_481D;

		payload = (u64 *) payload_481D_bin;
		payload_size = payload_481D_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_482C    )==FW_DATE_1_482C) &&
	   ( lv2peek(FW_DATE_OFFSET_482C + 8)==FW_DATE_2_482C) )
	{
		firmware = 0x482C;

		TOC_OFFSET = TOC_OFFSET_482C;
		HV_START_OFFSET = HV_START_OFFSET_482C;
		HTAB_OFFSET = HTAB_OFFSET_482C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_482C;
		MMAP_OFFSET1 = MMAP_OFFSET1_482C;
		MMAP_OFFSET2 = MMAP_OFFSET2_482C;
		SPE_OFFSET = SPE_OFFSET_482C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_482C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_482C;

		payload = (u64 *) payload_482C_bin;
		payload_size = payload_482C_bin_size;
	} else
	if(( lv2peek(FW_DATE_OFFSET_482D    )==FW_DATE_1_482D) &&
	   ( lv2peek(FW_DATE_OFFSET_482D + 8)==FW_DATE_2_482D) )
	{
		firmware = 0x482D;

		TOC_OFFSET = TOC_OFFSET_482D;
		HV_START_OFFSET = HV_START_OFFSET_482D;
		HTAB_OFFSET = HTAB_OFFSET_482D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_482D;
		MMAP_OFFSET1 = MMAP_OFFSET1_482D;
		MMAP_OFFSET2 = MMAP_OFFSET2_482D;
		SPE_OFFSET = SPE_OFFSET_482D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_482D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_482D;

		payload = (u64 *) payload_482D_bin;
		payload_size = payload_482D_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_483C    )==FW_DATE_1_483C) &&
	   ( lv2peek(FW_DATE_OFFSET_483C + 8)==FW_DATE_2_483C) )
	{
		firmware = 0x483C;

		TOC_OFFSET = TOC_OFFSET_483C;
		HV_START_OFFSET = HV_START_OFFSET_483C;
		HTAB_OFFSET = HTAB_OFFSET_483C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_483C;
		MMAP_OFFSET1 = MMAP_OFFSET1_483C;
		MMAP_OFFSET2 = MMAP_OFFSET2_483C;
		SPE_OFFSET = SPE_OFFSET_483C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_483C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_483C;

		payload = (u64 *) payload_483C_bin;
		payload_size = payload_483C_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_484C    )==FW_DATE_1_484C) &&
	   ( lv2peek(FW_DATE_OFFSET_484C + 8)==FW_DATE_2_484C) )
	{
		firmware = 0x484C;

		TOC_OFFSET = TOC_OFFSET_484C;
		HV_START_OFFSET = HV_START_OFFSET_484C;
		HTAB_OFFSET = HTAB_OFFSET_484C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_484C;
		MMAP_OFFSET1 = MMAP_OFFSET1_484C;
		MMAP_OFFSET2 = MMAP_OFFSET2_484C;
		SPE_OFFSET = SPE_OFFSET_484C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_484C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_484C;

		payload = (u64 *) payload_484C_bin;
		payload_size = payload_484C_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_484D    )==FW_DATE_1_484D) &&
	   ( lv2peek(FW_DATE_OFFSET_484D + 8)==FW_DATE_2_484D) )
	{
		firmware = 0x484D;

		TOC_OFFSET = TOC_OFFSET_484D;
		HV_START_OFFSET = HV_START_OFFSET_484D;
		HTAB_OFFSET = HTAB_OFFSET_484D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_484D;
		MMAP_OFFSET1 = MMAP_OFFSET1_484D;
		MMAP_OFFSET2 = MMAP_OFFSET2_484D;
		SPE_OFFSET = SPE_OFFSET_484D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_484D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_484D;

		payload = (u64 *) payload_484D_bin;
		payload_size = payload_484D_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_485C    )==FW_DATE_1_485C) &&
	   ( lv2peek(FW_DATE_OFFSET_485C + 8)==FW_DATE_2_485C) )
	{
		firmware = 0x485C;

		TOC_OFFSET = TOC_OFFSET_485C;
		HV_START_OFFSET = HV_START_OFFSET_485C;
		HTAB_OFFSET = HTAB_OFFSET_485C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_485C;
		MMAP_OFFSET1 = MMAP_OFFSET1_485C;
		MMAP_OFFSET2 = MMAP_OFFSET2_485C;
		SPE_OFFSET = SPE_OFFSET_485C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_485C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_485C;

		payload = (u64 *) payload_485C_bin;
		payload_size = payload_485C_bin_size;
	}  else
	if(( lv2peek(FW_DATE_OFFSET_485D    )==FW_DATE_1_485D) &&
	   ( lv2peek(FW_DATE_OFFSET_485D + 8)==FW_DATE_2_485D) )
	{
		firmware = 0x485D;

		TOC_OFFSET = TOC_OFFSET_485D;
		HV_START_OFFSET = HV_START_OFFSET_485D;
		HTAB_OFFSET = HTAB_OFFSET_485D;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_485D;
		MMAP_OFFSET1 = MMAP_OFFSET1_485D;
		MMAP_OFFSET2 = MMAP_OFFSET2_485D;
		SPE_OFFSET = SPE_OFFSET_485D;
		OFFSET_1_IDPS = OFFSET_1_IDPS_485D;
		OFFSET_2_IDPS = OFFSET_2_IDPS_485D;

		payload = (u64 *) payload_485D_bin;
		payload_size = payload_485D_bin_size;
	}
	else { // firmware inconnu
		u64 n;
		for(n=0x350000; n<0x4A0000; n++) {
			u64 data = lv2peek(0x8000000000000000ULL + n);
			
			if(0x350000 < n && n <0x450000) {
				if(OFFSET_1_IDPS == 0) {
					if(0x0000000100800000 < data && data <0x0000000100900000)  //PS3 IDPS
					{
						OFFSET_1_IDPS = 0x8000000000000000ULL + n;		
					}
				}
			}
			if(0x450000 < n && n <0x4A0000) {
				if(OFFSET_2_IDPS == 0) {
					if(0x0000000100800000 < data && data <0x0000000100900000)  //PS3 IDPS
					{
						OFFSET_2_IDPS = 0x8000000000000000ULL + n;		
					}
				}
			}
			if(OFFSET_1_IDPS != 0)
			if(OFFSET_2_IDPS != 0) break;
		}
		if(OFFSET_1_IDPS == 0 || OFFSET_2_IDPS == 0) return FAILED;
	}

	rebug =  fw_is_rebug();
	
	sysFSStat st;
	if(sysLv2FsStat("/dev_hdd0/tmp", &st) != 0) {
		sysLv2FsMkdir("/dev_hdd0/tmp", 0777);
	}
	sysLv2FsChmod("/dev_hdd0/tmp", FS_S_IFDIR | 0777);
	
	return SUCCESS;
}

int eid0_is_CEX()
{
	u32 sourceR, read;
	u64 offset, sector[ 0x40 ];
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
    	offset = 0x204; // * 0x200
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
	}
	else	offset = 0x178; // * 0x200 NOR
	
	read_device( sourceR, offset, 0x1, sector, &read, 0x22 );
	close_device( sourceR );
	
	if(0x0000000100820000 <= sector[0x0E] && sector[0x0E] < 0x0000000100830000) return NO; 
	else return YES;
	
}

int is_dumpable()
{
	if(TOC_OFFSET != 0) {
		return YES;
	}
	
	return NO;
}

void Draw_Box(float x, float y, float z, float r, float w, float h, u32 rgba)
{
	if(r > w || r > h) r=0;
	
	if(r==0){
		tiny3d_SetPolygon(TINY3D_QUADS);
		tiny3d_VertexPos(x     	, y     , z);
		tiny3d_VertexColor(rgba);
		tiny3d_VertexPos(x + w	, y     , z);
		tiny3d_VertexPos(x + w	, y + h	, z);
		tiny3d_VertexPos(x     	, y + h	, z);
		tiny3d_End();
	}
	else {
		int t;
		float PI = 3.14159265;
		tiny3d_SetPolygon(TINY3D_POLYGON);
		tiny3d_VertexColor(rgba);
		x+=r;
		y+=r;
		for(t=-90; t>=-180 ; t-=10) {
			tiny3d_VertexPos( x + r*cos(t*PI/180), y + r*sin(t*PI/180), z);
		}
		y+= h-2*r;
		for(t=180; t>=90 ; t-=10) {
			tiny3d_VertexPos( x + r*cos(t*PI/180), y + r*sin(t*PI/180), z);
		}
		x+= w-2*r;
		for(t=90; t>=0 ; t-=10) {
			tiny3d_VertexPos( x + r*cos(t*PI/180), y + r*sin(t*PI/180), z);
		}
		y-= h-2*r;
		for(t=0; t>=-90 ; t-=10) {
			tiny3d_VertexPos( x + r*cos(t*PI/180), y + r*sin(t*PI/180), z);
		}
		
		tiny3d_End();
	}
	
}

void Draw_NOT(float x, float y, float z, float w, float h)
{	
	if(NOT_offset != 0) {
		tiny3d_SetTexture(0, NOT_offset, NOT.width, NOT.height, NOT.pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);
		
		tiny3d_SetPolygon(TINY3D_QUADS);
		tiny3d_VertexPos(x, y, z);
		tiny3d_VertexColor(0xFFFFFFFF);
		tiny3d_VertexTexture(0.0f, 0.0f);
		tiny3d_VertexPos(x + w, y, z);
		tiny3d_VertexTexture(1.0f, 0.0f);
		tiny3d_VertexPos(x + w, y + h, z);
		tiny3d_VertexTexture(1.0f, 1.0f);
		tiny3d_VertexPos(x, y+h, z);
		tiny3d_VertexTexture(0.0f, 1.0f);
		tiny3d_End();
	}
}

void Draw_Notification()
{
	
	if(not_msg[0] == 0)	return;
	time_not++;
	if(time_not > 150) { time_not=0; not_msg[0]=0; return ;}
	
	SetFontSize(14, 14);
	SetFontColor(WHITE, 0x0);
	DrawFormatString( 635, 58, "%s", not_msg);
	
	Draw_Box(590, 40, 5, 3, 213, 50, 0x555555FF);
	Draw_NOT(600, 50, 4, 30, 30);
}

void show_msg(char *str)
{
	strcpy(not_msg, str);
}

void Draw_Button(float x, float y, float d)
{
	x+=d/2;
	y+=d/2;
	int t;
	float PI = 3.14159265;
	
	tiny3d_SetPolygon(TINY3D_POLYGON);
	tiny3d_VertexColor(0x3F3F41ff);
	for(t=0; t<=360 ; t+=10) {
		tiny3d_VertexPos( x + d/1.9*sin(t*PI/180), y + d/1.9*cos(t*PI/180), 0);
	}
	tiny3d_End();
	
}

int Draw_Button_Circle(float x, float y, float size)
{
	Draw_Button(x, y, size);
	
	x+=size/2;
	y+=size/2;

	int t;
	float PI = 3.14159265;

	tiny3d_SetPolygon(TINY3D_POLYGON);
	tiny3d_VertexColor(0xFF646Fff);
	for(t=0; t<=360 ; t+=10) {
		tiny3d_VertexPos( x + size/2*3/4*sin(t*PI/180), y + size/2*3/4*cos(t*PI/180), 0);
	}
	tiny3d_End();
	
	tiny3d_SetPolygon(TINY3D_POLYGON);
	tiny3d_VertexColor(0x3F3F41ff);
	for(t=0; t<=360 ; t+=10) {
		tiny3d_VertexPos( x + size/2*5/8*sin(t*PI/180), y + size/2*5/8*cos(t*PI/180), 0);
	}
	tiny3d_End();
	
	return x+size/2;
	
}

int Draw_Button_Cross(float x, float y, float size)
{
	Draw_Button(x, y, size);		
	
	x+=size/2;
	y+=size/2;
	
	tiny3d_SetPolygon(TINY3D_POLYGON);
	tiny3d_VertexColor(0x82B4EBff);
	tiny3d_VertexPos( x - size/2*15/16*sqrt(2)/2, y - size/2*13/16*sqrt(2)/2, 0);
	tiny3d_VertexPos( x - size/2*13/16*sqrt(2)/2, y - size/2*15/16*sqrt(2)/2, 0);
	tiny3d_VertexPos( x + size/2*15/16*sqrt(2)/2 , y + size/2*13/16*sqrt(2)/2 , 0);
	tiny3d_VertexPos( x + size/2*13/16*sqrt(2)/2 , y + size/2*15/16*sqrt(2)/2 , 0);
	tiny3d_End();
	
	tiny3d_SetPolygon(TINY3D_POLYGON);
	tiny3d_VertexColor(0x82B4EBff);
	tiny3d_VertexPos( x + size/2*13/16*sqrt(2)/2, y - size/2*15/16*sqrt(2)/2, 0);
	tiny3d_VertexPos( x + size/2*15/16*sqrt(2)/2, y - size/2*13/16*sqrt(2)/2, 0);	
	tiny3d_VertexPos( x - size/2*13/16*sqrt(2)/2, y + size/2*15/16*sqrt(2)/2, 0);
	tiny3d_VertexPos( x - size/2*15/16*sqrt(2)/2, y + size/2*13/16*sqrt(2)/2, 0);
	tiny3d_End();
	
	return size/2+x;
}

void DrawUp(float x, float y)
{
    tiny3d_SetPolygon(TINY3D_TRIANGLES);
    tiny3d_VertexPos(x-5 , y , 0);
    tiny3d_VertexColor(RED);
    tiny3d_VertexPos(x, y-5 , 0);
    tiny3d_VertexPos(x+5, y, 0);
	tiny3d_End();
}

void DrawDown(float x, float y) 
{
    tiny3d_SetPolygon(TINY3D_TRIANGLES);
    tiny3d_VertexPos(x-5, y , 0); 
    tiny3d_VertexColor(RED);
    tiny3d_VertexPos(x, y+5 , 0); 
    tiny3d_VertexPos(x+5, y, 0);
	tiny3d_End();
}

void Draw_BG()
{
	if(BG_offset != 0 ) {
		tiny3d_SetTexture(0, BG_offset, BG.width, BG.height, BG.pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);
		
		tiny3d_SetPolygon(TINY3D_QUADS);
		tiny3d_VertexPos(0, 0, 1000);
		tiny3d_VertexColor(WHITE);
		tiny3d_VertexTexture(0.0f, 0.0f);
		tiny3d_VertexPos(848, 0, 1000);
		tiny3d_VertexTexture(1.0f, 0.0f);
		tiny3d_VertexPos(848, 512, 1000);
		tiny3d_VertexTexture(1.0f, 1.0f);
		tiny3d_VertexPos(0, 512, 1000);
		tiny3d_VertexTexture(0.0f, 1.0f);
		tiny3d_End();
	}
	
}

void Draw_Header()
{
	Draw_BG();
	
	if(ICON0_offset != 0) {
		tiny3d_SetTexture(0, ICON0_offset, ICON0.width, ICON0.height, ICON0.pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);
		
		tiny3d_SetPolygon(TINY3D_QUADS);

		float w=140.0;
		float h=76.0;
		
		tiny3d_VertexPos(424-w/2, 20, 0);
		tiny3d_VertexColor(0xFFFFFFFF);
		tiny3d_VertexTexture(0.0f, 0.0f);

		tiny3d_VertexPos(424+w/2, 20, 0);
		tiny3d_VertexTexture(1.0f, 0.0f);

		tiny3d_VertexPos(424+w/2, 20+h, 0);
		tiny3d_VertexTexture(1.0f, 1.0f);

		tiny3d_VertexPos(424-w/2, 20+h, 0);
		tiny3d_VertexTexture(0.0f, 1.0f);

		tiny3d_End();
	}
	else {
		SetFontSize(30, 30);
		SetFontAutoCenter(1);
		SetFontColor(BLUE, 0x0);
		DrawString(0, 20, "IDPSet");
		SetFontAutoCenter(0);
	}
}

void Draw_IDPS()
{
	int x= 50;
	int y= 180;
	int x1;

	for(x1=0; x1<0x10; x1++) {
		IDPS[x1]=lv2peek8(OFFSET_1_IDPS + x1);
	}

	SetFontSize(25, 25); 
	SetFontColor(ORANGE, 0x0);
	x = DrawString(x,y, "00000001008");

	if(position==100) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x, y, "%X", IDPS[0x5] - (IDPS[5] >> 4)*0x10 );
	if(position==100) {
		if(IDPS[0x5] - (IDPS[0x5] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0x5] - (IDPS[0x5] >> 4)*0x10 > 0x2) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	SetFontColor(ORANGE, 0x0);
	x = DrawString(x,y, "00");
	x = DrawFormatString(x,y, "%X", (IDPS[0x7] >> 4));
	
	//0x7
	if(position==101) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0x7] - (IDPS[0x7] >> 4)*0x10);
	if(position==101) {
		if(IDPS[0x7]<0x14) DrawUp((x+x1)/2, y);
		if(IDPS[0x7]>0x01) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0x8
	if(position==102) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0x8] >> 4));
	if(position==102) {
		if((IDPS[0x8] >> 4) == 0x0 || (IDPS[0x8] >> 4) == 0x1) DrawUp((x+x1)/2, y);
		if((IDPS[0x8] >> 4) == 0x1 || (IDPS[0x8] >> 4) == 0xF) DrawDown((x+x1)/2, y+25*0.85);
	}
	if(position==103) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0x8] - (IDPS[0x8] >> 4)*0x10);
	if(position==103) {
		if(IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 == 0x0) DrawUp((x+x1)/2, y); else
		if(IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 == 0x4) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0x9
	if(position==104) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0x9] >> 4));
	if(position==104) {
		if((IDPS[0x9] >> 4) < 0x3) DrawUp((x+x1)/2, y);
		if((IDPS[0x9] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	if(position==105) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0x9] - (IDPS[0x9] >> 4)*0x10);
	if(position==105) {
		if(IDPS[0x9] - (IDPS[0x9] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0x9] - (IDPS[0x9] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xA
	if(position==106) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xA] >> 4));
	if(position==106) {
		if((IDPS[0xA] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xA] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==107) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xA] - (IDPS[0xA] >> 4)*0x10);
	if(position==107) {
		if(IDPS[0xA] - (IDPS[0xA] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xA] - (IDPS[0xA] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xB
	if(position==108) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xB] >> 4));
	if(position==108) {
		if((IDPS[0xB] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xB] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==109) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xB] - (IDPS[0xB] >> 4)*0x10);
	if(position==109) {
		if(IDPS[0xB] - (IDPS[0xB] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xB] - (IDPS[0xB] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xC
	if(position==110) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xC] >> 4));
	if(position==110) {
		if((IDPS[0xC] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xC] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==111) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xC] - (IDPS[0xC] >> 4)*0x10);
	if(position==111) {
		if(IDPS[0xC] - (IDPS[0xC] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xC] - (IDPS[0xC] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xD
	if(position==112) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xD] >> 4));
	if(position==112) {
		if((IDPS[0xD] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xD] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==113) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xD] - (IDPS[0xD] >> 4)*0x10);
	if(position==113) {
		if(IDPS[0xD] - (IDPS[0xD] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xD] - (IDPS[0xD] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xE
	if(position==114) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xE] >> 4));
	if(position==114) {
		if((IDPS[0xE] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xE] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==115) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xE] - (IDPS[0xE] >> 4)*0x10);
	if(position==115) {
		if(IDPS[0xE] - (IDPS[0xE] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xE] - (IDPS[0xE] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	//0xF
	if(position==116) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", (IDPS[0xF] >> 4));
	if(position==116) {
		if((IDPS[0xF] >> 4) < 0xF) DrawUp((x+x1)/2, y);
		if((IDPS[0xF] >> 4) > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	if(position==117) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	x1=x;
	x=DrawFormatString(x,y, "%X", IDPS[0xF] - (IDPS[0xF] >> 4)*0x10);
	if(position==117) {
		if(IDPS[0xF] - (IDPS[0xF] >> 4)*0x10 < 0xF) DrawUp((x+x1)/2, y);
		if(IDPS[0xF] - (IDPS[0xF] >> 4)*0x10 > 0x0) DrawDown((x+x1)/2, y+25*0.85);
	}
	
	x=50; y+=30;
	SetFontSize(10, 10);
	SetFontColor(ORANGE, 0x0);
	x=DrawString(x,y, "Target ID : ");
	
	if((IDPS[5] >> 4) == 0) x=DrawString(x,y, "PSP "); else
	if((IDPS[5] >> 4) == 8) x=DrawString(x,y, "PS3 ");
	
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x2) DrawString(x,y, "DEX"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x3) DrawString(x,y, "JAPAN"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x4) DrawString(x,y, "USA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x5) DrawString(x,y, "EUROPE"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x6) DrawString(x,y, "KOREA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x7) DrawString(x,y, "UK"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x8) DrawString(x,y, "MEXICO"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0x9) DrawString(x,y, "AUSTRALIA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xA) DrawString(x,y, "SOUTH ASIA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xB) DrawString(x,y, "TAIWAN"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xC) DrawString(x,y, "RUSSIA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xD) DrawString(x,y, "CHINA"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xE) DrawString(x,y, "HONG KONG"); else
	if(IDPS[5] - (IDPS[5] >> 4)*0x10 == 0xF) DrawString(x,y, "BRAZIL");

}

void Draw_menu()
{
	int x = 50;
	int y = 260;
	
	SetFontSize(20, 20); 
	
	if(position==0) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x, y, "Edit IDPS");
	
	y+=20;
	
	if(position==1) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x, y, "Save IDPS in EID0");
	
	y+=20;
	
	if(position==2) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x,y, "Save IDPS in EID0 & EID5");
	
	y+=20;
	
	if(position==3) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x,y, "Load IDPS from EID0");
	
	y+=20;
	
	if(position==4) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x,y, "Load IDPS from EID5");
	
	y+=20;
	
	if(position==5) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
	DrawString(x,y, "Make CEX & DEX dumps");
	
	if(is_dumpable()==YES) {
		y+=20;
		if(position==6) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
		DrawString(x,y, "Dump eid_root_key");
	}
	
	if(rebug==YES) {
		y+=20;
		if(position==7) SetFontColor(GREEN, 0x0); else SetFontColor(WHITE, 0x0);
		if(eid0_is_CEX() == YES)	DrawString(x, y, "Convert to DEX");  
		else DrawString(x,y, "Convert to CEX");
	}
	
}

void Draw_buttons()
{
	int x=50;
	SetFontAutoCenter(0);
	SetFontSize(20,20);
	SetFontColor(WHITE, 0x0);
	
	x=Draw_Button_Circle(x, 485, 15);
	if(position<100)	x=DrawString( x+5, 485, "Back to XMB"); else
	x=DrawString( x+5, 485, "Back to menu");
	if(position<100) {
		x=Draw_Button_Cross(x+10, 485, 15);
		x=DrawString( x+5, 485, "OK");
	}
	
}

int Draw_Progress_Bar(float x, float y, u8 size, float value, u32 color)
{	
	float w=size*100;
	
	float e = 2;
	float h = 4;
	
	Draw_Box(x  , y  , 0, 0, w+e*2+2, h+e*2+2,  WHITE);
	Draw_Box(x+e, y+e, 0, 0, w+2    , h+2    , 0x000000FF);
	
	Draw_Box(x+e+1, y+e+1, 0, 0, value*size, h, color);
	
	return x+w+2*e;
	
}

void Draw_Loading(void *unused)
{
	int i=0;
	int m=0;
	while(loading) {
		
		cls();
		ps3pad_read();
		
		Draw_Header();
		Draw_IDPS();
		int x=50, y=260;
		SetFontAutoCenter(0);
		SetFontSize(20, 20);
		
		for(i=0; i < 10; i++) {
			if(strstr(loading_log[i], "Error")) SetFontColor(RED, 0x0); else SetFontColor(ORANGE, 0x0);
			DrawString(x , y, loading_log[i]);
			y+=20;
		}
		
		if(0 <= prog_bar && prog_bar <= 100) {
			Draw_Progress_Bar(50, 230, 2, prog_bar, ORANGE);
		}
		
		// *** barre qui defile en boucle ***
		SetFontColor(ORANGE, 0x0);
		SetFontSize(15, 15);
		DrawString(50 , 70, "Please wait...");
		Draw_Box(50, 50, 0, 0, 50, 20, ORANGE);
		Draw_Box(55, 55, 0, 0, 40, 10, 0x000000FF);
		Draw_Box(50+m, 56, 0, 0, 5, 8, ORANGE);
		if(m>=40) m=0; else m++;
		
		tiny3d_Flip();
	}

	for(i=0; i<10; i++){
		strcpy(loading_log[i], "\0");
	}
	
	sysThreadExit(0);
	
}
	
void print_load(char *format, ...)
{
	char *str = (char *) buff;
	va_list	opt;
	
	va_start(opt, format);
	vsprintf( (void *) buff, format, opt);
	va_end(opt);
	
	int i=0;
	for(i=9; i>0; i--){
		strcpy(loading_log[i], loading_log[i-1]);
	}
	
	strcpy(loading_log[0], str);
	
	if(strstr(str, "Error")) sleep(4);
}

void start_loading()
{
	loading=1;
	sysThreadCreate(&loading_id, Draw_Loading, NULL, 999, 0x2000, THREAD_JOINABLE, "loading");	
}

void end_loading()
{
	u64 ret;
	loading= 0;
	sysThreadJoin(loading_id, &ret);
}

void Delete(char *path)
{
	sysLv2FsChmod(path, FS_S_IFMT | 0777);
	sysLv2FsUnlink(path);
}

int eid0_decrypt_section(u8 *section_in, u8 *section_out, int num_section)
{
	u8 indiv[INDIV_SIZE];
	u8 key[0x10];
	aes_context aes_ctxt;
	
	//Generate individuals.
	indiv_gen(eid0_indiv_seed, NULL, NULL, NULL, indiv);

	//Generate key.
	aes_setkey_enc(&aes_ctxt, indiv + 0x20, 0x100);
	
	if(num_section==0)		aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_0, key);
	if(num_section==6)		aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_6, key);
	if(num_section==0xA)	aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_A, key);
	
	//Decrypt section
	aes_setkey_dec(&aes_ctxt, key, 0x80);
	aes_crypt_cbc(&aes_ctxt, AES_DECRYPT, 0xC0, indiv + 0x10, section_in, section_out);

	//Calculate aes omac1.
	u8 digest[AES_OMAC1_DIGEST_SIZE];
	aes_omac1(digest, section_out, 0xA8, key, 0x80);
	
	if (memcmp(digest, section_out + 0xA8, AES_OMAC1_DIGEST_SIZE) != 0) {
        print_load("Error: bad eid_root_key");
		return FAILED;
	} else return SUCCESS; 
}

void eid0_encrypt_section(u8 *section_in, u8 *section_out, int num_section)
{
	u8 indiv[INDIV_SIZE];
	u8 key[0x10];
	aes_context aes_ctxt;

	//Generate individuals.
	indiv_gen(eid0_indiv_seed, NULL, NULL, NULL, indiv);

	//Generate key.
	aes_setkey_enc(&aes_ctxt, indiv + 0x20, 0x100);

	if(num_section==0)		aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_0, key);
	if(num_section==6)		aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_6, key);
	if(num_section==0xA)	aes_crypt_ecb(&aes_ctxt, AES_ENCRYPT, eid0_keyseed_A, key);

	//Calculate aes omac1.
	aes_omac1(section_in + 0xA8, section_in, 0xA8, key, 0x80);

	//Encrypt section
	aes_setkey_enc(&aes_ctxt, key, 0x80);
	aes_crypt_cbc(&aes_ctxt, AES_ENCRYPT, 0xC0, indiv + 0x10, section_in, section_out);
	
}

u8 choose_target()
{
	end_loading();
	
	u8 targ = 0x3;
	
	while(1) {
		cls();
		Draw_Header();
		int x=50, y=180, x1;
		
		SetFontSize(25, 25);
		SetFontColor(ORANGE, 0x0);
		DrawString(50, 100, "Choose your Target for the CEX dump");
		
		SetFontColor(GREEN, 0x0);
		
		x=DrawString(x, y, "8");
		x1=x;
		x=DrawFormatString(x, y, "%X", targ);
		if(targ<0xF) DrawUp((x+x1)/2, y);
		if(targ>0x3) DrawDown((x+x1)/2, y+25*0.85);
		
		y+=50;
		SetFontSize(10, 10);
		SetFontColor(ORANGE, 0x0);
		x=DrawString(x,y, "Target ID : ");
		if(targ==0x2) DrawString(x,y, "DEX");
		if(targ==0x3) DrawString(x,y, "JAPAN");
		if(targ==0x4) DrawString(x,y, "USA");
		if(targ==0x5) DrawString(x,y, "EUROPE");
		if(targ==0x6) DrawString(x,y, "KOREA");
		if(targ==0x7) DrawString(x,y, "UK");
		if(targ==0x8) DrawString(x,y, "MEXICO");
		if(targ==0x9) DrawString(x,y, "AUSTRALIA");
		if(targ==0xA) DrawString(x,y, "SOUTH ASIA");
		if(targ==0xB) DrawString(x,y, "TAIWAN");
		if(targ==0xC) DrawString(x,y, "RUSSIA");
		if(targ==0xD) DrawString(x,y, "CHINA");
		if(targ==0xE) DrawString(x,y, "HONG KONG");
		if(targ==0xF) DrawString(x,y, "BRAZIL");
		
		x=50;
		x=Draw_Button_Cross(x, 485, 15);
		DrawString( x+5, 485, "OK");
		
		tiny3d_Flip();
		ps3pad_read();
		if(new_pad & BUTTON_UP) {
			if(targ<0xF) targ++;
		}
		if(new_pad & BUTTON_UP) {
			if(targ>0x3) targ--;
		}
		if(new_pad & BUTTON_CROSS) {
			targ+=0x80;
			break;
		}
	}
	
	start_loading();
	
	return targ;
}

u8 target_from_eid5()
{
	
	u32 source, read;
	u64 offset;
    u64 buffer[ 0x40 ];
    int ret = 1;

    ret = open_device( 0x100000000000004ULL, &source );
    if( ret != 0 ) {
		offset = 0x20D; 
		close_device( source );
		open_device( 0x100000000000001ULL, &source );
	}
	else offset = 0x181; 
	
    read_device( source, offset, 0x1, buffer, &read, 0x22 );
	
	close_device( source );
	
	return (u8) ((buffer[0x3a] >> 16) - 0x10000);
}

int dump_root_key()
{
	int ret;
	sysFSStat s;
	char path[128];
	char temp[128];
	
	strcpy(path, "/dev_hdd0/tmp/eid_root_key");
	
	ret=sysLv2FsStat(path, &s);
	if(ret==0) Delete(path);
	
	ret = dump_eid_root_key(path) == 0 ;
	
	if(ret==FAILED) return FAILED;
	
	SetFilePerms(path);
	
	int c;
	
	for(c=0; c<9; c++) {
		sprintf(temp, "/dev_usb00%c", c+48);
		if(sysLv2FsStat(temp, &s)==0){
			strcat(temp, "/eid_root_key");
			Delete(temp);
			
			if( CopyFile(path, temp) == 0) {
				Delete(path);
				strcpy(path, temp);
				break;
			}
		}
	}
    
	print_load("> EID ROOT KEY dumped at %s", path);
	
	return SUCCESS;
	
}

int get_root_key()
{
	FILE* fp;
	fp = NULL;
	
	int dfdZ;
	u64 readZ;
	sysFSDirent dirZ;
	char temp[128];
	
	memset(eid_root_key, 0, EID_ROOT_KEY_SIZE);
	
	strcpy(temp, "/dev_hdd0/tmp/eid_root_key");
	if((fp = fopen(temp, "rb"))==NULL){
		strcpy(temp, "/dev_hdd0/game/FLTZ00010/USRDIR/eid_root_key");
		if((fp = fopen(temp, "rb"))==NULL){
			if(!sysLv2FsOpenDir("/", &dfdZ)) {
				readZ = sizeof(sysFSDirent);
				while(!sysLv2FsReadDir(dfdZ, &dirZ, &readZ)) {
					if(!readZ) break;
					if(strstr(dirZ.d_name, "dev_usb") || strstr(dirZ.d_name, "dev_hdd0")) {
						sprintf(temp, "/%s/eid_root_key", dirZ.d_name);
						if((fp = fopen(temp, "rb"))!=NULL) break; 
					}
				}
				sysLv2FsCloseDir(dfdZ);
			}
		}
	}
	
	if(fp!=NULL) print_load("* eid_root_key found ! = %s", temp);
	if(fp==NULL) {
		print_load("* eid_root_key not found !");
		if(is_dumpable() == YES) {
			print_load("* Dumping eid_root_key...");
			if(dump_root_key()==SUCCESS) return SUCCESS;
			else return FAILED;
		}
		else {
			return FAILED;
		}
	}

	fread(eid_root_key, sizeof(u8), EID_ROOT_KEY_SIZE, fp);
	
	fclose(fp);
	
	
	return SUCCESS;
}

int swap_kernel()
{

	u32 sourceR, read;
	u32 sourceW, write;
	
	u64 i,j,k;
	u64 offset_ros[2];
	u64 ros[2][3][0x40];
	
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
		offset_ros[0]=0x400;
		offset_ros[1]=0x3C00;
	} else {
		offset_ros[0]=0x600;
		offset_ros[1]=0x3E00;
	}
	
	read_device( sourceR, offset_ros[0]  , 0x1, ros[0][0], &read, 0x22 );
	read_device( sourceR, offset_ros[0]+1, 0x1, ros[0][1], &read, 0x22 );
	read_device( sourceR, offset_ros[0]+2, 0x1, ros[0][2], &read, 0x22 );
	
	read_device( sourceR, offset_ros[1]  , 0x1, ros[1][0], &read, 0x22 );
	read_device( sourceR, offset_ros[1]+1, 0x1, ros[1][1], &read, 0x22 );
	read_device( sourceR, offset_ros[1]+2, 0x1, ros[1][2], &read, 0x22 );
	
	close_device( sourceR );
	
	int swaped = NO;
	
	for(k=0; k<2; k++) {
	
		int n[3] = {-1,-1};
		int nC[3]= {-1,-1};
		int nD[3]= {-1,-1};
		
		for(j=0; j<3; j++) {
			for(i = 0; i < 0x40; i++) {
				if(!memcmp(&ros[k][j][i], "lv2_kernel.self", 15)) {
					n[0]=j; n[1]=i;
				}
				if(!memcmp(&ros[k][j][i], "lv2Ckernel.self", 15)) {
					nC[0]=j; nC[1]=i;
				}
				if(!memcmp(&ros[k][j][i], "lv2Dkernel.self", 15)) {
					nD[0]=j; nD[1]=i;
				}
			}
		}

		if(n[1]>0) {
			if(nD[1] > 0) { // swap to DEX
				if(eid0_is_CEX()==NO) {
					memcpy(&ros[k][nD[0]][nD[1]], "lv2_kernel.self", 15);
					memcpy(&ros[k][ n[0]][ n[1]], "lv2Ckernel.self", 15);
					swaped=YES;
				}
			} else
			if(nC[1] > 0) { //swap to CEX
				if(eid0_is_CEX()==YES) {
					memcpy(&ros[k][nC[0]][nC[1]], "lv2_kernel.self", 15);
					memcpy(&ros[k][ n[0]][ n[1]], "lv2Dkernel.self", 15);
					swaped=YES;
				}
			}
		}
	}
	
	if(swaped == NO) return FAILED;
	
	ret = 1;
	ret = open_device( 0x100000000000004ULL, &sourceW );
	if( ret != 0 ) {  //PS3 has nand
		close_device( sourceW );
		open_device( 0x100000000000001ULL, &sourceW );
	}
	
	write_device( sourceW, offset_ros[0]	, 0x1, ros[0][0], &write, 0x22 );
	write_device( sourceW, offset_ros[0]+1	, 0x1, ros[0][1], &write, 0x22 );
	write_device( sourceW, offset_ros[0]+2	, 0x1, ros[0][2], &write, 0x22 );
	
	write_device( sourceW, offset_ros[1]	, 0x1, ros[1][0], &write, 0x22 );
	write_device( sourceW, offset_ros[1]+1	, 0x1, ros[1][1], &write, 0x22 );
	write_device( sourceW, offset_ros[1]+2	, 0x1, ros[1][2], &write, 0x22 );
	
	close_device( sourceW );
	
	return SUCCESS;
	
}

int SaveEID0_s0_IDPS()
{

	u32 sourceR, sourceW;
	u32 read,	 write;
	u64 offset, sector[0x40];
	u8 eid0s0_e[0xC0];
	u8 eid0s0_d[0xC0];
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
		print_load("* Reading NAND");
    	offset = 0x204; // * 0x200
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
	}
	else {
		offset = 0x178; // * 0x200 Nor
		print_load("* Reading NOR");
	}

	read_device( sourceR, offset, 0x1, sector, &read, 0x22 );
	close_device( sourceR );
	
	
	if(lv2peek(OFFSET_1_IDPS) == sector[0xE] && lv2peek(OFFSET_1_IDPS + 0x8) == sector[0xF]) {
		print_load("Error : this IDPS is already written in the EID0");
		return FAILED;
	}
	
	//*** ANTI IDPS de PSP
	if(IDPS[5] < 0x10) {
		print_load("Error : You can't save in EID0 an IDPS from PSP");
		return FAILED;
	}
	
	//*** ANTI D2C&C2D ***
	if(0x0000000100820000 <= sector[0x0E] && sector[0x0E] < 0x0000000100830000) { // EID0 is DEX
		if(IDPS[0x5] != 0x82) { 													  // LV2 is CEX
			if(rebug==NO) {
				print_load("Error : Install a rebug firmware to convert your system to CEX");
				return FAILED;
			} else {
				print_load("Error : Please use the option to convert your system to CEX");
				return FAILED;
			}			
		}
	} else {	 				//EID0 is CEX
		if(IDPS[0x5] == 0x82) { // LV2 is DEX
			if(rebug==NO) {
				print_load("Error : Install a rebug firmware to convert your system to DEX");
				return FAILED;
			} else {
				print_load("Error : Please use the option to convert your system to DEX");
				return FAILED;
			}
		}
	}
	// ****************
	
	memcpy(eid0s0_e, &sector[0x12], 0xC0);

	print_load("* Getting EID ROOT KEY");
	if(get_root_key()==FAILED) {
		print_load("Error : Can't get the root key");
		return FAILED;
	}
	
	print_load("* Decrypt EID0 section 0");
	if( eid0_decrypt_section(eid0s0_e, eid0s0_d, 0) == FAILED) {
		return FAILED;
	}
	
	print_load("* Changing IDPS");
	memcpy(&eid0s0_d, &IDPS, 0x10);
	memset(eid0s0_e, 0, 0xC0);
	
	print_load("* Encrypt EID0 section 0");
	eid0_encrypt_section(eid0s0_d, eid0s0_e, 0);
	
	memcpy(&sector[0xE], &IDPS , 0x10);
	memcpy(&sector[0x12], eid0s0_e, 0xC0);
	
	ret = 1;
	ret = open_device( 0x100000000000004ULL, &sourceW );
	if( ret != 0 ) {
		print_load("* Writing in NAND");
		offset = 0x204;
		close_device( sourceW );
		open_device( 0x100000000000001ULL, &sourceW );
	}
	else {
		print_load("* Writing in NOR");
		offset = 0x178;
	}

	write_device( sourceW, offset, 0x1, sector, &write, 0x22 );
	close_device( sourceW );
	
	return SUCCESS;
}

int SaveEID0_s6_IDPS()
{
	u32 sourceR, read;
	u32 sourceW, write;
	
	u64 offset, sector[ 0x40 ];
	u8 eid0s6_e[0xC0];
	u8 eid0s6_d[0xC0];
	
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {
		print_load("* Reading in NAND");
    	offset = 0x206; // *0x200 Nand
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
	}
	else {
		print_load("* Writing in NOR");
		offset = 0x17A; // *0x200 NOR
	}

	read_device( sourceR, offset, 0x1, sector, &read, 0x22 );
	close_device( sourceR );
	
	memcpy(eid0s6_e, &sector[0x22], 0xC0);

	print_load("* Decrypt EID0 section 6");
	if( eid0_decrypt_section(eid0s6_e, eid0s6_d, 6) == FAILED) {
		return FAILED;
	}
	
	print_load("* Changing IDPS");
	memcpy(&eid0s6_d, &IDPS, 0x10);
	memset(eid0s6_e, 0, 0xC0);
	
	print_load("* Encrypt EID0 section 6");
	eid0_encrypt_section(eid0s6_d, eid0s6_e, 6);
	
	memcpy(&sector[0x22], eid0s6_e, 0xC0);
	
	ret = 1;
	ret = open_device( 0x100000000000004ULL, &sourceW );
	if( ret != 0 ) {  //PS3 has nand
		print_load("* Writing in NAND");
		offset = 0x206; // * 0x200
		close_device( sourceW );
		open_device( 0x100000000000001ULL, &sourceW );
	}
	else {
		print_load("* Writing in NOR");
		offset = 0x17A; // * 0x200
	}
	
	write_device( sourceW, offset, 0x1, sector, &write, 0x22 );
	close_device( sourceW );
	
	return SUCCESS;
	
}

int SaveEID0_IDPS()
{
	print_load("Writing IDPS in EID0 section 0");
	if(SaveEID0_s0_IDPS()==FAILED) {
		print_load("Error : failed to write IDPS in section 0 of the EID0");
		return FAILED;
	}
	
	print_load("Writing IDPS in EID0 section 6");
	if(SaveEID0_s6_IDPS()==FAILED) {
		print_load("Error : failed to write IDPS in section 6 of the EID0");
		return FAILED;
	}
	
	return SUCCESS ;
}

int SaveEID5_IDPS()
{
	u32 sourceR, read;
	u32 sourceW, write;
	u64 offset, sector[ 0x40 ];
	
	int ret = 1;
	
	//EID5
    ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
    	offset = 0x20D; // * 0x200
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
	}
	else offset = 0x181; // * 0x200 NOR

	read_device( sourceR, offset, 0x1, sector, &read, 0x22 );
	close_device( sourceR );
	
	memcpy(&sector[0x3A], &IDPS, 0x10);
	
	ret = 1;
	ret = open_device( 0x100000000000004ULL, &sourceW );
    if( ret != 0 ) {  //PS3 has nand
    	offset = 0x20D; // * 0x200
		close_device( sourceW );
		open_device( 0x100000000000001ULL, &sourceW );
	}
	else offset = 0x181; // * 0x200
	
    write_device( sourceW, offset, 0x1, sector, &write, 0x22 );
	close_device( sourceW );
	
	return SUCCESS;
}

int make_dumps()
{
	FILE *f;
	
	u32 source, read;
	u64 sector[0x40];
	u64 sector2convert[0x40];
	u8 eid0s0_e[0xC0];
	u8 eid0s0_d[0xC0];
	
	char f1[128];
	char f2[128];
	
	u8 is_NOR ;
	
	int ret = -1;
	u64 i;
	u64 sector_EID0_s0;
	u64 size_dump;
	
    ret = open_device( 0x100000000000004ULL, &source );
    if( ret != 0 ) { 
		is_NOR=NO;
		sector_EID0_s0=0x204;
		size_dump=0x77E00;
    	close_device(source);
		open_device(0x100000000000001ULL, &source);
	} 
	else {
		is_NOR=YES;
		sector_EID0_s0=0x178;
		size_dump=0x8000 ;
	}
	
	if(eid0_is_CEX()) {
		strcpy(f1, "/dev_hdd0/tmp/CEX-FLASH.EID0");
		strcpy(f2, "/dev_hdd0/tmp/DEX-FLASH.EID0");
	} else {
		strcpy(f1, "/dev_hdd0/tmp/DEX-FLASH.EID0");
		strcpy(f2, "/dev_hdd0/tmp/CEX-FLASH.EID0");
	}
	
	if(is_NOR==YES) {
		strcat(f1, ".NORBIN");
		strcat(f2, ".NORBIN");
	} else {
		strcat(f1, ".NANDBIN");
		strcat(f2, ".NANDBIN");
	}
	
	f = fopen(f1, "wb");
	print_load("Dumping to %s", f1);
	for(i=0x0; i < size_dump; i+=0x1) {
		read_device( source, i, 0x1, sector, &read, 0x22 );
		if(i==sector_EID0_s0) memcpy(sector2convert, sector, 0x200);
		fwrite(sector, sizeof(sector), 1, f);
		prog_bar = (i*100)/size_dump;
	}
	fclose(f);
	close_device(source);
	SetFilePerms(f1);
	
	prog_bar=-1;
	
	print_load("Dumping to %s", f2);
	ret = CopyFile(f1, f2);
	if(ret!=0) {print_load("Error : Failed to copy dump file"); return FAILED;}
	
	memcpy(eid0s0_e, &sector2convert[0x12], 0xC0);
	
	if(get_root_key()==FAILED) {
		print_load("Error : Can't get the root key");
		return FAILED;
	}
	
	if(eid0_decrypt_section(eid0s0_e, eid0s0_d, 0) == FAILED) {
		return FAILED;
	}
	
	if(eid0_is_CEX()==YES) {
		if(sector2convert[0xE] < 0x0000000100100000ULL) { //PSP IDPS
			sector2convert[0xE] += 0x800000;
		}
		while(sector2convert[0xE] >= 0x0000000100830000ULL )  {
			sector2convert[0xE] -= 0x10000;
		}
		memset(&eid0s0_d[0x5], 0x82, 0x1);
	} else {
		u8 target = target_from_eid5();
		if(target - (target >> 4)*0x10 < 0x3 && 0xF < target - (target >> 4)*0x10) {
			target=choose_target();
		}
		sector2convert[0xE] -= 0x820000 ;
		sector2convert[0xE] += target*0x10000;
		memcpy(&eid0s0_d[0x5], &target, 0x1);
	}
	
	memset(eid0s0_e, 0, 0xC0);
	eid0_encrypt_section(eid0s0_d, eid0s0_e, 0);
	
	memcpy(&sector2convert[0x12], eid0s0_e, 0xC0);
	
	f=fopen(f2, "r+");
	if(f==NULL) {print_load("Error: Failed to edit converted dump"); return FAILED;}
	fseek(f, sector_EID0_s0*0x200, SEEK_SET);
	fwrite(sector2convert, sizeof(u64), 0x40, f);
	fclose(f);
	
	sysFSStat s;
    ret= sysLv2FsStat("/dev_usb000", &s);
    if(ret) return SUCCESS;

	if(is_NOR==YES) {
		print_load("Copy file to /dev_usb000/CEX-FLASH.EID0.NORBIN");
		ret=CopyFile("/dev_hdd0/tmp/CEX-FLASH.EID0.NORBIN", "/dev_usb000/CEX-FLASH.EID0.NORBIN");
		if(ret==0) Delete("/dev_hdd0/tmp/CEX-FLASH.EID0.NORBIN");
		else print_load("Error : Failed to copy");
		
		print_load("Copy file to /dev_usb000/DEX-FLASH.EID0.NORBIN");
		ret=CopyFile("/dev_hdd0/tmp/DEX-FLASH.EID0.NORBIN", "/dev_usb000/DEX-FLASH.EID0.NORBIN");
		if(ret==0 )	Delete("/dev_hdd0/tmp/DEX-FLASH.EID0.NORBIN");
		else print_load("Error : Failed to copy");
	} 
	else {
		print_load("Copy file to /dev_usb000/CEX-FLASH.EID0.NANDBIN");
		ret=CopyFile("/dev_hdd0/tmp/CEX-FLASH.EID0.NANDBIN", "/dev_usb000/CEX-FLASH.EID0.NANDBIN");
		if(ret==0) Delete("/dev_hdd0/tmp/CEX-FLASH.EID0.NANDBIN");
		else print_load("Error : Failed to copy");
		
		print_load("Copy file to /dev_usb000/DEX-FLASH.EID0.NANDBIN");
		ret=CopyFile("/dev_hdd0/tmp/DEX-FLASH.EID0.NANDBIN", "/dev_usb000/DEX-FLASH.EID0.NANDBIN");
		if(ret==0)	Delete("/dev_hdd0/tmp/DEX-FLASH.EID0.NANDBIN"); 
		else print_load("Error : Failed to copy");
	}
	
	return SUCCESS;
}

int swap_upd_plugin()
{
	sysFSStat s;
	
	if(sysLv2FsStat("/dev_blind", &s) != 0)	sys_fs_mount("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_blind", 0);
	
	sleep(1); //useless ?
	
	if(eid0_is_CEX()==YES) {
		if(sysLv2FsStat("/dev_blind/vsh/module/software_update_plugin.sprx.cex", &s) == 0) {
			if(sysLv2FsRename("/dev_blind/vsh/module/software_update_plugin.sprx", "/dev_blind/vsh/module/software_update_plugin.sprx.dex") == 0) {
				if(sysLv2FsRename("/dev_blind/vsh/module/software_update_plugin.sprx.cex", "/dev_blind/vsh/module/software_update_plugin.sprx") !=0)  {
					print_load("Error : failed to rename CEX pluging");
					return FAILED;
				} return SUCCESS;
			} {print_load("Error : failed to rename DEX pluging"); return FAILED;}
		} else {print_load("Error : CEX pluging not found"); return FAILED;}
	}
	else {
		if(sysLv2FsStat("/dev_blind/vsh/module/software_update_plugin.sprx.dex", &s) == 0) {
			if(sysLv2FsRename("/dev_blind/vsh/module/software_update_plugin.sprx", "/dev_blind/vsh/module/software_update_plugin.sprx.cex") == 0) {
				if(sysLv2FsRename("/dev_blind/vsh/module/software_update_plugin.sprx.dex", "/dev_blind/vsh/module/software_update_plugin.sprx") !=0)  {
					print_load("Error : failed to rename DEX pluging");
					return FAILED;
				} return SUCCESS;
			} {print_load("Error : failed to rename CEX pluging"); return FAILED;}
		} else {print_load("Error : DEX pluging not found"); return FAILED;}
	}

}

int Convert()
{
	u32 sourceR, sourceW;
	u32 read,	 write;
	u64 offset, sector[0x40];
	u8 eid0s0_e[0xC0];
	u8 eid0s0_d[0xC0];
	int ret = 1;
	
	ret = open_device( 0x100000000000004ULL, &sourceR );
    if( ret != 0 ) {  // Nand
		print_load("Reading NAND");
    	offset = 0x204; // * 0x200
		close_device( sourceR );
		open_device( 0x100000000000001ULL, &sourceR );
	}
	else {
		print_load("Reading NOR");
		offset = 0x178; // * 0x200 Nor 
	}
	read_device( sourceR, offset, 0x1, sector, &read, 0x22 );
	close_device( sourceR );
	
	memcpy(eid0s0_e, &sector[0x12], 0xC0);
	
	print_load("Getting EID ROOT KEY");
	if(get_root_key()==FAILED) {
		print_load("Error : Can't get the root key");
		return FAILED;
	}
	
	print_load("Decrypting EID0 section 0");
	if( eid0_decrypt_section(eid0s0_e, eid0s0_d, 0) == FAILED) {
		return FAILED;
	}
	
	if(eid0_is_CEX()==YES) {
		print_load("Changing target ID to 0x82");
		while(sector[0xE] >= 0x0000000100830000ULL )  {
			sector[0xE] -= 0x10000;
		}
		memset(&eid0s0_d[0x5], 0x82, 0x1);
	} else {
		u8 target = target_from_eid5();
		if(target - (target >> 4)*0x10 < 0x3 && 0xF < target - (target >> 4)*0x10) {
			target=choose_target();
		}
		print_load("Changing target ID to 0x%X", target);
		sector[0xE] -= 0x820000 ;
		sector[0xE] += target*0x10000;
		
		memcpy(&eid0s0_d[0x5], &target, 0x1);
	}
	
	memset(eid0s0_e, 0, 0xC0);
	
	print_load("Encrypt EID0 section 0");
	eid0_encrypt_section(eid0s0_d, eid0s0_e, 0);
	
	memcpy(&sector[0x12], eid0s0_e, 0xC0);
	
	ret = 1;
	ret = open_device( 0x100000000000004ULL, &sourceW );
	if( ret != 0 ) {
		print_load("Writing in the NAND");
		offset = 0x204; 
		close_device( sourceW );
		open_device( 0x100000000000001ULL, &sourceW );
	}
	else {
		offset = 0x178;
		print_load("Writing in the NOR");
	}

	write_device( sourceW, offset, 0x1, sector, &write, 0x22 );
	close_device( sourceW );
	
	print_load("Swaping update plugin");
	if(swap_upd_plugin() == FAILED) {
		print_load("Error : failed to swap update plugin");
		return FAILED;
	}
	
	print_load("Swaping kernel");
	if(swap_kernel()==SUCCESS) {
		print_load("The system is going to reboot now...");
		sleep(3);
		Delete("/dev_hdd0/tmp/turnoff");
		reboot();
	} 
	
	return FAILED;
	
}

void LoadEID5_IDPS()
{
	u32 source, read;
	u64 offset;
    u64 buffer[ 0x40 ];
    int ret = 1;
    
    ret = open_device( 0x100000000000004ULL, &source );
    
    if( ret != 0 ) {
		offset = 0x20D; 
		close_device( source );
		open_device( 0x100000000000001ULL, &source );
	}
	else {
		offset = 0x181;
	}
   
    read_device( source, offset, 0x1, buffer, &read, 0x22 );
	close_device( source );
	
	lv2poke(OFFSET_1_IDPS  , buffer[ 0x3A ]);
	lv2poke(OFFSET_1_IDPS+8, buffer[ 0x3B ]);
	
	lv2poke(OFFSET_2_IDPS  , buffer[ 0x3A ]);
	lv2poke(OFFSET_2_IDPS+8, buffer[ 0x3B ]);
}

void LoadEID0_IDPS()
{
	u32 source, read;
	u64 offset;
    u64 buffer[ 0x40 ];
    int ret = 1;
    
    ret = open_device( 0x100000000000004ULL, &source );
    if( ret != 0 ) {
		offset = 0x204; 
		close_device( source );
		open_device( 0x100000000000001ULL, &source );
	}
	else offset = 0x178;

   
    read_device( source, offset, 0x1, buffer, &read, 0x22 );
	close_device( source );
	
	lv2poke(OFFSET_1_IDPS  , buffer[ 0x0E ]);
	lv2poke(OFFSET_1_IDPS+8, buffer[ 0x0F ]);
	
	lv2poke(OFFSET_2_IDPS  , buffer[ 0x0E ]);
	lv2poke(OFFSET_2_IDPS+8, buffer[ 0x0F ]);
	
}

int load_modules()
{
	int ret;
	ret = sysModuleLoad(SYSMODULE_IO);
	if(ret != 0) return -1;
	ret = sysModuleLoad(SYSMODULE_FS);
	if(ret != 0) return -2;
	ret = sysModuleLoad(SYSMODULE_PNGDEC);
	if(ret != 0) return -3;
	
	return 0;
}

int unload_modules()
{
	int ret;
	ret = sysModuleUnload(SYSMODULE_IO);
	if(ret != 0) return -1;
	ret = sysModuleUnload(SYSMODULE_FS);
	if(ret != 0) return -2;
	ret = sysModuleUnload(SYSMODULE_PNGDEC);
	if(ret != 0) return -3;
	
	return 0;
}

int main()
{
	int i=1;
	
	int module = load_modules();
	
	Init_Graph();
	ioPadInit(7);
	SetCurrentFont(-1);
	
	if(module != 0) {
		while(1) {
			cls();
			Draw_Header();

			SetFontAutoCenter(0);
			SetFontSize(20, 20);
			SetFontColor(RED, 0x0);
			
			DrawFormatString(50, 200, "Error: Failed to load module = %d", module);
			
			SetFontColor(WHITE, 0x0);
			i=Draw_Button_Circle(50, 485, 15);
			i=DrawString( i+5, 485, "Back to XMB"); 
			
			tiny3d_Flip();
			ps3pad_read();
			if(new_pad & BUTTON_CIRCLE) {unload_modules(); return 0;}
		}
	}
	
	if(lv2peek(0x8000000000003000ULL)==0xFFFFFFFF80010003ULL) {
		while(1) {
			cls();
			Draw_Header();

			SetFontAutoCenter(0);
			SetFontSize(20, 20);
			SetFontColor(RED, 0x0);
			
			DrawString(50, 200, "Error: peek&poke disabled");
			
			SetFontColor(WHITE, 0x0);
			i=Draw_Button_Circle(50, 485, 15);
			i=DrawString( i+5, 485, "Back to XMB"); 
			
			tiny3d_Flip();
			ps3pad_read();
			if(new_pad & BUTTON_CIRCLE) {unload_modules(); return 0;}
		}
	}
	
	if(init_IDPSet()==FAILED) {
		while(1) {
			cls();
			Draw_Header();
			
			SetFontAutoCenter(0);
			SetFontSize(20, 20);
			SetFontColor(RED, 0x0);
			
			DrawString(50, 200, "Error: Unsupported firmware");
			
			SetFontColor(WHITE, 0x0);
			i=Draw_Button_Circle(50, 485, 15);
			i=DrawString( i+5, 485, "Back to XMB"); 
			
			tiny3d_Flip();
			ps3pad_read();
			if(new_pad & BUTTON_CIRCLE) {unload_modules(); return 0;}
		}
	}
	
	while(1) {
		cls();
		Draw_Header();
		Draw_IDPS();
		Draw_menu();
		Draw_buttons();
		Draw_Notification();
		
		tiny3d_Flip();
		
		ps3pad_read();
		
		if(new_pad & BUTTON_DOWN) {
			if(position < 100) {
				if(position == 5 && is_dumpable() == YES) position=6; else
				if(position == 5 && is_dumpable() == NO && rebug == YES) position=7; else
				if(position == 6 && rebug == YES) position=7; else
				if(position < 5) position++;
			} else
			if(position == 100) {
				if(IDPS[0x5] - (IDPS[0x5] >> 4)*0x10 > 0x2) {
					IDPS[0x5]--;
					lv2poke8(OFFSET_1_IDPS + 0x5, IDPS[0x5]);
					lv2poke8(OFFSET_2_IDPS + 0x5, IDPS[0x5]);
				}
			} else
			if(position == 101) {
				if(IDPS[0x7]>0x01) {
					IDPS[0x7]--;
					lv2poke8(OFFSET_1_IDPS + 0x7, IDPS[0x7]);
					lv2poke8(OFFSET_2_IDPS + 0x7, IDPS[0x7]);
				}
			} else
			if(position == 102) {
				if((IDPS[0x8] >> 4) == 0x1) {
					IDPS[0x8] = IDPS[0x8] -(IDPS[0x8] >> 4)*0x10;
				} else
				if((IDPS[0x8] >> 4) == 0xF) {
					IDPS[0x8] = IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0x8, IDPS[0x8]);
				lv2poke8(OFFSET_2_IDPS + 0x8, IDPS[0x8]);
			} else
			if(position == 103) {
				if(IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 == 0x4) {
					IDPS[0x8]= (IDPS[0x8] >> 4)*0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0x8, IDPS[0x8]);
				lv2poke8(OFFSET_2_IDPS + 0x8, IDPS[0x8]);
			} else
			if(position == 104) {
				if((IDPS[0x9] >> 4) > 0x0) {
					IDPS[0x9]= IDPS[0x9] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0x9, IDPS[0x9]);
				lv2poke8(OFFSET_2_IDPS + 0x9, IDPS[0x9]);
			} else
			if(position == 105) {
				if(IDPS[0x9] - (IDPS[0x9] >> 4)*0x10 > 0x0) {
					IDPS[0x9]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0x9, IDPS[0x9]);
				lv2poke8(OFFSET_2_IDPS + 0x9, IDPS[0x9]);
			} else
			if(position == 106) {
				if((IDPS[0xA] >> 4) > 0x0) {
					IDPS[0xA] = IDPS[0xA] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xA, IDPS[0xA]);
				lv2poke8(OFFSET_2_IDPS + 0xA, IDPS[0xA]);
			} else
			if(position == 107) {
				if(IDPS[0xA] - (IDPS[0xA] >> 4)*0x10 > 0x0) {
					IDPS[0xA]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xA, IDPS[0xA]);
				lv2poke8(OFFSET_2_IDPS + 0xA, IDPS[0xA]);
			}  else
			if(position == 108) {
				if((IDPS[0xB] >> 4) > 0x0) {
					IDPS[0xB] = IDPS[0xB] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xB, IDPS[0xB]);
				lv2poke8(OFFSET_2_IDPS + 0xB, IDPS[0xB]);
			} else
			if(position == 109) {
				if(IDPS[0xB] - (IDPS[0xB] >> 4)*0x10 > 0x0) {
					IDPS[0xB]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xB, IDPS[0xB]);
				lv2poke8(OFFSET_2_IDPS + 0xB, IDPS[0xB]);
			}  else
			if(position == 110) {
				if((IDPS[0xC] >> 4) > 0x0) {
					IDPS[0xC] = IDPS[0xC] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xC, IDPS[0xC]);
				lv2poke8(OFFSET_2_IDPS + 0xC, IDPS[0xC]);
			} else
			if(position == 111) {
				if(IDPS[0xC] - (IDPS[0xC] >> 4)*0x10 > 0x0) {
					IDPS[0xC]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xC, IDPS[0xC]);
				lv2poke8(OFFSET_2_IDPS + 0xC, IDPS[0xC]);
			}  else
			if(position == 112) {
				if((IDPS[0xD] >> 4) > 0x0) {
					IDPS[0xD] = IDPS[0xD] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xD, IDPS[0xD]);
				lv2poke8(OFFSET_2_IDPS + 0xD, IDPS[0xD]);
			} else
			if(position == 113) {
				if(IDPS[0xD] - (IDPS[0xD] >> 4)*0x10 > 0x0) {
					IDPS[0xD]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xD, IDPS[0xD]);
				lv2poke8(OFFSET_2_IDPS + 0xD, IDPS[0xD]);
			}  else
			if(position == 114) {
				if((IDPS[0xE] >> 4) > 0x0) {
					IDPS[0xE] = IDPS[0xE] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xE, IDPS[0xE]);
				lv2poke8(OFFSET_2_IDPS + 0xE, IDPS[0xE]);
			} else
			if(position == 115) {
				if(IDPS[0xE] - (IDPS[0xE] >> 4)*0x10 > 0x0) {
					IDPS[0xE]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xE, IDPS[0xE]);
				lv2poke8(OFFSET_2_IDPS + 0xE, IDPS[0xE]);
			}  else
			if(position == 116) {
				if((IDPS[0xF] >> 4) > 0x0) {
					IDPS[0xF] = IDPS[0xF] - 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xF, IDPS[0xF]);
				lv2poke8(OFFSET_2_IDPS + 0xF, IDPS[0xF]);
			} else
			if(position == 117) {
				if(IDPS[0xF] - (IDPS[0xF] >> 4)*0x10 > 0x0) {
					IDPS[0xF]--;
				}
				lv2poke8(OFFSET_1_IDPS + 0xF, IDPS[0xF]);
				lv2poke8(OFFSET_2_IDPS + 0xF, IDPS[0xF]);
			}
		}
		
		if(new_pad & BUTTON_UP) {
			if(position < 100) {
				if(position == 7 && is_dumpable() == NO ) position=5; else
				if(position > 0) position--;
			
			} else
			if(position == 100) {
				if(IDPS[0x5] - (IDPS[0x5] >> 4)*0x10 < 0xF) {
					IDPS[0x5]++;
					lv2poke8(OFFSET_1_IDPS + 0x5, IDPS[0x5]);
					lv2poke8(OFFSET_2_IDPS + 0x5, IDPS[0x5]);
				}
			} else
			if(position == 101) {
				if(IDPS[0x7]<0x14) {
					IDPS[0x7]++;
					lv2poke8(OFFSET_1_IDPS + 0x7, IDPS[0x7]);
					lv2poke8(OFFSET_2_IDPS + 0x7, IDPS[0x7]);
				}
			} else
			if(position == 102) {
				if((IDPS[0x8] >> 4) == 0x1) {
					IDPS[0x8] = IDPS[0x8] -(IDPS[0x8] >> 4)*0x10 + 0xF0;
				} else
				if((IDPS[0x8] >> 4) == 0x0) {
					IDPS[0x8] = IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0x8, IDPS[0x8]);
				lv2poke8(OFFSET_2_IDPS + 0x8, IDPS[0x8]);
			} else
			if(position == 103) {
				if(IDPS[0x8] - (IDPS[0x8] >> 4)*0x10 == 0x0) {
					IDPS[0x8]= (IDPS[0x8] >> 4)*0x10 + 0x4;
				}
				lv2poke8(OFFSET_1_IDPS + 0x8, IDPS[0x8]);
				lv2poke8(OFFSET_2_IDPS + 0x8, IDPS[0x8]);
			} else
			if(position == 104) {
				if((IDPS[0x9] >> 4) < 0x3) {
					IDPS[0x9]= IDPS[0x9] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0x9, IDPS[0x9]);
				lv2poke8(OFFSET_2_IDPS + 0x9, IDPS[0x9]);
			} else
			if(position == 105) {
				if(IDPS[0x9] - (IDPS[0x9] >> 4)*0x10 < 0xF) {
					IDPS[0x9]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0x9, IDPS[0x9]);
				lv2poke8(OFFSET_2_IDPS + 0x9, IDPS[0x9]);
			} else
			if(position == 106) {
				if((IDPS[0xA] >> 4) < 0xF) {
					IDPS[0xA] = IDPS[0xA] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xA, IDPS[0xA]);
				lv2poke8(OFFSET_2_IDPS + 0xA, IDPS[0xA]);
			} else
			if(position == 107) {
				if(IDPS[0xA] - (IDPS[0xA] >> 4)*0x10 < 0xF) {
					IDPS[0xA]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xA, IDPS[0xA]);
				lv2poke8(OFFSET_2_IDPS + 0xA, IDPS[0xA]);
			}  else
			if(position == 108) {
				if((IDPS[0xB] >> 4) < 0xF) {
					IDPS[0xB] = IDPS[0xB] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xB, IDPS[0xB]);
				lv2poke8(OFFSET_2_IDPS + 0xB, IDPS[0xB]);
			} else
			if(position == 109) {
				if(IDPS[0xB] - (IDPS[0xB] >> 4)*0x10 < 0xF) {
					IDPS[0xB]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xB, IDPS[0xB]);
				lv2poke8(OFFSET_2_IDPS + 0xB, IDPS[0xB]);
			}  else
			if(position == 110) {
				if((IDPS[0xC] >> 4) < 0xF) {
					IDPS[0xC] = IDPS[0xC] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xC, IDPS[0xC]);
				lv2poke8(OFFSET_2_IDPS + 0xC, IDPS[0xC]);
			} else
			if(position == 111) {
				if(IDPS[0xC] - (IDPS[0xC] >> 4)*0x10 < 0xF) {
					IDPS[0xC]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xC, IDPS[0xC]);
				lv2poke8(OFFSET_2_IDPS + 0xC, IDPS[0xC]);
			}  else
			if(position == 112) {
				if((IDPS[0xD] >> 4) < 0xF) {
					IDPS[0xD] = IDPS[0xD] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xD, IDPS[0xD]);
				lv2poke8(OFFSET_2_IDPS + 0xD, IDPS[0xD]);
			} else
			if(position == 113) {
				if(IDPS[0xD] - (IDPS[0xD] >> 4)*0x10 < 0xF) {
					IDPS[0xD]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xD, IDPS[0xD]);
				lv2poke8(OFFSET_2_IDPS + 0xD, IDPS[0xD]);
			}  else
			if(position == 114) {
				if((IDPS[0xE] >> 4) < 0xF) {
					IDPS[0xE] = IDPS[0xE] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xE, IDPS[0xE]);
				lv2poke8(OFFSET_2_IDPS + 0xE, IDPS[0xE]);
			} else
			if(position == 115) {
				if(IDPS[0xE] - (IDPS[0xE] >> 4)*0x10 < 0xF) {
					IDPS[0xE]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xE, IDPS[0xE]);
				lv2poke8(OFFSET_2_IDPS + 0xE, IDPS[0xE]);
			}  else
			if(position == 116) {
				if((IDPS[0xF] >> 4) < 0xF) {
					IDPS[0xF] = IDPS[0xF] + 0x10;
				}
				lv2poke8(OFFSET_1_IDPS + 0xF, IDPS[0xF]);
				lv2poke8(OFFSET_2_IDPS + 0xF, IDPS[0xF]);
			} else
			if(position == 117) {
				if(IDPS[0xF] - (IDPS[0xF] >> 4)*0x10 < 0xF) {
					IDPS[0xF]++;
				}
				lv2poke8(OFFSET_1_IDPS + 0xF, IDPS[0xF]);
				lv2poke8(OFFSET_2_IDPS + 0xF, IDPS[0xF]);
			}
		}
		
		if(new_pad & BUTTON_RIGHT) {
			if(100 <= position && position < 117) {
				position++;
			}
		}
		
		if(new_pad & BUTTON_LEFT) {
			if(100 < position && position <= 117) {
				position--;
			}
		}
		
		if(new_pad & BUTTON_CROSS) {
			if(position==0) {
				position=100;
			} else 
			if(position==1) {
				start_loading();
				i=SaveEID0_IDPS();
				end_loading();
				if(i==SUCCESS)	show_msg("Done !"); else show_msg("Failed !");
			} else
			if(position==2) {
				start_loading();
				i=SaveEID0_IDPS();
				if(i==SUCCESS) i=SaveEID5_IDPS();
				end_loading();
				if(i==SUCCESS)	show_msg("Done !"); else show_msg("Failed !");
			} else
			if(position==3) {
				start_loading();
				LoadEID0_IDPS();
				end_loading();
				show_msg("Done !");
			} else
			if(position==4) {
				start_loading();
				LoadEID5_IDPS();
				end_loading();
				show_msg("Done !");
			} else
			if(position==5) {
				start_loading();
				i=make_dumps();
				end_loading();
				if(i==SUCCESS)	show_msg("Done !"); else show_msg("Failed !");
			} else
			if(position==6) {
				start_loading();
				i=dump_root_key();
				end_loading();
				if(i==SUCCESS)	show_msg("Done !"); else show_msg("Failed !");
			} else
			if(position==7) {
				start_loading();
				i=Convert();
				end_loading();
				if(i==SUCCESS)	show_msg("Done !"); else show_msg("Failed !");
			}
		}
		
		if(new_pad & BUTTON_CIRCLE) {
			if(100 <= position && position <= 117) {
				position=0;
			} else {
				{unload_modules(); return 0;}
			}
		}
		
	}
}