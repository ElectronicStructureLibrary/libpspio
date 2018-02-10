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
integer function pspiof_qn_alloc(qn) result(ierr)
  type(pspiof_qn_t), intent(inout) :: qn

  ierr = pspio_qn_alloc(qn%ptr)

end function pspiof_qn_alloc

! init
integer function pspiof_qn_init(qn, n, l, j) result(ierr)
  type(pspiof_qn_t), intent(inout) :: qn
  integer,           intent(in)    :: n
  integer,           intent(in)    :: l
  real(8),           intent(in)    :: j

  ierr = pspio_qn_init(qn%ptr, n, l, j)

end function pspiof_qn_init

! copy
integer function pspiof_qn_copy(src, dst) result(ierr)
  type(pspiof_qn_t), intent(in)    :: src
  type(pspiof_qn_t), intent(inout) :: dst

  ierr = pspio_qn_copy(src%ptr, dst%ptr)

end function pspiof_qn_copy

! free
subroutine pspiof_qn_free(qn)
  type(pspiof_qn_t), intent(inout) :: qn

  call pspio_qn_free(qn%ptr)
  qn%ptr = C_NULL_PTR

end subroutine pspiof_qn_free

! associated
logical function pspiof_qn_associated(qn) result(is_associated)
  type(pspiof_qn_t), intent(in) :: qn

  is_associated = c_associated(qn%ptr)

end function pspiof_qn_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! n
integer function pspiof_qn_get_n(qn) result(n)
  type(pspiof_qn_t), intent(in)  :: qn

  n = pspio_qn_get_n(qn%ptr)

end function pspiof_qn_get_n

! l
integer function pspiof_qn_get_l(qn) result(l)
  type(pspiof_qn_t), intent(in)  :: qn

  l = pspio_qn_get_l(qn%ptr)

end function pspiof_qn_get_l

! j
real(8) function pspiof_qn_get_j(qn) result(j)
  type(pspiof_qn_t), intent(in)  :: qn

  j = pspio_qn_get_j(qn%ptr)

end function pspiof_qn_get_j


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_qn_cmp(qn1, qn2) result(cmp)
  type(pspiof_qn_t) :: qn1
  type(pspiof_qn_t) :: qn2

  cmp = pspio_qn_cmp(qn1%ptr, qn2%ptr)

end function pspiof_qn_cmp

! label
integer function pspiof_qn_label(qn, label) result(ierr)
  type(pspiof_qn_t), intent(in)  :: qn
  character(len=*),  intent(out) :: label

  character(kind=c_char,len=1) :: c_label(len(label)+1)

  ierr = pspio_qn_label(qn%ptr, c_label)
  if (ierr == PSPIO_SUCCESS) call c_to_f_string(c_label, label)

end function pspiof_qn_label
