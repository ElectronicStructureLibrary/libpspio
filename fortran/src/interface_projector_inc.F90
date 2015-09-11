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
