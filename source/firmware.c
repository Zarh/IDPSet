
#include <sys/file.h>
#include "firmware.h"

#define SUCCESS		1
#define FAILED 		0

extern u8 rebug;
extern u64 *payload;
extern size_t payload_size;
extern u32 firmware;
extern u64 TOC_OFFSET;
extern u64 HV_START_OFFSET;
extern u64 HTAB_OFFSET;
extern u64 SYSCALL_TABLE_OFFSET;
extern u64 MMAP_OFFSET1;
extern u64 MMAP_OFFSET2;
extern u64 SPE_OFFSET;
extern u64 OFFSET_1_IDPS;
extern u64 OFFSET_2_IDPS;

extern int fw_is_rebug();
extern u64 lv2peek(u64 addr);

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
		
		payload_size = payload_421D_bin_size;
		payload = (u64 *) payload_421D_bin;
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
		
		payload_size = payload_430C_bin_size;
		payload = (u64 *) payload_430C_bin;
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
		
		payload_size = payload_430D_bin_size;
		payload = (u64 *) payload_430D_bin;
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
		
		payload_size = payload_431C_bin_size;
		payload = (u64 *) payload_431C_bin;
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
		
		payload_size = payload_440C_bin_size;
		payload = (u64 *) payload_440C_bin;
	} else
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
		
		payload_size = payload_441C_bin_size;
		payload = (u64 *) payload_441C_bin;
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
		
		payload_size = payload_441D_bin_size;
		payload = (u64 *) payload_441D_bin;
	} else
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
		
		payload_size = payload_446C_bin_size;
		payload = (u64 *) payload_446C_bin;
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
		
		payload_size = payload_446D_bin_size;
		payload = (u64 *) payload_446D_bin;
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
		
		payload_size = payload_450C_bin_size;
		payload = (u64 *) payload_450C_bin;
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
		
		payload_size = payload_450D_bin_size;
		payload = (u64 *) payload_450D_bin;
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
		
		payload_size = payload_453C_bin_size;
		payload = (u64 *) payload_453C_bin;
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
		
		payload_size = payload_453D_bin_size;
		payload = (u64 *) payload_453D_bin;
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
		
		payload_size = payload_455C_bin_size;
		payload = (u64 *) payload_455C_bin;
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
		
		payload_size = payload_455D_bin_size;
		payload = (u64 *) payload_455D_bin;
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
		
		payload_size = payload_460C_bin_size;
		payload = (u64 *) payload_460C_bin;
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
		
		payload_size = payload_465C_bin_size;
		payload = (u64 *) payload_465C_bin;
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
		
		payload_size = payload_465D_bin_size;
		payload = (u64 *) payload_465D_bin;
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
		
		payload_size = payload_470C_bin_size;
		payload = (u64 *) payload_470C_bin;
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
		
		payload_size = payload_470D_bin_size;
		payload = (u64 *) payload_470D_bin;
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
		
		payload_size = payload_475C_bin_size;
		payload = (u64 *) payload_475C_bin;
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
		
		payload_size = payload_475D_bin_size;
		payload = (u64 *) payload_475D_bin;
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
		
		payload_size = payload_476C_bin_size;
		payload = (u64 *) payload_476C_bin;
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
		
		payload_size = payload_476D_bin_size;
		payload = (u64 *) payload_476D_bin;
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
		
		payload_size = payload_478C_bin_size;
		payload = (u64 *) payload_478C_bin;
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
		
		payload_size = payload_478D_bin_size;
		payload = (u64 *) payload_478D_bin;
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
		
		payload_size = payload_480C_bin_size;
		payload = (u64 *) payload_480C_bin;
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
		
		payload_size = payload_480D_bin_size;
		payload = (u64 *) payload_480D_bin;
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
		
		payload_size = payload_481C_bin_size;
		payload = (u64 *) payload_481C_bin;
	} else
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
		
		payload_size = payload_481D_bin_size;
		payload = (u64 *) payload_481D_bin;
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
		
		payload_size = payload_482C_bin_size;
		payload = (u64 *) payload_482C_bin;
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
		
		payload_size = payload_482D_bin_size;
		payload = (u64 *) payload_482D_bin;
	} else
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
		
		payload_size = payload_483C_bin_size;
		payload = (u64 *) payload_483C_bin;
	} else
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
		
		payload_size = payload_484C_bin_size;
		payload = (u64 *) payload_484C_bin;
	} else
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
		
		payload_size = payload_484D_bin_size;
		payload = (u64 *) payload_484D_bin;
	} else
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
		
		payload_size = payload_485C_bin_size;
		payload = (u64 *) payload_485C_bin;
	} else
	if(( lv2peek(FW_DATE_OFFSET_486C    )==FW_DATE_1_486C) &&
	( lv2peek(FW_DATE_OFFSET_486C + 8)==FW_DATE_2_486C) )
	{
		firmware = 0x486C;
		
		TOC_OFFSET = TOC_OFFSET_486C;
		HV_START_OFFSET = HV_START_OFFSET_486C;
		HTAB_OFFSET = HTAB_OFFSET_486C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_486C;
		MMAP_OFFSET1 = MMAP_OFFSET1_486C;
		MMAP_OFFSET2 = MMAP_OFFSET2_486C;
		SPE_OFFSET = SPE_OFFSET_486C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_486C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_486C;
		
		payload_size = payload_486C_bin_size;
		payload = (u64 *) payload_486C_bin;
	} else
	if(( lv2peek(FW_DATE_OFFSET_487C    )==FW_DATE_1_487C) &&
	( lv2peek(FW_DATE_OFFSET_487C + 8)==FW_DATE_2_487C) )
	{
		firmware = 0x487C;
		
		TOC_OFFSET = TOC_OFFSET_487C;
		HV_START_OFFSET = HV_START_OFFSET_487C;
		HTAB_OFFSET = HTAB_OFFSET_487C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_487C;
		MMAP_OFFSET1 = MMAP_OFFSET1_487C;
		MMAP_OFFSET2 = MMAP_OFFSET2_487C;
		SPE_OFFSET = SPE_OFFSET_487C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_487C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_487C;
		
		payload_size = payload_487C_bin_size;
		payload = (u64 *) payload_487C_bin;
	} else
	if(( lv2peek(FW_DATE_OFFSET_488C    )==FW_DATE_1_488C) &&
	( lv2peek(FW_DATE_OFFSET_488C + 8)==FW_DATE_2_488C) )
	{
		firmware = 0x488C;
		
		TOC_OFFSET = TOC_OFFSET_488C;
		HV_START_OFFSET = HV_START_OFFSET_488C;
		HTAB_OFFSET = HTAB_OFFSET_488C;
		SYSCALL_TABLE_OFFSET = SYSCALL_TABLE_488C;
		MMAP_OFFSET1 = MMAP_OFFSET1_488C;
		MMAP_OFFSET2 = MMAP_OFFSET2_488C;
		SPE_OFFSET = SPE_OFFSET_488C;
		OFFSET_1_IDPS = OFFSET_1_IDPS_488C;
		OFFSET_2_IDPS = OFFSET_2_IDPS_488C;
		
		payload_size = payload_488C_bin_size;
		payload = (u64 *) payload_488C_bin;
	} else
	{
		u64 n;
		for(n=0x350000; n<0x4A0000; n++) {
			u64 data = lv2peek(0x8000000000000000ULL + n);
			
			if(0x350000 < n && n <0x450000) {
				if(OFFSET_1_IDPS == 0) {
					if(0x0000000100800000 < data && data <0x0000000100900000)
					{
						OFFSET_1_IDPS = 0x8000000000000000ULL + n;
					}
				}
			}
			if(0x450000 < n && n <0x4A0000) {
				if(OFFSET_2_IDPS == 0) {
					if(0x0000000100800000 < data && data <0x0000000100900000)
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
	sysLv2FsChmod("/dev_hdd0/tmp", 0777);
	
	return SUCCESS;
}
