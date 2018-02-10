!! Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

module fruit_info_test

  use pspiof_m
  use fruit

  implicit none

contains

  subroutine test_info_version()

    implicit none

    character(len=*), parameter :: ref_version = PACKAGE_VERSION
    character(len=len_trim(ref_version)) :: chk_version
    integer :: major, minor, micro

    call pspiof_info_version(major, minor, micro)
    write(chk_version, '(I0,".",I0,".",I0)') major, minor, micro

    call assert_equals(ref_version, chk_version, "Package version")

  end subroutine test_info_version

  subroutine test_info_string()

    implicit none

    character(len=*), parameter :: ref_string = PACKAGE_STRING
    character(len=len_trim(ref_string)) :: chk_string

    call pspiof_info_string(chk_string)

    call assert_equals(ref_string, chk_string, "Package string")

  end subroutine test_info_string

end module fruit_info_test
