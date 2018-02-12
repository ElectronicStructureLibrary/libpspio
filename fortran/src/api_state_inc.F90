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
integer function pspiof_state_alloc(state, np) result(ierr)
  type(pspiof_state_t), intent(inout) :: state
  integer,     intent(in)    :: np

  ierr = pspio_state_alloc(state%ptr, np)

end function pspiof_state_alloc

! init
integer function pspiof_state_init(state, eigenval, qn, occ, rc, mesh, wf, label) result(ierr)
  type(pspiof_state_t), intent(inout) :: state
  real(8),              intent(in)    :: eigenval
  type(pspiof_qn_t),    intent(in)    :: qn
  real(8),              intent(in)    :: occ
  real(8),              intent(in)    :: rc
  type(pspiof_mesh_t),  intent(in)    :: mesh
  real(8),              intent(in)    :: wf(*)
  character(len=*),     intent(in)    :: label

  ierr = pspio_state_init(state%ptr, eigenval, qn%ptr, occ, rc, mesh%ptr, wf, f_to_c_string(label))

end function pspiof_state_init

! copy
integer function pspiof_state_copy(src, dst) result(ierr)
  type(pspiof_state_t), intent(in)    :: src
  type(pspiof_state_t), intent(inout) :: dst

  ierr = pspio_state_copy(src%ptr, dst%ptr)

end function pspiof_state_copy

! free
subroutine pspiof_state_free(state)
  type(pspiof_state_t), intent(inout) :: state

  call pspio_state_free(state%ptr)
  state%ptr = C_NULL_PTR

end subroutine pspiof_state_free

! associated
logical function pspiof_state_associated(state) result(is_associated)
  type(pspiof_state_t), intent(in) :: state

  is_associated = c_associated(state%ptr)

end function pspiof_state_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! label
character(len=10) function pspiof_state_get_label(state) result(label)
  type(pspiof_state_t), intent(in)  :: state

  call c_to_f_string_ptr(pspio_state_get_label(state%ptr), label)

end function pspiof_state_get_label

! qn
type(pspiof_qn_t) function pspiof_state_get_qn(state) result(qn)
  type(pspiof_state_t), intent(in)  :: state

  qn%ptr = pspio_state_get_qn(state%ptr)

end function pspiof_state_get_qn

! occ
real(8) function pspiof_state_get_occ(state) result(occ)
  type(pspiof_state_t), intent(in)  :: state

  occ = pspio_state_get_occ(state%ptr)

end function pspiof_state_get_occ

! ev
real(8) function pspiof_state_get_ev(state) result(ev)
  type(pspiof_state_t), intent(in)  :: state
    
  ev = pspio_state_get_ev(state%ptr)
  
end function pspiof_state_get_ev

! rc
real(8) function pspiof_state_get_rc(state) result(rc)
  type(pspiof_state_t), intent(in)  :: state
     
  rc = pspio_state_get_rc(state%ptr)
   
end function pspiof_state_get_rc

! wf
type(pspiof_meshfunc_t) function pspiof_state_get_wf(state) result(wf)
  type(pspiof_state_t), intent(in)  :: state

  wf%ptr = pspio_state_get_wf(state%ptr)

end function pspiof_state_get_wf


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_state_cmp(state1, state2) result(cmp)
  type(pspiof_state_t) :: state1
  type(pspiof_state_t) :: state2

  cmp = pspio_state_cmp(state1%ptr, state2%ptr)

end function pspiof_state_cmp

! wf_eval
real(8) function pspiof_state_wf_eval(state, r) result(wf)
  type(pspiof_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wf = pspio_state_wf_eval(state%ptr, r)

end function pspiof_state_wf_eval

! wf_eval_deriv
real(8) function pspiof_state_wf_eval_deriv(state, r) result(wfp)
  type(pspiof_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wfp = pspio_state_wf_eval_deriv(state%ptr, r)

end function pspiof_state_wf_eval_deriv

! wf_eval_deriv2
real(8) function pspiof_state_wf_eval_deriv2(state, r) result(wfpp)
  type(pspiof_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wfpp = pspio_state_wf_eval_deriv2(state%ptr, r)

end function pspiof_state_wf_eval_deriv2

