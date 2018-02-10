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
  integer(c_int) function pspio_potential_alloc(potential, np) bind(c)
    import
    type(c_ptr)           :: potential
    integer(c_int), value :: np
  end function pspio_potential_alloc

  ! init
  integer(c_int) function pspio_potential_init(potential, qn, mesh, v) bind(c)
    import
    type(c_ptr),      value :: potential
    type(c_ptr),      value :: qn
    type(c_ptr),      value :: mesh
    real(c_double)          :: v(*)
  end function pspio_potential_init

  ! copy
  integer(c_int) function pspio_potential_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_potential_copy

  ! free
  subroutine pspio_potential_free(potential) bind(c)
    import
    type(c_ptr), value :: potential
  end subroutine pspio_potential_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! qn
  type(c_ptr) function pspio_potential_get_qn(potential) bind(c)
    import
    type(c_ptr), value :: potential
  end function pspio_potential_get_qn


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_potential_cmp(potential1, potential2) bind(c)
    import
    type(c_ptr), value :: potential1
    type(c_ptr), value :: potential2
  end function pspio_potential_cmp

  ! eval
  real(c_double) function pspio_potential_eval(potential, r) bind(c)
    import
    type(c_ptr),    value :: potential
    real(c_double), value :: r
  end function pspio_potential_eval

  ! eval_deriv
  real(c_double) function pspio_potential_eval_deriv(potential, r) bind(c)
    import
    type(c_ptr),    value :: potential
    real(c_double), value :: r
  end function pspio_potential_eval_deriv

  ! eval_deriv2
  real(c_double) function pspio_potential_eval_deriv2(potential, r) bind(c)
    import
    type(c_ptr),    value :: potential
    real(c_double), value :: r
  end function pspio_potential_eval_deriv2

end interface
