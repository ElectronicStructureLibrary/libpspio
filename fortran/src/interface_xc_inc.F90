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
  integer(c_int) function pspio_xc_alloc(xc) bind(c)
    import
    type(c_ptr) :: xc
  end function pspio_xc_alloc

  ! copy
  integer(c_int) function pspio_xc_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_xc_copy

  ! free
  subroutine pspio_xc_free(xc) bind(c)
    import
    type(c_ptr), value :: xc
  end subroutine pspio_xc_free


  !*********************************************************************!
  ! Setters                                                             !
  !*********************************************************************!

  ! exchange
  integer(c_int) function pspio_xc_set_exchange(xc, exchange) bind(c)
    import
    type(c_ptr),    value :: xc
    integer(c_int), value :: exchange
  end function pspio_xc_set_exchange

  ! correlation
  integer(c_int) function pspio_xc_set_correlation(xc, correlation) bind(c)
    import
    type(c_ptr),    value :: xc
    integer(c_int), value :: correlation
  end function pspio_xc_set_correlation

  ! nlcc_scheme
  integer(c_int) function pspio_xc_set_nlcc_scheme(xc, nlcc_scheme) bind(c)
    import
    type(c_ptr),    value :: xc
    integer(c_int), value :: nlcc_scheme
  end function pspio_xc_set_nlcc_scheme

  ! nlcc_density
  integer(c_int) function pspio_xc_set_nlcc_density(xc, mesh, cd, cdp, cdpp) bind(c)
    import
    type(c_ptr),    value :: xc
    type(c_ptr),    value :: mesh
    real(c_double)        :: cd(*)
    type(c_ptr),    value :: cdp
    type(c_ptr),    value :: cdpp
  end function pspio_xc_set_nlcc_density


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! correlation
  integer(c_int) function pspio_xc_get_correlation(xc) bind(c)
    import
    type(c_ptr), value :: xc
  end function pspio_xc_get_correlation

  ! exchange
  integer(c_int) function pspio_xc_get_exchange(xc) bind(c)
    import
    type(c_ptr), value :: xc
  end function pspio_xc_get_exchange

  ! nlcc_scheme
  integer(c_int) function pspio_xc_get_nlcc_scheme(xc) bind(c)
    import
    type(c_ptr), value :: xc
  end function pspio_xc_get_nlcc_scheme


  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_xc_cmp(xc1, xc2) bind(c)
    import
    type(c_ptr), value :: xc1
    type(c_ptr), value :: xc2
  end function pspio_xc_cmp

  ! nlcc_density_eval
  real(c_double) function pspio_xc_nlcc_density_eval(xc, r) bind(c)
    import
    type(c_ptr),    value :: xc
    real(c_double), value :: r
  end function pspio_xc_nlcc_density_eval

  ! nlcc_density_eval_deriv
  real(c_double) function pspio_xc_nlcc_density_eval_deriv(xc, r) bind(c)
    import
    type(c_ptr) ,   value :: xc
    real(c_double), value :: r
  end function pspio_xc_nlcc_density_eval_deriv

  ! nlcc_density_eval_deriv2
  real(c_double) function pspio_xc_nlcc_density_eval_deriv2(xc, r) bind(c)
    import
    type(c_ptr),    value :: xc
    real(c_double), value :: r
  end function pspio_xc_nlcc_density_eval_deriv2

  ! has_nlcc
  integer(c_int) function pspio_xc_has_nlcc(xc) bind(c)
    import
    type(c_ptr), value :: xc
  end function pspio_xc_has_nlcc

end interface
