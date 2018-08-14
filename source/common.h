#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define COUNT_OF(x) (sizeof(x) / sizeof((x)[0]))

#define KERNEL_BASE 0x8000000000000000ULL
#define KERNEL_ADDRESS(x) (KERNEL_BASE | (uint32_t)(x))
#define SYSCALL_OPD_PTR_OFFSET(n) (SYSCALL_TABLE_OFFSET + (n) * sizeof(uint64_t))
#define SYSCALL_OPD_OFFSET(n) lv2_peek64(SYSCALL_OPD_PTR_OFFSET(n))
#define SYSCALL_CODE_OFFSET(n) lv2_peek64(SYSCALL_OPD_OFFSET(n))

#define EID_ROOT_KEY_SIZE 0x30

#ifdef _DEBUG
	#define debug_printf(format, ...) console_printf(format, #__VA_ARGS__)
#else
	#define debug_printf(format, ...) while (0) {}
#endif

#define PAYLOAD_OFFSET       KERNEL_ADDRESS(0x7F0000)
#define PAYLOAD_OPD_OFFSET   KERNEL_ADDRESS(0x1830)

#define HTAB_BASE       KERNEL_ADDRESS(0xF000000)
#define HTAB_SIZE       0x40000ULL
#define HTAB_ENTRY_SIZE 0x10

#define SYSCALL_LV2_PEEK        6
#define SYSCALL_LV2_POKE        7
#define SYSCALL_LV1_PEEK        8
#define SYSCALL_LV1_POKE        9
#define SYSCALL_RUN_PAYLOAD     32
#define SYSCALL_HVC_REDIRECTOR  811
#define SYSCALL_POKE_WITH_CACHE 813
#define SYSCALL_SM_SHUTDOWN     379
#define SYSCALL_RING_BUZZER     392
#define SYSCALL_CONSOLE_WRITE   398

#define BIG_PICT 32

#define OK 			1
#define NOK 		0
#define YES			1
#define NO 			0
#define TRUE 		1
#define FALSE 		0
#define SUCCESS 	1
#define FAILED	 	0
#define ON			1
#define OFF			0

#define WHITE						0xFFFFFFFF
#define ORANGE 						0xFFA000FF
#define BLUE						0x0070FFFF
#define GREEN						0x00FF00FF
#define RED							0xFF0000FF

#endif
