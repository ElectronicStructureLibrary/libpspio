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


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

! alloc
integer function fpspio_mesh_alloc(mesh, np) result(ierr)
  type(fpspio_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: np

  ierr = pspio_mesh_alloc(mesh%ptr, np)

end function fpspio_mesh_alloc
  
! init
integer function fpspio_mesh_init(mesh, type, a, b, r, rab) result(ierr)
  type(fpspio_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: type
  real(8),             intent(in)    :: a
  real(8),             intent(in)    :: b
  real(8),             intent(in)    :: r(*)
  real(8),             intent(in)   :: rab(*)

  ierr = pspio_mesh_init(mesh%ptr, type, a, b, r, rab)

end function fpspio_mesh_init

! init_from_points
subroutine fpspio_mesh_init_from_points(mesh, r, rab)
  type(fpspio_mesh_t), intent(inout)                   :: mesh
  real(8),             intent(in)                      :: r(*)
  real(8),             intent(in),    optional, target :: rab(*)

  type(c_ptr) c_rab
  real(8), pointer :: f_rab(:)

  if (present(rab)) then
    f_rab => rab(1:fpspio_mesh_get_np(mesh))
    call c_f_pointer(c_rab, f_rab, [fpspio_mesh_get_np(mesh)])
  else
    c_rab = C_NULL_PTR
  end if

  call pspio_mesh_init_from_points(mesh%ptr, r, c_rab)

end subroutine fpspio_mesh_init_from_points

! init_from_parameters
subroutine fpspio_mesh_init_from_parameters(mesh, type, a, b)
  type(fpspio_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: type
  real(8),             intent(in)    :: a
  real(8),             intent(in)    :: b

  call pspio_mesh_init_from_parameters(mesh%ptr, type, a, b)

end subroutine fpspio_mesh_init_from_parameters

! copy
integer function fpspio_mesh_copy(src, dst) result(ierr)
  type(fpspio_mesh_t), intent(in)    :: src
  type(fpspio_mesh_t), intent(inout) :: dst

  ierr = pspio_mesh_copy(src%ptr, dst%ptr)

end function fpspio_mesh_copy

! free
subroutine fpspio_mesh_free(mesh)
  type(fpspio_mesh_t), intent(inout) :: mesh

  call pspio_mesh_free(mesh%ptr)
  mesh%ptr = C_NULL_PTR

end subroutine fpspio_mesh_free

! associated
logical function fpspio_mesh_associated(mesh) result(is_associated)
  type(fpspio_mesh_t), intent(in) :: mesh

  is_associated = c_associated(mesh%ptr)

end function fpspio_mesh_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! np
integer function fpspio_mesh_get_np(mesh) result(np)
  type(fpspio_mesh_t), intent(in)  :: mesh

  np = pspio_mesh_get_np(mesh%ptr)

end function fpspio_mesh_get_np

! a
real(8) function fpspio_mesh_get_a(mesh) result(a)
  type(fpspio_mesh_t), intent(in)  :: mesh

  a = pspio_mesh_get_a(mesh%ptr)

end function fpspio_mesh_get_a

! b
real(8) function fpspio_mesh_get_b(mesh) result(b)
  type(fpspio_mesh_t), intent(in)  :: mesh

  b = pspio_mesh_get_b(mesh%ptr)

end function fpspio_mesh_get_b

! r
function fpspio_mesh_get_r(mesh) result(r)
  type(fpspio_mesh_t), intent(in)  :: mesh
  real(8), pointer :: r(:)

  call c_f_pointer(pspio_mesh_get_r(mesh%ptr), r, [fpspio_mesh_get_np(mesh)])

end function fpspio_mesh_get_r

! rab
function fpspio_mesh_get_rab(mesh) result(rab)
  type(fpspio_mesh_t), intent(in)  :: mesh
  real(8), pointer :: rab(:)

  call c_f_pointer(pspio_mesh_get_rab(mesh%ptr), rab, [fpspio_mesh_get_np(mesh)])

end function fpspio_mesh_get_rab


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function fpspio_mesh_cmp(mesh1, mesh2) result(cmp)
  type(fpspio_mesh_t) :: mesh1
  type(fpspio_mesh_t) :: mesh2

  cmp = pspio_mesh_cmp(mesh1%ptr, mesh2%ptr)

end function fpspio_mesh_cmp
