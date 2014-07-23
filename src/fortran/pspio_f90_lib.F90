!! Copyright (C) 2012 M. Oliveira
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

module pspio_f90_lib_m

  use pspio_f90_types_m

  implicit none

  ! DO NOT EDIT THE FOLLOWING SECTION - ALL CHANGES WILL BE OVERWRITTEN!
  ! Add new definitions into pspio_common.h instead
  !%%% BEGIN PSPIO CONSTANTS
  integer, parameter :: PSPIO_STRLEN_ERROR = 1024
  integer, parameter :: PSPIO_STRLEN_LINE = 256
  integer, parameter :: PSPIO_STRLEN_TITLE = 80
  integer, parameter :: PSPIO_SUCCESS = 0
  integer, parameter :: PSPIO_ERROR = -1
  integer, parameter :: PSPIO_EFILE_CORRUPT = 1
  integer, parameter :: PSPIO_EFILE_FORMAT = 2
  integer, parameter :: PSPIO_EGSL = 3
  integer, parameter :: PSPIO_EIO = 4
  integer, parameter :: PSPIO_ENOFILE = 5
  integer, parameter :: PSPIO_ENOMEM = 6
  integer, parameter :: PSPIO_ENOSUPPORT = 7
  integer, parameter :: PSPIO_ETYPE = 8
  integer, parameter :: PSPIO_EVALUE = 9
  integer, parameter :: PSPIO_FMT_NFORMATS = 19
  integer, parameter :: PSPIO_FMT_UNKNOWN = -1
  integer, parameter :: PSPIO_FMT_NONE = 0
  integer, parameter :: PSPIO_FMT_ABINIT_1 = 1
  integer, parameter :: PSPIO_FMT_ABINIT_2 = 2
  integer, parameter :: PSPIO_FMT_ABINIT_3 = 3
  integer, parameter :: PSPIO_FMT_ABINIT_4 = 4
  integer, parameter :: PSPIO_FMT_ABINIT_5 = 5
  integer, parameter :: PSPIO_FMT_ABINIT_6 = 6
  integer, parameter :: PSPIO_FMT_ABINIT_7 = 7
  integer, parameter :: PSPIO_FMT_ABINIT_8 = 8
  integer, parameter :: PSPIO_FMT_ABINIT_9 = 9
  integer, parameter :: PSPIO_FMT_ABINIT_10 = 10
  integer, parameter :: PSPIO_FMT_ABINIT_11 = 11
  integer, parameter :: PSPIO_FMT_ABINIT_17 = 12
  integer, parameter :: PSPIO_FMT_ATOM = 13
  integer, parameter :: PSPIO_FMT_FHI98PP = 14
  integer, parameter :: PSPIO_FMT_OCTOPUS_HGH = 15
  integer, parameter :: PSPIO_FMT_SIESTA = 16
  integer, parameter :: PSPIO_FMT_UPF = 17
  integer, parameter :: PSPIO_FMT_XML = 18
  integer, parameter :: PSPIO_EQN_DIRAC = 1
  integer, parameter :: PSPIO_EQN_SCALAR_REL = 2
  integer, parameter :: PSPIO_EQN_SCHRODINGER = 3
  integer, parameter :: PSPIO_SCM_BHS = 1
  integer, parameter :: PSPIO_SCM_GTH = 2
  integer, parameter :: PSPIO_SCM_HAMANN = 3
  integer, parameter :: PSPIO_SCM_HGH = 4
  integer, parameter :: PSPIO_SCM_HSC = 5
  integer, parameter :: PSPIO_SCM_KERKER = 6
  integer, parameter :: PSPIO_SCM_MRPP = 7
  integer, parameter :: PSPIO_SCM_RRKJ = 8
  integer, parameter :: PSPIO_SCM_TM = 9
  integer, parameter :: PSPIO_SCM_TM2 = 10
  integer, parameter :: PSPIO_MESH_UNKNOWN = -1
  integer, parameter :: PSPIO_MESH_NONE = 0
  integer, parameter :: PSPIO_MESH_LOG1 = 1
  integer, parameter :: PSPIO_MESH_LOG2 = 2
  integer, parameter :: PSPIO_MESH_LINEAR = 3
  integer, parameter :: PSPIO_QN_DIFF = -1
  integer, parameter :: PSPIO_QN_EQUAL = -2
  integer, parameter :: PSPIO_QN_MTEQUAL = -3
  integer, parameter :: PSPIO_NLCC_UNKNOWN = -1
  integer, parameter :: PSPIO_NLCC_NONE = 0
  integer, parameter :: PSPIO_NLCC_FHI = 1
  integer, parameter :: PSPIO_NLCC_LOUIE = 2
  integer, parameter :: PSPIO_NLCC_TETER1 = 3
  integer, parameter :: PSPIO_NLCC_TETER2 = 4
  !%%% END PSPIO CONSTANTS

  ! pspio_pspdata
  interface
    integer function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer,                   intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      integer,                   intent(in) :: format
      character(len=*),          intent(in) :: filename
    end function pspio_f90_pspdata_write

    subroutine pspio_f90_pspdata_free(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end subroutine pspio_f90_pspdata_free

    subroutine pspio_f90_pspdata_get_symbol(pspdata, symbol)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      character(len=*),          intent(out) :: symbol
    end subroutine pspio_f90_pspdata_get_symbol

    subroutine pspio_f90_pspdata_get_z(pspdata, z)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: z
    end subroutine pspio_f90_pspdata_get_z

    subroutine pspio_f90_pspdata_get_zvalence(pspdata, zvalence)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: zvalence
    end subroutine pspio_f90_pspdata_get_zvalence

    subroutine pspio_f90_pspdata_get_l_max(pspdata, l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_max
    end subroutine pspio_f90_pspdata_get_l_max

    subroutine pspio_f90_pspdata_get_wave_eq(pspdata, wave_eq)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: wave_eq
    end subroutine pspio_f90_pspdata_get_wave_eq

    subroutine pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end subroutine pspio_f90_pspdata_get_mesh

    subroutine pspio_f90_pspdata_get_n_states(pspdata, n_states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_states
    end subroutine pspio_f90_pspdata_get_n_states

    subroutine pspio_f90_pspdata_get_state(pspdata, i, state)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(in)  :: i
      type(pspio_f90_state_t),   intent(out) :: state
    end subroutine pspio_f90_pspdata_get_state

    subroutine pspio_f90_pspdata_get_n_kbproj(pspdata, n_kbproj)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_kbproj
    end subroutine pspio_f90_pspdata_get_n_kbproj

    subroutine pspio_f90_pspdata_get_kb_projector(pspdata, i, projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer,                     intent(in)  :: i
      type(pspio_f90_projector_t), intent(out) :: projector
    end subroutine pspio_f90_pspdata_get_kb_projector

    subroutine pspio_f90_pspdata_get_l_local(pspdata, l_local)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_local
    end subroutine pspio_f90_pspdata_get_l_local

    subroutine pspio_f90_pspdata_get_kb_l_max(pspdata, kb_l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: kb_l_max
    end subroutine pspio_f90_pspdata_get_kb_l_max

    subroutine pspio_f90_pspdata_get_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: vlocal
    end subroutine pspio_f90_pspdata_get_vlocal

    subroutine pspio_f90_pspdata_get_xc(pspdata, xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_xc_t),      intent(out) :: xc
    end subroutine pspio_f90_pspdata_get_xc
  end interface

  ! pspio_mesh
  interface
    subroutine pspio_f90_mesh_get_np(mesh, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer,                intent(out) :: np
    end subroutine pspio_f90_mesh_get_np

    subroutine pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_f90_kind),   intent(inout) :: r
    end subroutine pspio_f90_mesh_get_r
  end interface

  ! pspio_state
  interface pspio_f90_state_wf_eval
    subroutine pspio_f90_state_wf_eval_s(state, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(in)  :: r
      real(pspio_f90_kind),    intent(out) :: wf
    end subroutine pspio_f90_state_wf_eval_s

    subroutine pspio_f90_state_wf_eval_v(state, np, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer,                 intent(in)  :: np
      real(pspio_f90_kind),    intent(in)  :: r(np)
      real(pspio_f90_kind),    intent(out) :: wf(np)
    end subroutine pspio_f90_state_wf_eval_v
  end interface

  interface
    subroutine pspio_f90_state_get_qn(state, n, l, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer,                 intent(out) :: n
      integer,                 intent(out) :: l
      real(pspio_f90_kind),    intent(out) :: j
    end subroutine pspio_f90_state_get_qn

    subroutine pspio_f90_state_get_occ(state, occ)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(out) :: occ
    end subroutine pspio_f90_state_get_occ
  end interface

  ! pspio_potential
  interface pspio_f90_potential_eval
    subroutine pspio_f90_potential_eval_s(potential, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: v
    end subroutine pspio_f90_potential_eval_s

    subroutine pspio_f90_potential_eval_v(potential, np, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      integer,                     intent(in)  :: np
      real(pspio_f90_kind),        intent(in)  :: r(np)
      real(pspio_f90_kind),        intent(out) :: v(np)
    end subroutine pspio_f90_potential_eval_v
  end interface pspio_f90_potential_eval

  ! pspio_projector
  interface pspio_f90_projector_eval
    subroutine pspio_f90_projector_eval_s(projector, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: p
    end subroutine pspio_f90_projector_eval_s

    subroutine pspio_f90_projector_eval_v(projector, np, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer,                     intent(in)  :: np
      real(pspio_f90_kind),        intent(in)  :: r(np)
      real(pspio_f90_kind),        intent(out) :: p(np)
    end subroutine pspio_f90_projector_eval_v
  end interface

  interface
    subroutine pspio_f90_projector_get_energy(projector, e)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: e
    end subroutine pspio_f90_projector_get_energy

    subroutine pspio_f90_projector_get_l(projector, l)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer,                     intent(out) :: l
    end subroutine pspio_f90_projector_get_l

    subroutine pspio_f90_projector_get_j(projector, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: j
    end subroutine pspio_f90_projector_get_j
  end interface

  ! pspio_xc
  interface
    subroutine pspio_f90_xc_get_correlation(xc, correlation)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(out) :: correlation
    end subroutine pspio_f90_xc_get_correlation
  end interface

  interface
    subroutine pspio_f90_xc_get_exchange(xc, exchange)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(out) :: exchange
    end subroutine pspio_f90_xc_get_exchange
  end interface

  interface
    subroutine pspio_f90_xc_has_nlcc_int(xc, has_nlcc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(out) :: has_nlcc
    end subroutine pspio_f90_xc_has_nlcc_int
  end interface

  interface pspio_f90_xc_nlcc_eval
    subroutine pspio_f90_xc_nlcc_eval_s(xc, r, core_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      real(pspio_f90_kind), intent(in)  :: r
      real(pspio_f90_kind), intent(out) :: core_dens
    end subroutine pspio_f90_xc_nlcc_eval_s

    subroutine pspio_f90_xc_nlcc_eval_v(xc, np, r, core_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(in)  :: np
      real(pspio_f90_kind), intent(in)  :: r(np)
      real(pspio_f90_kind), intent(out) :: core_dens(np)
    end subroutine pspio_f90_xc_nlcc_eval_v
  end interface

  ! pspio_error
  interface
    subroutine pspio_f90_error_add(filename, lineno)
      implicit none
      integer, intent(in) :: lineno
      character(len=*), intent(in) :: filename
    end subroutine pspio_f90_error_add

    integer function pspio_f90_error_fetchall(err_msg)
      implicit none
      character(len=PSPIO_STRLEN_ERROR), intent(out) :: err_msg
    end function pspio_f90_error_fetchall

    integer function pspio_f90_error_flush()
      implicit none
    end function pspio_f90_error_flush

    integer function pspio_f90_error_free()
      implicit none
    end function pspio_f90_error_free

    integer function pspio_f90_error_len()
      implicit none
    end function pspio_f90_error_len

    integer function pspio_f90_error_get()
      implicit none
    end function pspio_f90_error_get

    subroutine pspio_f90_error_set(eid)
      implicit none
      integer, intent(in) :: eid
    end subroutine pspio_f90_error_set
  end interface

  ! pspio_info
  interface
    integer function pspio_f90_version(major, minor, micro)
      implicit none
      integer, intent(out) :: major, minor, micro
    end function pspio_f90_version
  end interface

contains

  subroutine pspio_f90_xc_has_nlcc(xc, has_nlcc)
    use pspio_f90_types_m
    implicit none
    type(pspio_f90_xc_t), intent(in)  :: xc
    logical,              intent(out) :: has_nlcc
    
    integer :: i

    call pspio_f90_xc_has_nlcc_int(xc, i)
    has_nlcc = i /= 0

  end subroutine pspio_f90_xc_has_nlcc


end module pspio_f90_lib_m
