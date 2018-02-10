!! Copyright (C) 2016-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
  ! Global routines                                                     !
  !*********************************************************************!

  ! alloc
  integer(c_int) function pspio_pspinfo_alloc(pspinfo) bind(c)
    import
    type(c_ptr) :: pspinfo
  end function pspio_pspinfo_alloc

  ! copy
  integer(c_int) function pspio_pspinfo_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_pspinfo_copy

  ! free
  subroutine pspio_pspinfo_free(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end subroutine pspio_pspinfo_free


  !*********************************************************************!
  ! Setters                                                             !
  !*********************************************************************!

  ! author
  integer(c_int) function pspio_pspinfo_set_author(pspinfo, author) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: author(*)
  end function pspio_pspinfo_set_author

  ! code_name
  integer(c_int) function pspio_pspinfo_set_code_name(pspinfo, code_name) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: code_name(*)
  end function pspio_pspinfo_set_code_name
  ! code_name

  integer(c_int) function pspio_pspinfo_set_code_version(pspinfo, code_version) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: code_version(*)
  end function pspio_pspinfo_set_code_version

  ! generation_day
  integer(c_int) function pspio_pspinfo_set_generation_day(pspinfo, day) bind(c)
    import
    type(c_ptr),    value :: pspinfo
    integer(c_int), value :: day
  end function pspio_pspinfo_set_generation_day

  ! generation_month
  integer(c_int) function pspio_pspinfo_set_generation_month(pspinfo, month) bind(c)
    import
    type(c_ptr),    value :: pspinfo
    integer(c_int), value :: month
  end function pspio_pspinfo_set_generation_month

  ! generation_year
    integer(c_int) function pspio_pspinfo_set_generation_year(pspinfo, year) bind(c)
      import
      type(c_ptr),    value :: pspinfo
      integer(c_int), value :: year
    end function pspio_pspinfo_set_generation_year

  ! description
  integer(c_int) function pspio_pspinfo_set_description(pspinfo, description) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: description(*)
  end function pspio_pspinfo_set_description

  
  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! author
  character(kind=c_char) function pspio_pspinfo_get_author(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_author

  ! code_name
  character(kind=c_char) function pspio_pspinfo_get_code_name(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_code_name

  ! code_version
  character(kind=c_char) function pspio_pspinfo_get_code_version(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_code_version

  ! generation_day
  integer(c_int) function pspio_pspinfo_get_generation_day(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_generation_day

  ! generation_month
  integer(c_int) function pspio_pspinfo_get_generation_month(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_generation_month

  ! generation_year
  integer(c_int) function pspio_pspinfo_get_generation_year(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_generation_year

  ! description
  character(kind=c_char) function pspio_pspinfo_get_description(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_description


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_pspinfo_cmp(pspinfo1, pspinfo2) bind(c)
    import
    type(c_ptr), value :: pspinfo1
    type(c_ptr), value :: pspinfo2
  end function pspio_pspinfo_cmp

end interface
