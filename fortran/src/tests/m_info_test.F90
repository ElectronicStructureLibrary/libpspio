!! Copyright (C) 2015-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
!!                         Yann Pouillon <notifications@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! Libpspio is free software: you can redistribute it and/or modify it under
!! the terms of the GNU Lesser General Public License as published by the Free
!! Software Foundation, version 3 of the License, or (at your option) any later
!! version.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
!! more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
!! the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
!! 02110-1301  USA.

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

module m_info_test

  use fpspio_m
  use fruit

  implicit none

contains

  subroutine test_info_version()

    implicit none

    character(len=*), parameter :: ref_version = PACKAGE_VERSION
    character(len=len_trim(ref_version)) :: chk_version
    integer :: major, minor, micro

    call fpspio_info_version(major, minor, micro)
    write(chk_version, '(I0,".",I0,".",I0)') major, minor, micro

    call assert_equals(ref_version, chk_version, "Package version")

  end subroutine test_info_version

  subroutine test_info_string()

    implicit none

    character(len=*), parameter :: ref_string = PACKAGE_STRING
    character(len=len_trim(ref_string)) :: chk_string

    call fpspio_info_string(chk_string)

    call assert_equals(ref_string, chk_string, "Package string")

  end subroutine test_info_string

end module m_info_test
