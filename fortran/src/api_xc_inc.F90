!! Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

! alloc
integer function pspiof_xc_alloc(xc) result(ierr)
  type(pspiof_xc_t), intent(inout) :: xc

  ierr = pspio_xc_alloc(xc%ptr)

end function pspiof_xc_alloc

! copy
integer function pspiof_xc_copy(src, dst) result(ierr)
  type(pspiof_xc_t), intent(in)    :: src
  type(pspiof_xc_t), intent(inout) :: dst

  ierr = pspio_xc_copy(src%ptr, dst%ptr)

end function pspiof_xc_copy

! free
subroutine pspiof_xc_free(xc)
  type(pspiof_xc_t), intent(inout) :: xc

  call pspio_xc_free(xc%ptr)
  xc%ptr = C_NULL_PTR

end subroutine pspiof_xc_free

! associated
logical function pspiof_xc_associated(xc) result(is_associated)
  type(pspiof_xc_t), intent(in) :: xc

  is_associated = c_associated(xc%ptr)

end function pspiof_xc_associated


!*********************************************************************!
! Setters                                                             !
!*********************************************************************!

! exchange
integer function pspiof_xc_set_exchange(xc, exchange) result(ierr)
  type(pspiof_xc_t), intent(inout) :: xc
  integer,           intent(in)    :: exchange

  ierr = pspio_xc_set_exchange(xc%ptr, exchange)

end function pspiof_xc_set_exchange

! correlation
integer function pspiof_xc_set_correlation(xc, correlation) result(ierr)
  type(pspiof_xc_t), intent(inout) :: xc
  integer,           intent(in)    :: correlation

  ierr = pspio_xc_set_correlation(xc%ptr, correlation)

end function pspiof_xc_set_correlation

! nlcc_scheme
integer function pspiof_xc_set_nlcc_scheme(xc, nlcc_scheme) result(ierr)
  type(pspiof_xc_t), intent(inout) :: xc
  integer,           intent(in)    :: nlcc_scheme

  ierr = pspio_xc_set_nlcc_scheme(xc%ptr, nlcc_scheme)

end function pspiof_xc_set_nlcc_scheme

! nlcc_density
integer function pspiof_xc_set_nlcc_density(xc, mesh, cd, cdp, cdpp) result(ierr)
  type(pspiof_xc_t),   intent(inout) :: xc
  type(pspiof_mesh_t), intent(in)    :: mesh
  real(8),             intent(in)    :: cd(*)
  real(8),             intent(in),    optional, target :: cdp(*)
  real(8),             intent(in),    optional, target :: cdpp(*)

  type(c_ptr) c_cdp, c_cdpp
  real(8), pointer :: f_cdp(:), f_cdpp(:)

  if (present(cdp)) then
    f_cdp => cdp(1:pspiof_mesh_get_np(mesh))
    call c_f_pointer(c_cdp, f_cdp, [pspiof_mesh_get_np(mesh)])
  else
    c_cdp = C_NULL_PTR
  end if
  if (present(cdpp)) then
    f_cdpp => cdpp(1:pspiof_mesh_get_np(mesh))
    call c_f_pointer(c_cdpp, f_cdpp, [pspiof_mesh_get_np(mesh)])
  else
    c_cdpp = C_NULL_PTR
  end if

  ierr = pspio_xc_set_nlcc_density(xc%ptr, mesh%ptr, cd, c_cdp, c_cdpp)

end function pspiof_xc_set_nlcc_density


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! exchange
integer function pspiof_xc_get_exchange(xc) result(exchange)
  type(pspiof_xc_t), intent(in)  :: xc

  exchange = pspio_xc_get_exchange(xc%ptr)

end function pspiof_xc_get_exchange

! correlation
integer function pspiof_xc_get_correlation(xc) result(correlation)
  type(pspiof_xc_t), intent(in)  :: xc

  correlation = pspio_xc_get_correlation(xc%ptr)

end function pspiof_xc_get_correlation

! nlcc_scheme
integer function pspiof_xc_get_nlcc_scheme(xc) result(nlcc_scheme)
  type(pspiof_xc_t), intent(in)  :: xc

  nlcc_scheme = pspio_xc_get_nlcc_scheme(xc%ptr)

end function pspiof_xc_get_nlcc_scheme


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_xc_cmp(xc1, xc2) result(cmp)
  type(pspiof_xc_t) :: xc1
  type(pspiof_xc_t) :: xc2

  cmp = pspio_xc_cmp(xc1%ptr, xc2%ptr)

end function pspiof_xc_cmp

! cd_eval
real(8) function pspiof_xc_nlcc_density_eval(xc, r) result(cd)
  type(pspiof_xc_t), intent(in)  :: xc
  real(8),     intent(in)  :: r

  cd = pspio_xc_nlcc_density_eval(xc%ptr, r)

end function pspiof_xc_nlcc_density_eval

! cd_eval_deriv
real(8) function pspiof_xc_nlcc_density_eval_deriv(xc, r) result(cdp)
  type(pspiof_xc_t), intent(in)  :: xc
  real(8),     intent(in)  :: r

  cdp = pspio_xc_nlcc_density_eval_deriv(xc%ptr, r)

end function pspiof_xc_nlcc_density_eval_deriv

! cd_eval_deriv2
real(8) function pspiof_xc_nlcc_density_eval_deriv2(xc, r) result(cdpp)
  type(pspiof_xc_t), intent(in)  :: xc
  real(8),     intent(in)  :: r

  cdpp = pspio_xc_nlcc_density_eval_deriv2(xc%ptr, r)

end function pspiof_xc_nlcc_density_eval_deriv2

! has_nlcc
logical function pspiof_xc_has_nlcc(xc) result(has_nlcc)
  type(pspiof_xc_t), intent(in)  :: xc
    
  has_nlcc = pspio_xc_has_nlcc(xc%ptr) /= 0

end function pspiof_xc_has_nlcc
