!! Copyright (C) 2012 M. Oliveira
!! Copyright (C) 2014 M. Oliveira
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

  ! pspio_pspdata
  interface ! Global routines
    integer(pspio_cint) function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer(pspio_cint) function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer(pspio_cint) function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      integer(pspio_cint),       intent(in) :: format
      character(len=*),          intent(in) :: filename
    end function pspio_f90_pspdata_write

    subroutine pspio_f90_pspdata_free(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end subroutine pspio_f90_pspdata_free
  end interface

  interface ! symbol
    subroutine pspio_f90_pspdata_set_symbol(pspdata, symbol)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      character(len=*),          intent(in)    :: symbol
    end subroutine pspio_f90_pspdata_set_symbol

    subroutine pspio_f90_pspdata_get_symbol(pspdata, symbol)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      character(len=*),          intent(out) :: symbol
    end subroutine pspio_f90_pspdata_get_symbol
  end interface

  interface ! z
    subroutine pspio_f90_pspdata_set_z(pspdata, z)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      real(pspio_cdouble),       intent(in)    :: z
    end subroutine pspio_f90_pspdata_set_z

    subroutine pspio_f90_pspdata_get_z(pspdata, z)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_cdouble),       intent(out) :: z
    end subroutine pspio_f90_pspdata_get_z
  end interface

  interface ! zvalence
    subroutine pspio_f90_pspdata_set_zvalence(pspdata, zvalence)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      real(pspio_cdouble),       intent(in)    :: zvalence
    end subroutine pspio_f90_pspdata_set_zvalence

    subroutine pspio_f90_pspdata_get_zvalence(pspdata, zvalence)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_cdouble),       intent(out) :: zvalence
    end subroutine pspio_f90_pspdata_get_zvalence
  end interface

  interface ! l_max
    subroutine pspio_f90_pspdata_set_l_max(pspdata, l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: l_max
    end subroutine pspio_f90_pspdata_set_l_max

    subroutine pspio_f90_pspdata_get_l_max(pspdata, l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: l_max
    end subroutine pspio_f90_pspdata_get_l_max
  end interface

  interface ! wave_eq
    subroutine pspio_f90_pspdata_set_wave_eq(pspdata, wave_eq)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: wave_eq
    end subroutine pspio_f90_pspdata_set_wave_eq

    subroutine pspio_f90_pspdata_get_wave_eq(pspdata, wave_eq)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: wave_eq
    end subroutine pspio_f90_pspdata_get_wave_eq
  end interface

  interface ! mesh
    subroutine pspio_f90_pspdata_set_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      type(pspio_f90_mesh_t),    intent(in)    :: mesh
    end subroutine pspio_f90_pspdata_set_mesh

    subroutine pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end subroutine pspio_f90_pspdata_get_mesh
  end interface

  interface ! n_states
    subroutine pspio_f90_pspdata_set_n_states(pspdata, n_states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: n_states
    end subroutine pspio_f90_pspdata_set_n_states

    subroutine pspio_f90_pspdata_get_n_states(pspdata, n_states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: n_states
    end subroutine pspio_f90_pspdata_get_n_states
  end interface

  interface ! states
    subroutine pspio_f90_pspdata_set_states(pspdata, states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      type(pspio_f90_state_t),   intent(in) :: states(*)
    end subroutine pspio_f90_pspdata_set_states

    subroutine pspio_f90_pspdata_get_states(pspdata, states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_state_t),   intent(out) :: states(*)
    end subroutine pspio_f90_pspdata_get_states
  end interface

  interface ! n_potentials
    subroutine pspio_f90_pspdata_set_n_potentials(pspdata, n_potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: n_potentials
    end subroutine pspio_f90_pspdata_set_n_potentials

    integer(pspio_cint) function pspio_f90_pspdata_get_n_potentials(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_n_potentials
  end interface

  interface ! potentials
    subroutine pspio_f90_pspdata_set_potentials(pspdata, potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_potential_t), intent(in)    :: potentials(*)
    end subroutine pspio_f90_pspdata_set_potentials

    subroutine pspio_f90_pspdata_get_potentials(pspdata, potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: potentials(*)
    end subroutine pspio_f90_pspdata_get_potentials
  end interface

  interface ! n_kbproj
    subroutine pspio_f90_pspdata_set_n_kbproj(pspdata, n_kbproj)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: n_kbproj
    end subroutine pspio_f90_pspdata_set_n_kbproj

    subroutine pspio_f90_pspdata_get_n_kbproj(pspdata, n_kbproj)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: n_kbproj
    end subroutine pspio_f90_pspdata_get_n_kbproj
  end interface

  interface ! kb_projectors
    subroutine pspio_f90_pspdata_set_kb_projectors(pspdata, kb_projectors)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_projector_t), intent(in)    :: kb_projectors(*)
    end subroutine pspio_f90_pspdata_set_kb_projectors

    subroutine pspio_f90_pspdata_get_kb_projectors(pspdata, kb_projectors)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_projector_t), intent(out) :: kb_projectors(*)
    end subroutine pspio_f90_pspdata_get_kb_projectors
  end interface

  interface ! l_local
    subroutine pspio_f90_pspdata_set_l_local(pspdata, l_local)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: l_local
    end subroutine pspio_f90_pspdata_set_l_local

    subroutine pspio_f90_pspdata_get_l_local(pspdata, l_local)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: l_local
    end subroutine pspio_f90_pspdata_get_l_local
  end interface

  interface ! kb_l_max
    subroutine pspio_f90_pspdata_set_kb_l_max(pspdata, kb_l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: kb_l_max
    end subroutine pspio_f90_pspdata_set_kb_l_max

    subroutine pspio_f90_pspdata_get_kb_l_max(pspdata, kb_l_max)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: kb_l_max
    end subroutine pspio_f90_pspdata_get_kb_l_max
  end interface

  interface ! vlocal
    subroutine pspio_f90_pspdata_set_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_potential_t), intent(in)    :: vlocal
    end subroutine pspio_f90_pspdata_set_vlocal

    subroutine pspio_f90_pspdata_get_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: vlocal
    end subroutine pspio_f90_pspdata_get_vlocal
  end interface

  interface ! xc
    subroutine pspio_f90_pspdata_set_xc(pspdata, xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      type(pspio_f90_xc_t),      intent(in)    :: xc
    end subroutine pspio_f90_pspdata_set_xc

    subroutine pspio_f90_pspdata_get_xc(pspdata, xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_xc_t),      intent(out) :: xc
    end subroutine pspio_f90_pspdata_get_xc
  end interface

  interface ! rho_valence
    integer(pspio_cint) function pspio_f90_pspdata_set_rho_valence(pspdata, rho_valence)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),  intent(inout) :: pspdata
      real(pspio_cdouble),        intent(in)    :: rho_valence(*)
    end function pspio_f90_pspdata_set_rho_valence
  end interface

  interface pspio_f90_pspdata_rho_valence_eval
    subroutine pspio_f90_pspdata_rho_valence_eval_s(pspdata, r, rho)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_cdouble),       intent(in)  :: r
      real(pspio_cdouble),       intent(out) :: rho
    end subroutine pspio_f90_pspdata_rho_valence_eval_s

    subroutine pspio_f90_pspdata_rho_valence_eval_v(pspdata, np, r, rho)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(in)  :: np
      real(pspio_cdouble),       intent(in)  :: r(np)
      real(pspio_cdouble),       intent(out) :: rho(np)
    end subroutine pspio_f90_pspdata_rho_valence_eval_v
  end interface

  ! pspio_mesh
  interface
    subroutine pspio_f90_mesh_get_info(mesh, mesh_type, np, a, b)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer(pspio_cint),    intent(out) :: mesh_type
      integer(pspio_cint),    intent(out) :: np
      real(pspio_cdouble),    intent(out) :: a
      real(pspio_cdouble),    intent(out) :: b
    end subroutine pspio_f90_mesh_get_info
  end interface

  interface ! 
    subroutine pspio_f90_mesh_get_np(mesh, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer(pspio_cint),    intent(out) :: np
    end subroutine pspio_f90_mesh_get_np
  end interface

  interface ! 
    subroutine pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),   intent(out) :: r(*)
    end subroutine pspio_f90_mesh_get_r
  end interface

  interface ! 
    subroutine pspio_f90_mesh_get_rab(mesh, rab)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),    intent(out) :: rab(*)
    end subroutine pspio_f90_mesh_get_rab
  end interface

  ! pspio_state
  interface pspio_f90_state_wf_eval
    subroutine pspio_f90_state_wf_eval_s(state, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_cdouble),     intent(in)  :: r
      real(pspio_cdouble),     intent(out) :: wf
    end subroutine pspio_f90_state_wf_eval_s

    subroutine pspio_f90_state_wf_eval_v(state, np, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer(pspio_cint),     intent(in)  :: np
      real(pspio_cdouble),     intent(in)  :: r(np)
      real(pspio_cdouble),     intent(out) :: wf(np)
    end subroutine pspio_f90_state_wf_eval_v
  end interface

  interface
    subroutine pspio_f90_state_get_qn(state, n, l, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer(pspio_cint),     intent(out) :: n
      integer(pspio_cint),     intent(out) :: l
      real(pspio_cdouble),     intent(out) :: j
    end subroutine pspio_f90_state_get_qn
  end interface

  interface ! 
    subroutine pspio_f90_state_get_occ(state, occ)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_cdouble),     intent(out) :: occ
    end subroutine pspio_f90_state_get_occ
  end interface

  ! pspio_potential
  interface pspio_f90_potential_eval
    subroutine pspio_f90_potential_eval_s(potential, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      real(pspio_cdouble),         intent(in)  :: r
      real(pspio_cdouble),         intent(out) :: v
    end subroutine pspio_f90_potential_eval_s

    subroutine pspio_f90_potential_eval_v(potential, np, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      integer(pspio_cint),         intent(in)  :: np
      real(pspio_cdouble),         intent(in)  :: r(np)
      real(pspio_cdouble),         intent(out) :: v(np)
    end subroutine pspio_f90_potential_eval_v
  end interface pspio_f90_potential_eval

  ! pspio_projector
  interface pspio_f90_projector_eval
    subroutine pspio_f90_projector_eval_s(projector, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_cdouble),         intent(in)  :: r
      real(pspio_cdouble),         intent(out) :: p
    end subroutine pspio_f90_projector_eval_s

    subroutine pspio_f90_projector_eval_v(projector, np, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer(pspio_cint),         intent(in)  :: np
      real(pspio_cdouble),         intent(in)  :: r(np)
      real(pspio_cdouble),         intent(out) :: p(np)
    end subroutine pspio_f90_projector_eval_v
  end interface

  interface
    subroutine pspio_f90_projector_get_energy(projector, energy)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_cdouble),         intent(out) :: energy
    end subroutine pspio_f90_projector_get_energy
  end interface

  interface ! 
    subroutine pspio_f90_projector_get_l(projector, l)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer(pspio_cint),         intent(out) :: l
    end subroutine pspio_f90_projector_get_l
  end interface

  interface ! 
    subroutine pspio_f90_projector_get_j(projector, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_cdouble),         intent(out) :: j
    end subroutine pspio_f90_projector_get_j
  end interface

  ! pspio_xc
  interface !
    subroutine pspio_f90_xc_get_correlation(xc, correlation)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(out) :: correlation
    end subroutine pspio_f90_xc_get_correlation
  end interface

  interface !
    subroutine pspio_f90_xc_get_exchange(xc, exchange)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(out) :: exchange
    end subroutine pspio_f90_xc_get_exchange
  end interface

  interface !
    subroutine pspio_f90_xc_has_nlcc_int(xc, has_nlcc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(out) :: has_nlcc
    end subroutine pspio_f90_xc_has_nlcc_int
  end interface

  interface pspio_f90_xc_nlcc_density_eval
    subroutine pspio_f90_xc_nlcc_density_eval_s(xc, r, nlcc_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      real(pspio_cdouble),  intent(in)  :: r
      real(pspio_cdouble),  intent(out) :: nlcc_dens
    end subroutine pspio_f90_xc_nlcc_density_eval_s

    subroutine pspio_f90_xc_nlcc_density_eval_v(xc, np, r, nlcc_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(in)  :: np
      real(pspio_cdouble),  intent(in)  :: r(np)
      real(pspio_cdouble),  intent(out) :: nlcc_dens(np)
    end subroutine pspio_f90_xc_nlcc_density_eval_v
  end interface

  ! pspio_error
  interface
    integer(pspio_cint) function pspio_f90_error_fetchall(err_msg)
      use pspio_f90_types_m, only:PSPIO_STRLEN_ERROR, pspio_cint
      implicit none
      character(len=PSPIO_STRLEN_ERROR), intent(out) :: err_msg
    end function pspio_f90_error_fetchall

    integer(pspio_cint) function pspio_f90_error_flush()
      use pspio_f90_types_m
      implicit none
    end function pspio_f90_error_flush

    integer(pspio_cint) function pspio_f90_error_free()
      use pspio_f90_types_m
      implicit none
    end function pspio_f90_error_free
  end interface

  ! pspio_info
  interface
    integer(pspio_cint) function pspio_f90_version(major, minor, micro)
      use pspio_f90_types_m
      implicit none
      integer(pspio_cint), intent(out) :: major, minor, micro
    end function pspio_f90_version
  end interface

contains

  logical function pspio_f90_xc_has_nlcc(xc)
    use pspio_f90_types_m
    implicit none
    type(pspio_f90_xc_t), intent(in)  :: xc
    
    integer :: has_nlcc

    call pspio_f90_xc_has_nlcc_int(xc, has_nlcc)
    pspio_f90_xc_has_nlcc = has_nlcc /= 0

  end function pspio_f90_xc_has_nlcc


end module pspio_f90_lib_m
