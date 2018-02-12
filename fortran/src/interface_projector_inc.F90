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
  integer(c_int) function pspio_projector_alloc(projector, np) bind(c)
    import
    type(c_ptr)           :: projector
    integer(c_int), value :: np
  end function pspio_projector_alloc

  ! init
  integer(c_int) function pspio_projector_init(projector, qn, energy, mesh, proj) bind(c)
    import
    type(c_ptr),      value :: projector
    type(c_ptr),      value :: qn
    real(c_double),   value :: energy
    type(c_ptr),      value :: mesh
    real(c_double)          :: proj(*)
  end function pspio_projector_init

  ! copy
  integer(c_int) function pspio_projector_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_projector_copy

  ! free
  subroutine pspio_projector_free(projector) bind(c)
    import
    type(c_ptr), value :: projector
  end subroutine pspio_projector_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! energy
  real(c_double) function pspio_projector_get_energy(projector) bind(c)
    import
    type(c_ptr), value :: projector
  end function pspio_projector_get_energy

  ! qn
  type(c_ptr) function pspio_projector_get_qn(projector) bind(c)
    import
    type(c_ptr), value :: projector      
  end function pspio_projector_get_qn


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_projector_cmp(projector1, projector2) bind(c)
    import
    type(c_ptr), value :: projector1
    type(c_ptr), value :: projector2
  end function pspio_projector_cmp

  ! eval
  real(c_double) function pspio_projector_eval(projector, r) bind(c)
    import
    type(c_ptr),    value :: projector
    real(c_double), value :: r
  end function pspio_projector_eval

  ! eval_deriv
  real(c_double) function pspio_projector_eval_deriv(projector, r) bind(c)
    import
    type(c_ptr),    value :: projector
    real(c_double), value :: r
  end function pspio_projector_eval_deriv

  ! eval_deriv2
  real(c_double) function pspio_projector_eval_deriv2(projector, r) bind(c)
    import
    type(c_ptr),    value :: projector
    real(c_double), value :: r
  end function pspio_projector_eval_deriv2

end interface
