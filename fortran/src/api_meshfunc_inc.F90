!! Copyright (C) 2015-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
!!                         Yann Pouillon <notifications@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! Libpspio is free software: you can redistribute it and/or modify it under
!! the terms of the GNU Lesser General Public License as published by the Free
!! Software Foundation, version 3 of the License, or (at your option) any later
!! version.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
!! more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
!! the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
!! 02110-1301  USA.


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

! alloc
integer function fpspio_meshfunc_alloc(meshfunc, np) result(ierr)
  type(fpspio_meshfunc_t), intent(inout) :: meshfunc
  integer,                 intent(in)    :: np

  ierr = pspio_meshfunc_alloc(meshfunc%ptr, np)

end function fpspio_meshfunc_alloc

! init
integer function fpspio_meshfunc_init(meshfunc, mesh, f, fp, fpp) result(ierr)
  type(fpspio_meshfunc_t), intent(inout)                   :: meshfunc
  type(fpspio_mesh_t),     intent(in)                      :: mesh
  real(8),                 intent(in)                      :: f(*)
  real(8),                 intent(in),    optional, target :: fp(*)
  real(8),                 intent(in),    optional, target :: fpp(*)

  type(c_ptr) c_fp, c_fpp
  real(8), pointer :: f_fp(:), f_fpp(:)

  if (present(fp)) then
    f_fp => fp(1:fpspio_mesh_get_np(mesh))
    call c_f_pointer(c_fp, f_fp, [fpspio_mesh_get_np(mesh)])
  else
    c_fp = C_NULL_PTR
  end if
  if (present(fpp)) then
    f_fpp => fpp(1:fpspio_mesh_get_np(mesh))
    call c_f_pointer(c_fpp, f_fpp, [fpspio_mesh_get_np(mesh)])
  else
    c_fpp = C_NULL_PTR
  end if
  ierr = pspio_meshfunc_init(meshfunc%ptr, mesh%ptr, f, c_fp, c_fpp)

end function fpspio_meshfunc_init

! copy
integer function fpspio_meshfunc_copy(src, dst) result(ierr)
  type(fpspio_meshfunc_t), intent(in)    :: src
  type(fpspio_meshfunc_t), intent(inout) :: dst

  ierr = pspio_meshfunc_copy(src%ptr, dst%ptr)

end function fpspio_meshfunc_copy

! free
subroutine fpspio_meshfunc_free(meshfunc)
  type(fpspio_meshfunc_t), intent(inout) :: meshfunc

  call pspio_meshfunc_free(meshfunc%ptr)
  meshfunc%ptr = C_NULL_PTR

end subroutine fpspio_meshfunc_free

! associated
logical function fpspio_meshfunc_associated(meshfunc) result(is_associated)
  type(fpspio_meshfunc_t), intent(in) :: meshfunc

  is_associated = c_associated(meshfunc%ptr)

end function fpspio_meshfunc_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! function
function fpspio_meshfunc_get_function(meshfunc) result(func)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: func(:)

  call c_f_pointer(pspio_meshfunc_get_function(meshfunc%ptr), func, [fpspio_mesh_get_np(fpspio_meshfunc_get_mesh(meshfunc))])

end function fpspio_meshfunc_get_function

! deriv1
function fpspio_meshfunc_get_deriv1(meshfunc) result(deriv1)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: deriv1(:)

  call c_f_pointer(pspio_meshfunc_get_deriv1(meshfunc%ptr), deriv1, [fpspio_mesh_get_np(fpspio_meshfunc_get_mesh(meshfunc))])

end function fpspio_meshfunc_get_deriv1

! deriv1
function fpspio_meshfunc_get_deriv2(meshfunc) result(deriv2)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8), pointer :: deriv2(:)

  call c_f_pointer(pspio_meshfunc_get_deriv2(meshfunc%ptr), deriv2, [fpspio_mesh_get_np(fpspio_meshfunc_get_mesh(meshfunc))])

end function fpspio_meshfunc_get_deriv2

! interp_method
integer function fpspio_meshfunc_get_interp_method(meshfunc) result(interp_method)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc

  interp_method = pspio_meshfunc_get_interp_method(meshfunc%ptr)
  
end function fpspio_meshfunc_get_interp_method

! mesh
type(fpspio_mesh_t) function fpspio_meshfunc_get_mesh(meshfunc) result(mesh)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc

  mesh%ptr = pspio_meshfunc_get_mesh(meshfunc%ptr)

end function fpspio_meshfunc_get_mesh


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function fpspio_meshfunc_cmp(meshfunc1, meshfunc2) result(cmp)
  type(fpspio_meshfunc_t) :: meshfunc1
  type(fpspio_meshfunc_t) :: meshfunc2

  cmp = pspio_meshfunc_cmp(meshfunc1%ptr, meshfunc2%ptr)

end function fpspio_meshfunc_cmp

! eval
real(8) function fpspio_meshfunc_eval(meshfunc, r) result(f)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  f = pspio_meshfunc_eval(meshfunc%ptr, r)

end function fpspio_meshfunc_eval

! eval_deriv
real(8) function fpspio_meshfunc_eval_deriv(meshfunc, r) result(fp)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  fp = pspio_meshfunc_eval_deriv(meshfunc%ptr, r)

end function fpspio_meshfunc_eval_deriv

! eval_deriv2
real(8) function fpspio_meshfunc_eval_deriv2(meshfunc, r) result(fpp)
  type(fpspio_meshfunc_t), intent(in)  :: meshfunc
  real(8),                 intent(in)  :: r

  fpp = pspio_meshfunc_eval_deriv2(meshfunc%ptr, r)

end function fpspio_meshfunc_eval_deriv2

