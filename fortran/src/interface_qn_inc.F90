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
    type(c_ptr),    value :: qn
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
  integer(c_int) function pspio_qn_label(qn, label) bind(c)
    import
    type(c_ptr),            value :: qn
    character(kind=c_char)        :: label(*)
  end function pspio_qn_label

end interface
