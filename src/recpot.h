/* Copyright (C) 2018-2019 Damien Caliste <damien.caliste@cea.fr>
 *
 * This file is part of Libpspio. It is a C parser for CASTEP / ONETEP
 * recpot norm conserving pseudopotential files.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

#ifndef RECPOT_H
#define RECPOT_H

#include "pspio_pspdata.h"

int pspio_recpot_read(FILE *fp, pspio_pspdata_t *pspdata);
int pspio_recpot_write(FILE *fp, const pspio_pspdata_t *pspdata);

#endif
