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
  integer(c_int) function pspio_state_alloc(state, np) bind(c)
    import
    type(c_ptr)           :: state
    integer(c_int), value :: np
  end function pspio_state_alloc

  ! init
  integer(c_int) function pspio_state_init(state, eigenval, qn, occ, rc, mesh, wf, label) bind(c)
    import
    type(c_ptr),           value :: state
    real(c_double),        value :: eigenval
    type(c_ptr),           value :: qn
    real(c_double),        value :: occ
    real(c_double),        value :: rc
    type(c_ptr),           value :: mesh
    real(c_double)               :: wf(*)
    character(kind=c_char)       :: label(*)
  end function pspio_state_init

  ! copy
  integer(c_int) function pspio_state_copy(dst, src) bind(c)
    import
    type(c_ptr) :: dst
    type(c_ptr), value :: src
  end function pspio_state_copy

  ! free
  subroutine pspio_state_free(state) bind(c)
    import
    type(c_ptr), value :: state
  end subroutine pspio_state_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! label
  type(c_ptr) function pspio_state_get_label(state) bind(c)
    import
    type(c_ptr), value :: state
  end function pspio_state_get_label

  ! qn
  type(c_ptr) function pspio_state_get_qn(state) bind(c)
    import
    type(c_ptr), value :: state
  end function pspio_state_get_qn

  ! occ
  real(c_double) function pspio_state_get_occ(state) bind(c)
    import
    type(c_ptr), value :: state      
  end function pspio_state_get_occ

  ! ev
  real(c_double) function pspio_state_get_ev(state) bind(c)
    import
    type(c_ptr), value :: state      
  end function pspio_state_get_ev

  ! rc
  real(c_double) function  pspio_state_get_rc(state) bind(c)
    import
    type(c_ptr), value :: state
  end function pspio_state_get_rc

  ! wf
  type(c_ptr) function pspio_state_get_wf(state) bind(c)
    import
    type(c_ptr), value :: state
  end function pspio_state_get_wf


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_state_cmp(state1, state2) bind(c)
    import
    type(c_ptr), value :: state1
    type(c_ptr), value :: state2
  end function pspio_state_cmp

  ! wf_eval
  real(c_double) function pspio_state_wf_eval(state, r) bind(c)
    import
    type(c_ptr),    value :: state
    real(c_double), value :: r
  end function pspio_state_wf_eval

  ! wf_eval_deriv
  real(c_double) function pspio_state_wf_eval_deriv(state, r) bind(c)
    import
    type(c_ptr),    value :: state
    real(c_double), value :: r
  end function pspio_state_wf_eval_deriv

  ! wf_eval_deriv2
  real(c_double) function pspio_state_wf_eval_deriv2(state, r) bind(c)
    import
    type(c_ptr),    value :: state
    real(c_double), value :: r
  end function pspio_state_wf_eval_deriv2

end interface
