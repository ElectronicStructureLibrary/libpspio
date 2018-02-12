!! Copyright (C) 2015-2018 Damien Caliste <dcaliste@free.fr>
!!                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
integer function pspiof_projector_alloc(projector, np) result(ierr)
  type(pspiof_projector_t), intent(inout) :: projector
  integer,                  intent(in)    :: np

  ierr = pspio_projector_alloc(projector%ptr, np)

end function pspiof_projector_alloc

! init
integer function pspiof_projector_init(projector, qn, energy, mesh, proj) result(ierr)
  type(pspiof_projector_t), intent(inout) :: projector
  type(pspiof_qn_t),        intent(in)    :: qn
  real(8),                  intent(in)    :: energy
  type(pspiof_mesh_t),      intent(in)    :: mesh
  real(8),                  intent(in)    :: proj(*)

  ierr = pspio_projector_init(projector%ptr, qn%ptr, energy, mesh%ptr, proj)

end function pspiof_projector_init

! copy
integer function pspiof_projector_copy(src, dst) result(ierr)
  type(pspiof_projector_t), intent(in)    :: src
  type(pspiof_projector_t), intent(inout) :: dst

  ierr = pspio_projector_copy(dst%ptr, src%ptr)

end function pspiof_projector_copy

! free
subroutine pspiof_projector_free(projector)
  type(pspiof_projector_t), intent(inout) :: projector

  call pspio_projector_free(projector%ptr)
  projector%ptr = C_NULL_PTR

end subroutine pspiof_projector_free

! associated
logical function pspiof_projector_associated(projector) result(is_associated)
  type(pspiof_projector_t), intent(in) :: projector

  is_associated = c_associated(projector%ptr)

end function pspiof_projector_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! energy
real(8) function pspiof_projector_get_energy(projector) result(energy)
  type(pspiof_projector_t), intent(in)  :: projector

  energy = pspio_projector_get_energy(projector%ptr)

end function pspiof_projector_get_energy

! qn
type(pspiof_qn_t) function pspiof_projector_get_qn(projector) result(qn)
  type(pspiof_projector_t), intent(in)  :: projector

  qn%ptr = pspio_projector_get_qn(projector%ptr)

end function pspiof_projector_get_qn


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_projector_cmp(projector1, projector2) result(cmp)
  type(pspiof_projector_t) :: projector1
  type(pspiof_projector_t) :: projector2

  cmp = pspio_projector_cmp(projector1%ptr, projector2%ptr)

end function pspiof_projector_cmp

! eval
real(8) function pspiof_projector_eval(projector, r) result(proj)
  type(pspiof_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  proj = pspio_projector_eval(projector%ptr, r)

end function pspiof_projector_eval

! eval_deriv
real(8) function pspiof_projector_eval_deriv(projector, r) result(projp)
  type(pspiof_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  projp = pspio_projector_eval_deriv(projector%ptr, r)

end function pspiof_projector_eval_deriv

! eval_deriv2
real(8) function pspiof_projector_eval_deriv2(projector, r) result(projpp)
  type(pspiof_projector_t), intent(in)  :: projector
  real(8),                  intent(in)  :: r

  projpp = pspio_projector_eval_deriv2(projector%ptr, r)

end function pspiof_projector_eval_deriv2
