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

interface

  !*********************************************************************!
  ! Routines                                                            !
  !*********************************************************************!

  ! version
  subroutine pspiof_info_version(major, minor, micro) bind(c, name="pspio_info_version")
    import
    integer(c_int), intent(out) :: major, minor, micro
  end subroutine pspiof_info_version

  ! info
  subroutine pspio_info_string(info) bind(c)
    import
    character(kind=c_char) :: info(*)
  end subroutine pspio_info_string

end interface
