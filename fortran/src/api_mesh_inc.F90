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
integer function pspiof_mesh_alloc(mesh, np) result(ierr)
  type(pspiof_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: np

  ierr = pspio_mesh_alloc(mesh%ptr, np)

end function pspiof_mesh_alloc
  
! init
integer function pspiof_mesh_init(mesh, type, a, b, r, rab) result(ierr)
  type(pspiof_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: type
  real(8),             intent(in)    :: a
  real(8),             intent(in)    :: b
  real(8),             intent(in)    :: r(*)
  real(8),             intent(in)   :: rab(*)

  ierr = pspio_mesh_init(mesh%ptr, type, a, b, r, rab)

end function pspiof_mesh_init

! init_from_points
subroutine pspiof_mesh_init_from_points(mesh, r, rab)
  type(pspiof_mesh_t), intent(inout)                   :: mesh
  real(8),             intent(in)                      :: r(*)
  real(8),             intent(in),    optional, target :: rab(*)

  type(c_ptr) c_rab
  real(8), pointer :: f_rab(:)

  if (present(rab)) then
    f_rab => rab(1:pspiof_mesh_get_np(mesh))
    call c_f_pointer(c_rab, f_rab, [pspiof_mesh_get_np(mesh)])
  else
    c_rab = C_NULL_PTR
  end if

  call pspio_mesh_init_from_points(mesh%ptr, r, c_rab)

end subroutine pspiof_mesh_init_from_points

! init_from_parameters
subroutine pspiof_mesh_init_from_parameters(mesh, type, a, b)
  type(pspiof_mesh_t), intent(inout) :: mesh
  integer,             intent(in)    :: type
  real(8),             intent(in)    :: a
  real(8),             intent(in)    :: b

  call pspio_mesh_init_from_parameters(mesh%ptr, type, a, b)

end subroutine pspiof_mesh_init_from_parameters

! copy
integer function pspiof_mesh_copy(src, dst) result(ierr)
  type(pspiof_mesh_t), intent(in)    :: src
  type(pspiof_mesh_t), intent(inout) :: dst

  ierr = pspio_mesh_copy(src%ptr, dst%ptr)

end function pspiof_mesh_copy

! free
subroutine pspiof_mesh_free(mesh)
  type(pspiof_mesh_t), intent(inout) :: mesh

  call pspio_mesh_free(mesh%ptr)
  mesh%ptr = C_NULL_PTR

end subroutine pspiof_mesh_free

! associated
logical function pspiof_mesh_associated(mesh) result(is_associated)
  type(pspiof_mesh_t), intent(in) :: mesh

  is_associated = c_associated(mesh%ptr)

end function pspiof_mesh_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! np
integer function pspiof_mesh_get_np(mesh) result(np)
  type(pspiof_mesh_t), intent(in)  :: mesh

  np = pspio_mesh_get_np(mesh%ptr)

end function pspiof_mesh_get_np

! a
real(8) function pspiof_mesh_get_a(mesh) result(a)
  type(pspiof_mesh_t), intent(in)  :: mesh

  a = pspio_mesh_get_a(mesh%ptr)

end function pspiof_mesh_get_a

! b
real(8) function pspiof_mesh_get_b(mesh) result(b)
  type(pspiof_mesh_t), intent(in)  :: mesh

  b = pspio_mesh_get_b(mesh%ptr)

end function pspiof_mesh_get_b

! r
function pspiof_mesh_get_r(mesh) result(r)
  type(pspiof_mesh_t), intent(in)  :: mesh
  real(8), pointer :: r(:)

  call c_f_pointer(pspio_mesh_get_r(mesh%ptr), r, [pspiof_mesh_get_np(mesh)])

end function pspiof_mesh_get_r

! rab
function pspiof_mesh_get_rab(mesh) result(rab)
  type(pspiof_mesh_t), intent(in)  :: mesh
  real(8), pointer :: rab(:)

  call c_f_pointer(pspio_mesh_get_rab(mesh%ptr), rab, [pspiof_mesh_get_np(mesh)])

end function pspiof_mesh_get_rab


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_mesh_cmp(mesh1, mesh2) result(cmp)
  type(pspiof_mesh_t) :: mesh1
  type(pspiof_mesh_t) :: mesh2

  cmp = pspio_mesh_cmp(mesh1%ptr, mesh2%ptr)

end function pspiof_mesh_cmp
