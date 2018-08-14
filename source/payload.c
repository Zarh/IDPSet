#include "dumper.h"
#include "payload.h"
#include <ppu-lv2.h>

u64 *payload;
size_t payload_size;

static uint64_t real_opd_offset = 0;
extern u64 TOC_OFFSET;
extern u64 SYSCALL_TABLE_OFFSET;

int install_payload(void) {

	if (payload_size <= 0)	return -1;
	
	lv2_copy_from_user(payload, PAYLOAD_OFFSET, payload_size);

	lv2_poke64(PAYLOAD_OPD_OFFSET + 0, PAYLOAD_OFFSET);
	lv2_poke64(PAYLOAD_OPD_OFFSET + 8, TOC_OFFSET);

	real_opd_offset = SYSCALL_OPD_OFFSET(SYSCALL_RUN_PAYLOAD);
	lv2_poke64(SYSCALL_OPD_PTR_OFFSET(SYSCALL_RUN_PAYLOAD), PAYLOAD_OPD_OFFSET);

	return 0;
}

int remove_payload(void) {
	if (real_opd_offset != 0) {
		lv2_poke64(SYSCALL_OPD_PTR_OFFSET(SYSCALL_RUN_PAYLOAD), real_opd_offset);
		real_opd_offset = 0;
	}

	return 0;
}
