!! Copyright (C) 2016 M. Oliveira
!!
!! This program is free software; you can redistribute it and/or modify
!! it under the terms of the GNU Lesser General Public License as published by
!! the Free Software Foundation; either version 3 of the License, or 
!! (at your option) any later version.
!!
!! This program is distributed in the hope that it will be useful,
!! but WITHOUT ANY WARRANTY; without even the implied warranty of
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!! GNU Lesser General Public License for more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with this program; if not, write to the Free Software
!! Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
!!

interface

  !*********************************************************************!
  ! Global routines                                                     !
  !*********************************************************************!

  ! alloc
  integer(c_int) function pspio_pspinfo_alloc(pspinfo) bind(c)
    import
    type(c_ptr) :: pspinfo
  end function pspio_pspinfo_alloc

  ! init
  integer(c_int) function pspio_pspinfo_init(pspinfo, author, code, date, description) bind(c)
    import
    type(c_ptr),           value :: pspinfo
    character(kind=c_char)       :: author(*)
    character(kind=c_char)       :: code(*)
    character(kind=c_char)       :: date(*)
    character(kind=c_char)       :: description(*)
  end function pspio_pspinfo_init
  
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

  ! code
  integer(c_int) function pspio_pspinfo_set_code(pspinfo, code) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: code(*)
  end function pspio_pspinfo_set_code

  ! date
  integer(c_int) function pspio_pspinfo_set_date(pspinfo, date) bind(c)
    import
    type(c_ptr),            value :: pspinfo
    character(kind=c_char)        :: date(*)
  end function pspio_pspinfo_set_date

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

  ! code
  character(kind=c_char) function pspio_pspinfo_get_code(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_code

  ! date
  character(kind=c_char) function pspio_pspinfo_get_date(pspinfo) bind(c)
    import
    type(c_ptr), value :: pspinfo
  end function pspio_pspinfo_get_date

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
