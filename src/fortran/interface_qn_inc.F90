!! Copyright (C) 2012-2015 M. Oliveira
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
  integer(c_int) function pspio_qn_alloc(qn) bind(c)
    import
    type(c_ptr) :: qn
  end function pspio_qn_alloc

  ! init
  integer(c_int) function pspio_qn_init(qn, n, l, j) bind(c)
    import
    type(c_ptr)           :: qn
    integer(c_int), value :: n
    integer(c_int), value :: l
    real(c_double), value :: j
  end function pspio_qn_init
  
  ! copy
  integer(c_int) function pspio_qn_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_qn_copy

  ! free
  subroutine pspio_qn_free(qn) bind(c)
    import
    type(c_ptr), value :: qn
  end subroutine pspio_qn_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! n
  integer(c_int) function pspio_qn_get_n(qn) bind(c)
    import
    type(c_ptr), value :: qn
  end function pspio_qn_get_n

  ! l
  integer(c_int) function pspio_qn_get_l(qn) bind(c)
    import
    type(c_ptr), value :: qn
  end function pspio_qn_get_l

  ! j
  real(c_double) function pspio_qn_get_j(qn) bind(c)
    import
    type(c_ptr), value :: qn
  end function pspio_qn_get_j


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_qn_cmp(qn1, qn2) bind(c)
    import
    type(c_ptr), value :: qn1
    type(c_ptr), value :: qn2
  end function pspio_qn_cmp

  ! label
  subroutine pspio_qn_label(qn, label) bind(c)
    import
    type(c_ptr),            value :: qn
    character(kind=c_char)        :: label(*)
  end subroutine pspio_qn_label

end interface
