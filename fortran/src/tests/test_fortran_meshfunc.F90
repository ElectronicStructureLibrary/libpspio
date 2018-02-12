!! Copyright (C) 2015-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
!!                         Yann Pouillon <devops@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! This Source Code Form is subject to the terms of the Mozilla Public License,
!! version 2.0. If a copy of the MPL was not distributed with this file, You
!! can obtain one at https://mozilla.org/MPL/2.0/.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
!! more details.

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

program test_fortran_meshfunc

  use fruit
  use fruit_meshfunc_basket

  implicit none

  call init_fruit()
  call init_fruit_xml()

  call fruit_basket()

  call fruit_summary()
  call fruit_summary_xml()
  call fruit_finalize()

end program test_fortran_meshfunc
