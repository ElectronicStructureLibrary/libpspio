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
integer function fpspio_state_alloc(state, np) result(ierr)
  type(fpspio_state_t), intent(inout) :: state
  integer,     intent(in)    :: np

  ierr = pspio_state_alloc(state%ptr, np)

end function fpspio_state_alloc

! init
integer function fpspio_state_init(state, eigenval, qn, occ, rc, mesh, wf, label) result(ierr)
  type(fpspio_state_t), intent(inout) :: state
  real(8),              intent(in)    :: eigenval
  type(fpspio_qn_t),    intent(in)    :: qn
  real(8),              intent(in)    :: occ
  real(8),              intent(in)    :: rc
  type(fpspio_mesh_t),  intent(in)    :: mesh
  real(8),              intent(in)    :: wf(*)
  character(len=*),     intent(in)    :: label

  ierr = pspio_state_init(state%ptr, eigenval, qn, occ, rc, mesh%ptr, wf, f_to_c_string(label))

end function fpspio_state_init

! copy
integer function fpspio_state_copy(src, dst) result(ierr)
  type(fpspio_state_t), intent(in)    :: src
  type(fpspio_state_t), intent(inout) :: dst

  ierr = pspio_state_copy(src%ptr, dst%ptr)

end function fpspio_state_copy

! free
subroutine fpspio_state_free(state)
  type(fpspio_state_t), intent(inout) :: state

  call pspio_state_free(state%ptr)
  state%ptr = C_NULL_PTR

end subroutine fpspio_state_free


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! label
character(len=10) function fpspio_state_get_label(state) result(label)
  type(fpspio_state_t), intent(in)  :: state

  call c_to_f_string_ptr(pspio_state_get_label(state%ptr), label)

end function fpspio_state_get_label

! qn
type(fpspio_qn_t) function fpspio_state_get_qn(state) result(qn)
  type(fpspio_state_t), intent(in)  :: state

  qn = pspio_state_get_qn(state%ptr)

end function fpspio_state_get_qn

! occ
real(8) function fpspio_state_get_occ(state) result(occ)
  type(fpspio_state_t), intent(in)  :: state

  occ = pspio_state_get_occ(state%ptr)

end function fpspio_state_get_occ

! ev
real(8) function fpspio_state_get_ev(state) result(ev)
  type(fpspio_state_t), intent(in)  :: state
    
  ev = pspio_state_get_ev(state%ptr)
  
end function fpspio_state_get_ev

! rc
real(8) function fpspio_state_get_rc(state) result(rc)
  type(fpspio_state_t), intent(in)  :: state
     
  rc = pspio_state_get_rc(state%ptr)
   
end function fpspio_state_get_rc


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function fpspio_state_cmp(state1, state2) result(cmp)
  type(fpspio_state_t) :: state1
  type(fpspio_state_t) :: state2

  cmp = pspio_state_cmp(state1%ptr, state2%ptr)

end function fpspio_state_cmp

! wf_eval
real(8) function fpspio_state_wf_eval(state, r) result(wf)
  type(fpspio_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wf = pspio_state_wf_eval(state%ptr, r)

end function fpspio_state_wf_eval

! wf_eval_deriv
real(8) function fpspio_state_wf_eval_deriv(state, r) result(wfp)
  type(fpspio_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wfp = pspio_state_wf_eval_deriv(state%ptr, r)

end function fpspio_state_wf_eval_deriv

! wf_eval_deriv2
real(8) function fpspio_state_wf_eval_deriv2(state, r) result(wfpp)
  type(fpspio_state_t), intent(in)  :: state
  real(8),     intent(in)  :: r

  wfpp = pspio_state_wf_eval_deriv2(state%ptr, r)

end function fpspio_state_wf_eval_deriv2

