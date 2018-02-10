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
integer function pspiof_meshfunc_alloc(meshfunc, np) result(ierr)
  type(pspiof_meshfunc_t), intent(inout) :: meshfunc
  integer,                 intent(in)    :: np

  ierr = pspio_meshfunc_alloc(meshfunc%ptr, np)

end function pspiof_meshfunc_alloc

! init
integer function pspiof_meshfunc_init(meshfunc, mesh, f, fp, fpp) result(ierr)
  type(pspiof_meshfunc_t), intent(inout)                   :: meshfunc
  type(pspiof_mesh_t),     intent(in)                      :: mesh
  real(8),                 intent(in)                      :: f(*)
  real(8),                 intent(in),    optional, target :: fp(*)
  real(8),                 intent(in),    optional, target :: fpp(*)

  type(c_ptr) c_fp, c_fpp
  real(8), pointer :: f_fp(:), f_fpp(:)

  if (present(fp)) then
    f_fp => fp(1:pspiof_mesh_get_np(mesh))
    call c_f_pointer(c_fp, f_fp, [pspiof_mesh_get_np(mesh)])
  else
    c_fp = C_NULL_PTR
  end if
  if (present(fpp)) then
    f_fpp => fpp(1:pspiof_mesh_get_np(mesh))
    call c_f_pointer(c_fpp, f_fpp, [pspiof_mesh_get_np(mesh)])
  else
    c_fpp = C_NULL_PTR
  end if
  ierr = pspio_meshfunc_init(meshfunc%ptr, mesh%ptr, f, c_fp, c_fpp)

end function pspiof_meshfunc_init

! copy
integer function pspiof_meshfunc_copy(src, dst) result(ierr)
  type(pspiof_meshfunc_t), intent(in)    :: src
  type(pspiof_meshfunc_t), intent(inout) :: dst

  ierr = pspio_meshfunc_copy(src%ptr, dst%ptr)

end function pspiof_meshfunc_copy

! free
subroutine pspiof_meshfunc_free(meshfunc)
  type(pspiof_meshfunc_t), intent(inout) :: meshfunc

  call pspio_meshfunc_free(meshfunc%ptr)
  meshfunc%ptr = C_NULL_PTR

end subroutine pspiof_meshfunc_free

! associated
logical function pspiof_meshfunc_associated(meshfunc) result(is_associated)
  type(pspiof_meshfunc_t), intent(in) :: meshfunc

  is_associated = c_associated(meshfunc%ptr)

end function pspiof_meshfunc_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! function
function pspiof_meshfunc_get_function(meshfunc) result(func)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: func(:)

  call c_f_pointer(pspio_meshfunc_get_function(meshfunc%ptr), func, [pspiof_mesh_get_np(pspiof_meshfunc_get_mesh(meshfunc))])

end function pspiof_meshfunc_get_function

! deriv1
function pspiof_meshfunc_get_deriv1(meshfunc) result(deriv1)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: deriv1(:)

  call c_f_pointer(pspio_meshfunc_get_deriv1(meshfunc%ptr), deriv1, [pspiof_mesh_get_np(pspiof_meshfunc_get_mesh(meshfunc))])

end function pspiof_meshfunc_get_deriv1

! deriv1
function pspiof_meshfunc_get_deriv2(meshfunc) result(deriv2)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: deriv2(:)

  call c_f_pointer(pspio_meshfunc_get_deriv2(meshfunc%ptr), deriv2, [pspiof_mesh_get_np(pspiof_meshfunc_get_mesh(meshfunc))])

end function pspiof_meshfunc_get_deriv2

! interp_method
integer function pspiof_meshfunc_get_interp_method(meshfunc) result(interp_method)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc

  interp_method = pspio_meshfunc_get_interp_method(meshfunc%ptr)
  
end function pspiof_meshfunc_get_interp_method

! mesh
type(pspiof_mesh_t) function pspiof_meshfunc_get_mesh(meshfunc) result(mesh)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc

  mesh%ptr = pspio_meshfunc_get_mesh(meshfunc%ptr)

end function pspiof_meshfunc_get_mesh


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_meshfunc_cmp(meshfunc1, meshfunc2) result(cmp)
  type(pspiof_meshfunc_t) :: meshfunc1
  type(pspiof_meshfunc_t) :: meshfunc2

  cmp = pspio_meshfunc_cmp(meshfunc1%ptr, meshfunc2%ptr)

end function pspiof_meshfunc_cmp

! eval
real(8) function pspiof_meshfunc_eval(meshfunc, r) result(f)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  f = pspio_meshfunc_eval(meshfunc%ptr, r)

end function pspiof_meshfunc_eval

! eval_deriv
real(8) function pspiof_meshfunc_eval_deriv(meshfunc, r) result(fp)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  fp = pspio_meshfunc_eval_deriv(meshfunc%ptr, r)

end function pspiof_meshfunc_eval_deriv

! eval_deriv2
real(8) function pspiof_meshfunc_eval_deriv2(meshfunc, r) result(fpp)
  type(pspiof_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  fpp = pspio_meshfunc_eval_deriv2(meshfunc%ptr, r)

end function pspiof_meshfunc_eval_deriv2

