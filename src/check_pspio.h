/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
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

#ifndef CHECK_PSPIO_H
#define CHECK_PSPIO_H

/**
 * @file check_pspio.h 
 * @brief pspsio check testsuites
 */

Suite *make_info_suite(void);
Suite *make_error_suite(void);
Suite *make_qn_suite(void);
Suite *make_mesh_suite(void);
Suite *make_meshfunc_suite(void);
Suite *make_potential_suite(void);
Suite *make_projector_suite(void);
Suite *make_state_suite(void);
Suite *make_xc_suite(void);
Suite *make_pspinfo_suite(void);
Suite *make_pspdata_suite(void);

#endif
