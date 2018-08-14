/*
* Copyright (c) 2011-2012 by ps3dev.net
* This file is released under the GPLv2.
*/

#ifndef _INDIV_H_
#define _INDIV_H_

#include <ppu-types.h>

/*! EID0 section 0 iv offset. */
#define INDIV_EID0_SEC_0_IV_OFFSET 0x10
/*! EID0 section 0 keygen key offset. */
#define INDIV_EID0_SEC_0_GENKEY_OFFSET 0x20

/*! EID0 section A iv offset. */
#define INDIV_EID0_SEC_A_IV_OFFSET 0x10
/*! EID0 section A keygen key offset. */
#define INDIV_EID0_SEC_A_GENKEY_OFFSET 0x20

/*! EID1 iv offset. */
#define INDIV_EID1_IV_OFFSET 0x10
/*! EID1 key offset. */
#define INDIV_EID1_KEY_OFFSET 0x20

/*! EID2 iv offset. */
#define INDIV_EID2_IV_OFFSET 0x10
/*! EID2 key offset. */
#define INDIV_EID2_KEY_OFFSET 0x20

/*! EID4 iv offset. */
#define INDIV_EID4_IV_OFFSET 0x10
/*! EID4 key offset. */
#define INDIV_EID4_KEY_OFFSET 0x20

/*!
* \brief Generate individuals.
* \param indiv Individuals dest.
* \param seed0 Seed chunk 0.
* \param seed1 Seed chunk 1.
* \param seed2 Seed chunk 2.
* \param seed3 Seed chunk 3.
*/
void indiv_gen(u8 *seed0, u8 *seed1, u8 *seed2, u8 *seed3, u8 *indiv);

#endif
