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
integer function pspiof_potential_alloc(potential, np) result(ierr)
  type(pspiof_potential_t), intent(inout) :: potential
  integer,                  intent(in)    :: np

  ierr = pspio_potential_alloc(potential%ptr, np)

end function pspiof_potential_alloc

! init
integer function pspiof_potential_init(potential, qn, mesh, v) result(ierr)
  type(pspiof_potential_t), intent(inout) :: potential
  type(pspiof_qn_t),        intent(in)    :: qn
  type(pspiof_mesh_t),      intent(in)    :: mesh
  real(8),                  intent(in)    :: v(*)

  ierr = pspio_potential_init(potential%ptr, qn%ptr, mesh%ptr, v)

end function pspiof_potential_init

! copy
integer function pspiof_potential_copy(src, dst) result(ierr)
  type(pspiof_potential_t), intent(in)    :: src
  type(pspiof_potential_t), intent(inout) :: dst

  ierr = pspio_potential_copy(src%ptr, dst%ptr)

end function pspiof_potential_copy

! free
subroutine pspiof_potential_free(potential)
  type(pspiof_potential_t), intent(inout) :: potential

  call pspio_potential_free(potential%ptr)
  potential%ptr = C_NULL_PTR

end subroutine pspiof_potential_free

! associated
logical function pspiof_potential_associated(potential) result(is_associated)
  type(pspiof_potential_t), intent(in) :: potential

  is_associated = c_associated(potential%ptr)

end function pspiof_potential_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! qn
type(pspiof_qn_t) function pspiof_potential_get_qn(potential) result(qn)
  type(pspiof_potential_t), intent(in)  :: potential

  qn%ptr = pspio_potential_get_qn(potential%ptr)
  
end function pspiof_potential_get_qn

!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_potential_cmp(potential1, potential2) result(cmp)
  type(pspiof_potential_t) :: potential1
  type(pspiof_potential_t) :: potential2

  cmp = pspio_potential_cmp(potential1%ptr, potential2%ptr)

end function pspiof_potential_cmp

! eval
real(8) function pspiof_potential_eval(potential, r) result(v)
  type(pspiof_potential_t), intent(in)  :: potential
  real(8),                 intent(in)  :: r

  v = pspio_potential_eval(potential%ptr, r)

end function pspiof_potential_eval

! eval_deriv
real(8) function pspiof_potential_eval_deriv(potential, r) result(vp)
  type(pspiof_potential_t), intent(in)  :: potential
  real(8),                 intent(in)  :: r

  vp = pspio_potential_eval_deriv(potential%ptr, r)

end function pspiof_potential_eval_deriv

! eval_deriv2
real(8) function pspiof_potential_eval_deriv2(potential, r) result(vpp)
  type(pspiof_potential_t), intent(in)  :: potential
  real(8),                 intent(in)  :: r

  vpp = pspio_potential_eval_deriv2(potential%ptr, r)

end function pspiof_potential_eval_deriv2
