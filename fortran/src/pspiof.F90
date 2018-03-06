!! Copyright (C) 2015-2018 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

module pspiof_m
  use, intrinsic :: iso_c_binding
  implicit none

  private

  public :: &
    ! error
    pspiof_error_add, &
    pspiof_error_fetchall, &
    pspiof_error_flush, &
    pspiof_error_free, &
    pspiof_error_get_last, &
    pspiof_error_len, &
    pspiof_error_string, &
    ! info
    pspiof_info_version, &
    pspiof_info_string, &
    ! mesh
    pspiof_mesh_t, &
    pspiof_mesh_alloc, &
    pspiof_mesh_init, &
    pspiof_mesh_init_from_points, &
    pspiof_mesh_init_from_parameters, &
    pspiof_mesh_copy, &
    pspiof_mesh_free, &
    pspiof_mesh_get_np, &
    pspiof_mesh_get_a, &
    pspiof_mesh_get_b, &
    pspiof_mesh_get_r, &
    pspiof_mesh_get_rab, &
    pspiof_mesh_cmp, &
    ! meshfunc
    pspiof_meshfunc_t, &
    pspiof_meshfunc_alloc, &
    pspiof_meshfunc_init, &
    pspiof_meshfunc_copy, &
    pspiof_meshfunc_free, &
    pspiof_meshfunc_get_function, &
    pspiof_meshfunc_get_deriv1, &
    pspiof_meshfunc_get_deriv2, &
    pspiof_meshfunc_get_interp_method, &
    pspiof_meshfunc_get_mesh, &
    pspiof_meshfunc_cmp, &
    pspiof_meshfunc_eval, &
    pspiof_meshfunc_eval_deriv, &
    pspiof_meshfunc_eval_deriv2, &
    ! potential
    pspiof_potential_t, &
    pspiof_potential_alloc, &
    pspiof_potential_init, &
    pspiof_potential_copy, &
    pspiof_potential_free, &
    pspiof_potential_get_qn, &
    pspiof_potential_cmp, &
    pspiof_potential_eval, &
    pspiof_potential_eval_deriv, &
    pspiof_potential_eval_deriv2, &
    ! projector
    pspiof_projector_t, &
    pspiof_projector_alloc, &
    pspiof_projector_init, &
    pspiof_projector_copy, &
    pspiof_projector_free, &
    pspiof_projector_get_energy, &
    pspiof_projector_get_qn, &
    pspiof_projector_cmp, &
    pspiof_projector_eval, &
    pspiof_projector_eval_deriv, &
    pspiof_projector_eval_deriv2, &
    ! pspdata
    pspiof_pspdata_t, &
    pspiof_pspdata_alloc, &
    pspiof_pspdata_read, &
    pspiof_pspdata_write, &
    pspiof_pspdata_free, &
    pspiof_pspdata_set_pspinfo, &
    pspiof_pspdata_set_symbol, &
    pspiof_pspdata_set_z, &
    pspiof_pspdata_set_zvalence, &
    pspiof_pspdata_set_nelvalence, &
    pspiof_pspdata_set_l_max, &
    pspiof_pspdata_set_wave_eq, &
    pspiof_pspdata_set_total_energy, &
    pspiof_pspdata_set_mesh, &
    pspiof_pspdata_set_n_states, &
    pspiof_pspdata_set_state, &
    pspiof_pspdata_set_scheme, &
    pspiof_pspdata_set_n_potentials, &
    pspiof_pspdata_set_potential, &
    pspiof_pspdata_set_n_projectors, &
    pspiof_pspdata_set_projector, &
    pspiof_pspdata_set_projectors_l_max, &
    pspiof_pspdata_set_l_local, &
    pspiof_pspdata_set_vlocal, &
    pspiof_pspdata_set_xc, &
    pspiof_pspdata_set_rho_valence, &
    pspiof_pspdata_get_format_guessed, &
    pspiof_pspdata_get_pspinfo, &
    pspiof_pspdata_get_symbol, &
    pspiof_pspdata_get_z, &
    pspiof_pspdata_get_zvalence, &
    pspiof_pspdata_get_nelvalence, &
    pspiof_pspdata_get_l_max, &
    pspiof_pspdata_get_wave_eq, &
    pspiof_pspdata_get_total_energy, &
    pspiof_pspdata_get_mesh, &
    pspiof_pspdata_get_n_states, &
    pspiof_pspdata_get_state, &
    pspiof_pspdata_get_scheme, &
    pspiof_pspdata_get_n_potentials, &
    pspiof_pspdata_get_potential, &
    pspiof_pspdata_get_n_projectors, &
    pspiof_pspdata_get_projector, &
    pspiof_pspdata_get_projectors_l_max, &
    pspiof_pspdata_get_l_local, &
    pspiof_pspdata_get_vlocal, &
    pspiof_pspdata_get_xc, &
    pspiof_pspdata_get_rho_valence, &
    pspiof_pspdata_get_projector_energy, &
    ! pspinfo
    pspiof_pspinfo_t, &
    pspiof_pspinfo_alloc, &
    pspiof_pspinfo_copy, &
    pspiof_pspinfo_free, &
    pspiof_pspinfo_set_author, &
    pspiof_pspinfo_set_code_name, &
    pspiof_pspinfo_set_code_version, &
    pspiof_pspinfo_set_generation_day, &
    pspiof_pspinfo_set_generation_month, &
    pspiof_pspinfo_set_generation_year, &
    pspiof_pspinfo_set_description, &
    pspiof_pspinfo_get_author, &
    pspiof_pspinfo_get_code_name, &
    pspiof_pspinfo_get_code_version, &
    pspiof_pspinfo_get_generation_day, &
    pspiof_pspinfo_get_generation_month, &
    pspiof_pspinfo_get_generation_year, &
    pspiof_pspinfo_get_description, &
    pspiof_pspinfo_cmp, &
    ! qn
    pspiof_qn_t, &
    pspiof_qn_alloc, &
    pspiof_qn_init, &
    pspiof_qn_copy, &
    pspiof_qn_free, &
    pspiof_qn_get_n, &
    pspiof_qn_get_l, &
    pspiof_qn_get_j, &
    pspiof_qn_cmp, &
    pspiof_qn_label, &
    ! state
    pspiof_state_t, &
    pspiof_state_alloc, &
    pspiof_state_init, &
    pspiof_state_copy, &
    pspiof_state_free, &
    pspiof_state_get_label, &
    pspiof_state_get_qn, &
    pspiof_state_get_occ, &
    pspiof_state_get_ev, &
    pspiof_state_get_rc, &
    pspiof_state_get_wf, &
    pspiof_state_cmp, &
    pspiof_state_wf_eval, &
    pspiof_state_wf_eval_deriv, &
    pspiof_state_wf_eval_deriv2, &
    ! xc
    pspiof_xc_t, &
    pspiof_xc_alloc, &
    pspiof_xc_copy, &
    pspiof_xc_free, &
    pspiof_xc_set_exchange, &
    pspiof_xc_set_correlation, &
    pspiof_xc_set_nlcc_scheme, &
    pspiof_xc_set_nlcc_density, &
    pspiof_xc_get_exchange, &
    pspiof_xc_get_correlation, &
    pspiof_xc_get_nlcc_scheme, &
    pspiof_xc_cmp, &
    pspiof_xc_nlcc_density_eval, &
    pspiof_xc_nlcc_density_eval_deriv, &
    pspiof_xc_nlcc_density_eval_deriv2, &
    pspiof_xc_has_nlcc, &
    ! associated
    pspiof_associated

  interface pspiof_associated
    module procedure pspiof_qn_associated, pspiof_mesh_associated, pspiof_meshfunc_associated, &
      pspiof_xc_associated, pspiof_potential_associated, pspiof_projector_associated, &
      pspiof_state_associated, pspiof_pspdata_associated, pspiof_pspinfo_associated
  end interface

  ! DO NOT EDIT THE FOLLOWING SECTION - ALL CHANGES WILL BE OVERWRITTEN!
  ! Add new definitions into pspio_common.h instead
  !%%% BEGIN PSPIO CONSTANTS
  integer(c_int), parameter, public :: PSPIO_STRLEN_ERROR = 1024
  integer(c_int), parameter, public :: PSPIO_STRLEN_LINE = 256
  integer(c_int), parameter, public :: PSPIO_STRLEN_TITLE = 80
  integer(c_int), parameter, public :: PSPIO_STRLEN_DESCRIPTION = 4096
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
  integer(c_int), parameter, public :: PSPIO_SCM_ONCV = 12
  integer(c_int), parameter, public :: PSPIO_MESH_UNKNOWN = -1
  integer(c_int), parameter, public :: PSPIO_MESH_NONE = 0
  integer(c_int), parameter, public :: PSPIO_MESH_LOG1 = 1
  integer(c_int), parameter, public :: PSPIO_MESH_LOG2 = 2
  integer(c_int), parameter, public :: PSPIO_MESH_LINEAR = 3
  integer(c_int), parameter, public :: PSPIO_DIFF = -1
  integer(c_int), parameter, public :: PSPIO_EQUAL = -2
  integer(c_int), parameter, public :: PSPIO_MTEQUAL = -3
  integer(c_int), parameter, public :: PSPIO_INTERP_GSL_CSPLINE = 1
  integer(c_int), parameter, public :: PSPIO_INTERP_JB_CSPLINE = 2
  integer(c_int), parameter, public :: PSPIO_NLCC_UNKNOWN = -1
  integer(c_int), parameter, public :: PSPIO_NLCC_NONE = 0
  integer(c_int), parameter, public :: PSPIO_NLCC_FHI = 1
  integer(c_int), parameter, public :: PSPIO_NLCC_LOUIE = 2
  integer(c_int), parameter, public :: PSPIO_NLCC_TETER1 = 3
  integer(c_int), parameter, public :: PSPIO_NLCC_TETER2 = 4
  integer(c_int), parameter, public :: PSPIO_NLCC_ATOM = 5
  integer(c_int), parameter, public :: PSPIO_NLCC_ONCV = 5
  !%%% END PSPIO CONSTANTS


  type :: pspiof_qn_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_qn_t

  type pspiof_mesh_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_mesh_t

  type pspiof_meshfunc_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_meshfunc_t

  type pspiof_potential_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_potential_t

  type pspiof_projector_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_projector_t

  type pspiof_state_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_state_t

  type pspiof_xc_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_xc_t

  type pspiof_pspinfo_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_pspinfo_t
  
  type pspiof_pspdata_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type pspiof_pspdata_t

#include "interface_info_inc.F90"
#include "interface_error_inc.F90"
#include "interface_qn_inc.F90"
#include "interface_mesh_inc.F90"
#include "interface_meshfunc_inc.F90"
#include "interface_state_inc.F90"
#include "interface_potential_inc.F90"
#include "interface_projector_inc.F90"
#include "interface_xc_inc.F90"
#include "interface_pspinfo_inc.F90"
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
#include "api_pspinfo_inc.F90"
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
  
end module pspiof_m
