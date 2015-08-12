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

module fpspio_m
  use, intrinsic :: iso_c_binding
  implicit none

  private

  public :: &
    ! error
    fpspio_error_fetchall, &
    fpspio_error_flush, &
    fpspio_error_free, &
    fpspio_error_get_last, &
    fpspio_error_len, &
    fpspio_error_string, &
    ! info
    fpspio_info_version, &
    fpspio_info_string, &
    ! mesh
    fpspio_mesh_t, &
    fpspio_mesh_alloc, &
    fpspio_mesh_init, &
    fpspio_mesh_init_from_points, &
    fpspio_mesh_init_from_parameters, &
    fpspio_mesh_copy, &
    fpspio_mesh_free, &
    fpspio_mesh_get_np, &
    fpspio_mesh_get_a, &
    fpspio_mesh_get_b, &
    fpspio_mesh_get_r, &
    fpspio_mesh_get_rab, &
    fpspio_mesh_cmp, &
    ! meshfunc
    fpspio_meshfunc_t, &
    fpspio_meshfunc_alloc, &
    fpspio_meshfunc_init, &
    fpspio_meshfunc_copy, &
    fpspio_meshfunc_free, &
    fpspio_meshfunc_get_function, &
    fpspio_meshfunc_get_deriv1, &
    fpspio_meshfunc_get_deriv2, &
    fpspio_meshfunc_get_interp_method, &
    fpspio_meshfunc_get_mesh, &
    fpspio_meshfunc_cmp, &
    fpspio_meshfunc_eval, &
    fpspio_meshfunc_eval_deriv, &
    fpspio_meshfunc_eval_deriv2, &
    ! potential
    fpspio_potential_t, &
    fpspio_potential_alloc, &
    fpspio_potential_init, &
    fpspio_potential_copy, &
    fpspio_potential_free, &
    fpspio_potential_get_qn, &
    fpspio_potential_cmp, &
    fpspio_potential_eval, &
    fpspio_potential_eval_deriv, &
    fpspio_potential_eval_deriv2, &
    ! projector
    fpspio_projector_t, &
    fpspio_projector_alloc, &
    fpspio_projector_init, &
    fpspio_projector_copy, &
    fpspio_projector_free, &
    fpspio_projector_get_energy, &
    fpspio_projector_get_qn, &
    fpspio_projector_cmp, &
    fpspio_projector_eval, &
    fpspio_projector_eval_deriv, &
    fpspio_projector_eval_deriv2, &
    ! pspdata
    fpspio_pspdata_t, &
    fpspio_pspdata_alloc, &
    fpspio_pspdata_read, &
    fpspio_pspdata_write, &
    fpspio_pspdata_free, &
    fpspio_pspdata_set_symbol, &
    fpspio_pspdata_set_z, &
    fpspio_pspdata_set_zvalence, &
    fpspio_pspdata_set_nelvalence, &
    fpspio_pspdata_set_l_max, &
    fpspio_pspdata_set_wave_eq, &
    fpspio_pspdata_set_total_energy, &
    fpspio_pspdata_set_mesh, &
    fpspio_pspdata_set_n_states, &
    fpspio_pspdata_set_state, &
    fpspio_pspdata_set_scheme, &
    fpspio_pspdata_set_n_potentials, &
    fpspio_pspdata_set_potential, &
    fpspio_pspdata_set_n_projectors, &
    fpspio_pspdata_set_projector, &
    fpspio_pspdata_set_projectors_l_max, &
    fpspio_pspdata_set_l_local, &
    fpspio_pspdata_set_vlocal, &
    fpspio_pspdata_set_xc, &
    fpspio_pspdata_set_rho_valence, &
    fpspio_pspdata_get_format_guessed, &
    fpspio_pspdata_get_symbol, &
    fpspio_pspdata_get_z, &
    fpspio_pspdata_get_zvalence, &
    fpspio_pspdata_get_nelvalence, &
    fpspio_pspdata_get_l_max, &
    fpspio_pspdata_get_wave_eq, &
    fpspio_pspdata_get_total_energy, &
    fpspio_pspdata_get_mesh, &
    fpspio_pspdata_get_n_states, &
    fpspio_pspdata_get_state, &
    fpspio_pspdata_get_scheme, &
    fpspio_pspdata_get_n_potentials, &
    fpspio_pspdata_get_potential, &
    fpspio_pspdata_get_n_projectors, &
    fpspio_pspdata_get_projector, &
    fpspio_pspdata_get_projectors_l_max, &
    fpspio_pspdata_get_l_local, &
    fpspio_pspdata_get_vlocal, &
    fpspio_pspdata_get_xc, &
    fpspio_pspdata_get_rho_valence, &
    ! qn
    fpspio_qn_t, &
    fpspio_qn_alloc, &
    fpspio_qn_init, &
    fpspio_qn_copy, &
    fpspio_qn_free, &
    fpspio_qn_get_n, &
    fpspio_qn_get_l, &
    fpspio_qn_get_j, &
    fpspio_qn_cmp, &
    fpspio_qn_label, &
    ! state
    fpspio_state_t, &
    fpspio_state_alloc, &
    fpspio_state_init, &
    fpspio_state_copy, &
    fpspio_state_free, &
    fpspio_state_get_label, &
    fpspio_state_get_qn, &
    fpspio_state_get_occ, &
    fpspio_state_get_ev, &
    fpspio_state_get_rc, &
    fpspio_state_cmp, &
    fpspio_state_wf_eval, &
    fpspio_state_wf_eval_deriv, &
    fpspio_state_wf_eval_deriv2, &
    ! xc
    fpspio_xc_t, &
    fpspio_xc_alloc, &
    fpspio_xc_copy, &
    fpspio_xc_free, &
    fpspio_xc_set_exchange, &
    fpspio_xc_set_correlation, &
    fpspio_xc_set_nlcc_scheme, &
    fpspio_xc_set_nlcc_density, &
    fpspio_xc_get_exchange, &
    fpspio_xc_get_correlation, &
    fpspio_xc_get_nlcc_scheme, &
    fpspio_xc_cmp, &
    fpspio_xc_nlcc_density_eval, &
    fpspio_xc_nlcc_density_eval_deriv, &
    fpspio_xc_nlcc_density_eval_deriv2, &
    fpspio_xc_has_nlcc


  ! DO NOT EDIT THE FOLLOWING SECTION - ALL CHANGES WILL BE OVERWRITTEN!
  ! Add new definitions into pspio_common.h instead
  !%%% BEGIN PSPIO CONSTANTS
  integer(c_int), parameter, public :: PSPIO_STRLEN_ERROR = 1024
  integer(c_int), parameter, public :: PSPIO_STRLEN_LINE = 256
  integer(c_int), parameter, public :: PSPIO_STRLEN_TITLE = 80
  integer(c_int), parameter, public :: PSPIO_SUCCESS = 0
  integer(c_int), parameter, public :: PSPIO_ERROR = -1
  integer(c_int), parameter, public :: PSPIO_EFILE_CORRUPT = 1
  integer(c_int), parameter, public :: PSPIO_EFILE_FORMAT = 2
  integer(c_int), parameter, public :: PSPIO_EGSL = 3
  integer(c_int), parameter, public :: PSPIO_EIO = 4
  integer(c_int), parameter, public :: PSPIO_ENOFILE = 5
  integer(c_int), parameter, public :: PSPIO_ENOMEM = 6
  integer(c_int), parameter, public :: PSPIO_ENOSUPPORT = 7
  integer(c_int), parameter, public :: PSPIO_ETYPE = 8
  integer(c_int), parameter, public :: PSPIO_EVALUE = 9
  integer(c_int), parameter, public :: PSPIO_FMT_NFORMATS = 19
  integer(c_int), parameter, public :: PSPIO_FMT_UNKNOWN = -1
  integer(c_int), parameter, public :: PSPIO_FMT_NONE = 0
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_1 = 1
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_2 = 2
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_3 = 3
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_4 = 4
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_5 = 5
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_6 = 6
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_7 = 7
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_8 = 8
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_9 = 9
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_10 = 10
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_11 = 11
  integer(c_int), parameter, public :: PSPIO_FMT_ABINIT_17 = 12
  integer(c_int), parameter, public :: PSPIO_FMT_ATOM = 13
  integer(c_int), parameter, public :: PSPIO_FMT_FHI98PP = 14
  integer(c_int), parameter, public :: PSPIO_FMT_OCTOPUS_HGH = 15
  integer(c_int), parameter, public :: PSPIO_FMT_SIESTA = 16
  integer(c_int), parameter, public :: PSPIO_FMT_UPF = 17
  integer(c_int), parameter, public :: PSPIO_FMT_XML = 18
  integer(c_int), parameter, public :: PSPIO_EQN_DIRAC = 1
  integer(c_int), parameter, public :: PSPIO_EQN_SCALAR_REL = 2
  integer(c_int), parameter, public :: PSPIO_EQN_SCHRODINGER = 3
  integer(c_int), parameter, public :: PSPIO_SCM_UNKNOWN = 0
  integer(c_int), parameter, public :: PSPIO_SCM_BHS = 1
  integer(c_int), parameter, public :: PSPIO_SCM_GTH = 2
  integer(c_int), parameter, public :: PSPIO_SCM_HAMANN = 3
  integer(c_int), parameter, public :: PSPIO_SCM_HGH = 4
  integer(c_int), parameter, public :: PSPIO_SCM_HSC = 5
  integer(c_int), parameter, public :: PSPIO_SCM_KERKER = 6
  integer(c_int), parameter, public :: PSPIO_SCM_MRPP = 7
  integer(c_int), parameter, public :: PSPIO_SCM_RRKJ = 8
  integer(c_int), parameter, public :: PSPIO_SCM_TM = 9
  integer(c_int), parameter, public :: PSPIO_SCM_TM2 = 10
  integer(c_int), parameter, public :: PSPIO_SCM_RTM = 11
  integer(c_int), parameter, public :: PSPIO_MESH_UNKNOWN = -1
  integer(c_int), parameter, public :: PSPIO_MESH_NONE = 0
  integer(c_int), parameter, public :: PSPIO_MESH_LOG1 = 1
  integer(c_int), parameter, public :: PSPIO_MESH_LOG2 = 2
  integer(c_int), parameter, public :: PSPIO_MESH_LINEAR = 3
  integer(c_int), parameter, public :: PSPIO_QN_DIFF = -1
  integer(c_int), parameter, public :: PSPIO_QN_EQUAL = -2
  integer(c_int), parameter, public :: PSPIO_QN_MTEQUAL = -3
  integer(c_int), parameter, public :: PSPIO_INTERP_GSL_CSPLINE = 1
  integer(c_int), parameter, public :: PSPIO_INTERP_JB_CSPLINE = 2
  integer(c_int), parameter, public :: PSPIO_NLCC_UNKNOWN = -1
  integer(c_int), parameter, public :: PSPIO_NLCC_NONE = 0
  integer(c_int), parameter, public :: PSPIO_NLCC_FHI = 1
  integer(c_int), parameter, public :: PSPIO_NLCC_LOUIE = 2
  integer(c_int), parameter, public :: PSPIO_NLCC_TETER1 = 3
  integer(c_int), parameter, public :: PSPIO_NLCC_TETER2 = 4
  integer(c_int), parameter, public :: PSPIO_NLCC_ATOM = 5
  !%%% END PSPIO CONSTANTS


  type :: fpspio_qn_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_qn_t

  type fpspio_mesh_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_mesh_t

  type fpspio_meshfunc_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_meshfunc_t

  type fpspio_potential_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_potential_t

  type fpspio_projector_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_projector_t

  type fpspio_state_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_state_t

  type fpspio_xc_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_xc_t

  type fpspio_pspdata_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_pspdata_t

#include "interface_info_inc.F90"
#include "interface_error_inc.F90"
#include "interface_qn_inc.F90"
#include "interface_mesh_inc.F90"
#include "interface_meshfunc_inc.F90"
#include "interface_state_inc.F90"
#include "interface_potential_inc.F90"
#include "interface_projector_inc.F90"
#include "interface_xc_inc.F90"
#include "interface_pspdata_inc.F90"

contains

#include "api_info_inc.F90"
#include "api_error_inc.F90"
#include "api_qn_inc.F90"
#include "api_mesh_inc.F90"
#include "api_meshfunc_inc.F90"
#include "api_state_inc.F90"
#include "api_potential_inc.F90"
#include "api_projector_inc.F90"
#include "api_xc_inc.F90"
#include "api_pspdata_inc.F90"


  ! Helper functions to convert between C and Fortran strings
  ! Based on the routines by Joseph M. Krahn
  function f_to_c_string(f_string) result(c_string)
    character(len=*), intent(in) :: f_string
    character(kind=c_char,len=1) :: c_string(len_trim(f_string)+1)
      
    integer :: i, strlen

    strlen = len_trim(f_string)

    forall (i=1:strlen)
      c_string(i) = f_string(i:i)
    end forall
    c_string(strlen+1) = C_NULL_CHAR

  end function f_to_c_string

  subroutine c_to_f_string(c_string, f_string)
    character(kind=c_char,len=1), intent(in)  :: c_string(*)
    character(len=*),             intent(out) :: f_string

    integer :: i

    i = 1
    do while(c_string(i) /= C_NULL_CHAR .and. i <= len(f_string))
      f_string(i:i) = c_string(i)
      i = i + 1
    end do
    if (i < len(f_string)) f_string(i:) = ' '

  end subroutine c_to_f_string

  subroutine c_to_f_string_ptr(c_string, f_string)
    type(c_ptr),      intent(in)  :: c_string
    character(len=*), intent(out) :: f_string

    character(len=1, kind=c_char), pointer :: p_chars(:)
    integer :: i

    if (.not. c_associated(c_string)) then
      f_string = ' '
    else
      call c_f_pointer(c_string, p_chars, [huge(0)])
      i = 1
      do while(p_chars(i) /= C_NULL_CHAR .and. i <= len(f_string))
        f_string(i:i) = p_chars(i)
        i = i + 1
      end do
      if (i < len(f_string)) f_string(i:) = ' '
    end if

  end subroutine c_to_f_string_ptr
  
end module fpspio_m
