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
integer function fpspio_projector_alloc(projector, np) result(ierr)
  type(fpspio_projector_t), intent(inout) :: projector
  integer,                  intent(in)    :: np

  ierr = pspio_projector_alloc(projector%ptr, np)

end function fpspio_projector_alloc

! init
integer function fpspio_projector_init(projector, qn, energy, mesh, proj) result(ierr)
  type(fpspio_projector_t), intent(inout) :: projector
  type(fpspio_qn_t),        intent(in)    :: qn
  real(8),                  intent(in)    :: energy
  type(fpspio_mesh_t),      intent(in)    :: mesh
  real(8),                  intent(in)    :: proj(*)

  ierr = pspio_projector_init(projector%ptr, qn%ptr, energy, mesh%ptr, proj)

end function fpspio_projector_init

! copy
integer function fpspio_projector_copy(src, dst) result(ierr)
  type(fpspio_projector_t), intent(in)    :: src
  type(fpspio_projector_t), intent(inout) :: dst

  ierr = pspio_projector_copy(src%ptr, dst%ptr)

end function fpspio_projector_copy

! free
subroutine fpspio_projector_free(projector)
  type(fpspio_projector_t), intent(inout) :: projector

  call pspio_projector_free(projector%ptr)
  projector%ptr = C_NULL_PTR

end subroutine fpspio_projector_free

! associated
logical function fpspio_projector_associated(projector) result(is_associated)
  type(fpspio_projector_t), intent(in) :: projector

  is_associated = c_associated(projector%ptr)

end function fpspio_projector_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! energy
real(8) function fpspio_projector_get_energy(projector) result(energy)
  type(fpspio_projector_t), intent(in)  :: projector

  energy = pspio_projector_get_energy(projector%ptr)

end function fpspio_projector_get_energy

! qn
type(fpspio_qn_t) function fpspio_projector_get_qn(projector) result(qn)
  type(fpspio_projector_t), intent(in)  :: projector

  qn%ptr = pspio_projector_get_qn(projector%ptr)

end function fpspio_projector_get_qn


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function fpspio_projector_cmp(projector1, projector2) result(cmp)
  type(fpspio_projector_t) :: projector1
  type(fpspio_projector_t) :: projector2

  cmp = pspio_projector_cmp(projector1%ptr, projector2%ptr)

end function fpspio_projector_cmp

! eval
real(8) function fpspio_projector_eval(projector, r) result(proj)
  type(fpspio_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  proj = pspio_projector_eval(projector%ptr, r)

end function fpspio_projector_eval

! eval_deriv
real(8) function fpspio_projector_eval_deriv(projector, r) result(projp)
  type(fpspio_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  projp = pspio_projector_eval_deriv(projector%ptr, r)

end function fpspio_projector_eval_deriv

! eval_deriv2
real(8) function fpspio_projector_eval_deriv2(projector, r) result(projpp)
  type(fpspio_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  projpp = pspio_projector_eval_deriv2(projector%ptr, r)

end function fpspio_projector_eval_deriv2
