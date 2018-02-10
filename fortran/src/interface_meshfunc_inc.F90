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
  integer(c_int) function pspio_meshfunc_alloc(meshfunc, np) bind(c)
    import
    type(c_ptr)           :: meshfunc
    integer(c_int), value :: np
  end function pspio_meshfunc_alloc

  ! init
  integer(c_int) function pspio_meshfunc_init(meshfunc, mesh, f, fp, fpp) bind(c)
    import
    type(c_ptr),    value :: meshfunc
    type(c_ptr),    value :: mesh
    real(c_double)        :: f(*)
    type(c_ptr),    value :: fp
    type(c_ptr),    value :: fpp
  end function pspio_meshfunc_init

  ! copy
  integer(c_int) function pspio_meshfunc_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_meshfunc_copy

  ! free
  subroutine pspio_meshfunc_free(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end subroutine pspio_meshfunc_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! function
  type(c_ptr) function pspio_meshfunc_get_function(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end function pspio_meshfunc_get_function

  ! deriv1
  type(c_ptr) function pspio_meshfunc_get_deriv1(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end function pspio_meshfunc_get_deriv1

  ! deriv2
  type(c_ptr) function pspio_meshfunc_get_deriv2(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end function pspio_meshfunc_get_deriv2

  ! interp_method
  integer(c_int) function pspio_meshfunc_get_interp_method(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end function pspio_meshfunc_get_interp_method

  ! mesh
  type(c_ptr) function pspio_meshfunc_get_mesh(meshfunc) bind(c)
    import
    type(c_ptr), value :: meshfunc
  end function pspio_meshfunc_get_mesh


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_meshfunc_cmp(meshfunc1, meshfunc2) bind(c)
    import
    type(c_ptr), value :: meshfunc1
    type(c_ptr), value :: meshfunc2
  end function pspio_meshfunc_cmp

  ! eval
  real(c_double) function pspio_meshfunc_eval(meshfunc, r) bind(c)
    import
    type(c_ptr),    value :: meshfunc
    real(c_double), value :: r
  end function pspio_meshfunc_eval

  ! eval_deriv
  real(c_double) function pspio_meshfunc_eval_deriv(meshfunc, r) bind(c)
    import
    type(c_ptr),    value :: meshfunc
    real(c_double), value :: r
  end function pspio_meshfunc_eval_deriv

  ! eval_deriv2
  real(c_double) function pspio_meshfunc_eval_deriv2(meshfunc, r) bind(c)
    import
    type(c_ptr),    value :: meshfunc
    real(c_double), value :: r
  end function pspio_meshfunc_eval_deriv2

end interface
