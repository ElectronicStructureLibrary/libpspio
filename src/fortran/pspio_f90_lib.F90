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


  !----------------------------------------------------------------------------!
  ! pspio_pspdata                                                              !
  !----------------------------------------------------------------------------!

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

    subroutine pspio_f90_pspdata_get_format_guessed(pspdata, format)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: format
    end subroutine pspio_f90_pspdata_get_format_guessed
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
    integer(pspio_cint) function pspio_f90_pspdata_set_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      type(pspio_f90_mesh_t),    intent(in)    :: mesh
    end function pspio_f90_pspdata_set_mesh

    integer(pspio_cint) function  pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end function pspio_f90_pspdata_get_mesh
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
    integer(pspio_cint) function pspio_f90_pspdata_set_states(pspdata, states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      type(pspio_f90_state_t),   intent(in) :: states(*)
    end function pspio_f90_pspdata_set_states

    integer(pspio_cint) function  pspio_f90_pspdata_get_states(pspdata, states)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_state_t),   intent(out) :: states(*)
    end function pspio_f90_pspdata_get_states

    integer(pspio_cint) function  pspio_f90_pspdata_get_state(pspdata, i, state)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(in)  :: i
      type(pspio_f90_state_t),   intent(out) :: state
    end function pspio_f90_pspdata_get_state
  end interface

  interface ! scheme
    subroutine pspio_f90_pspdata_set_scheme(pspdata, scheme)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: scheme
    end subroutine pspio_f90_pspdata_set_scheme

    subroutine pspio_f90_pspdata_get_scheme(pspdata, scheme)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: scheme
    end subroutine pspio_f90_pspdata_get_scheme
  end interface

  interface ! n_potentials
    subroutine pspio_f90_pspdata_set_n_potentials(pspdata, n_potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),       intent(in)    :: n_potentials
    end subroutine pspio_f90_pspdata_set_n_potentials

    subroutine pspio_f90_pspdata_get_n_potentials(pspdata, n_potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),       intent(out) :: n_potentials
    end subroutine pspio_f90_pspdata_get_n_potentials
  end interface

  interface ! potentials
    integer(pspio_cint) function pspio_f90_pspdata_set_potentials(pspdata, potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_potential_t), intent(in)    :: potentials(*)
    end function pspio_f90_pspdata_set_potentials

    integer(pspio_cint) function pspio_f90_pspdata_get_potentials(pspdata, potentials)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: potentials(*)
    end function pspio_f90_pspdata_get_potentials

    integer(pspio_cint) function pspio_f90_pspdata_get_potential(pspdata, i, potential)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer(pspio_cint),         intent(in)  :: i
      type(pspio_f90_potential_t), intent(out) :: potential
    end function pspio_f90_pspdata_get_potential
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
    integer(pspio_cint) function pspio_f90_pspdata_set_kb_projectors(pspdata, kb_projectors)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_projector_t), intent(in)    :: kb_projectors(*)
    end function pspio_f90_pspdata_set_kb_projectors

    integer(pspio_cint) function pspio_f90_pspdata_get_kb_projectors(pspdata, kb_projectors)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_projector_t), intent(out) :: kb_projectors(*)
    end function pspio_f90_pspdata_get_kb_projectors

    integer(pspio_cint) function pspio_f90_pspdata_get_kb_projector(pspdata, i, kb_projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer(pspio_cint),         intent(in) :: i
      type(pspio_f90_projector_t), intent(out) :: kb_projector
    end function pspio_f90_pspdata_get_kb_projector
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
    integer(pspio_cint) function pspio_f90_pspdata_set_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(inout) :: pspdata
      type(pspio_f90_potential_t), intent(in)    :: vlocal
    end function pspio_f90_pspdata_set_vlocal

    integer(pspio_cint) function pspio_f90_pspdata_get_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: vlocal
    end function pspio_f90_pspdata_get_vlocal
  end interface

  interface ! xc
    integer(pspio_cint) function pspio_f90_pspdata_set_xc(pspdata, xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      type(pspio_f90_xc_t),      intent(in)    :: xc
    end function pspio_f90_pspdata_set_xc

    integer(pspio_cint) function pspio_f90_pspdata_get_xc(pspdata, xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_xc_t),      intent(out) :: xc
    end function pspio_f90_pspdata_get_xc
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


  !----------------------------------------------------------------------------!
  ! pspio_mesh                                                                 !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_mesh_alloc(mesh, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(inout) :: mesh
      integer(pspio_cint),    intent(in)    :: np
    end function pspio_f90_mesh_alloc
  end interface

  interface ! set
    integer(pspio_cint) function pspio_f90_mesh_init(mesh, type, a, b, r, rab)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(inout) :: mesh
      integer(pspio_cint),    intent(in)    :: type
      real(pspio_cdouble),    intent(in)    :: a
      real(pspio_cdouble),    intent(in)    :: b
      real(pspio_cdouble),    intent(in)    :: r(*)
      real(pspio_cdouble),    intent(in)    :: rab(*)
    end function pspio_f90_mesh_init
  end interface

  interface ! init_from_points
    subroutine pspio_f90_mesh_init_from_points(mesh, r, rab)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(inout) :: mesh
      real(pspio_cdouble),    intent(in)    :: r(*)
      real(pspio_cdouble),    intent(in)    :: rab(*)
    end subroutine pspio_f90_mesh_init_from_points
  end interface

  interface ! init_from_parameters
    subroutine pspio_f90_mesh_init_from_parameters(mesh, type, a, b)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(inout) :: mesh
      integer(pspio_cint),    intent(in)    :: type
      real(pspio_cdouble),    intent(in)    :: a
      real(pspio_cdouble),    intent(in)    :: b
    end subroutine pspio_f90_mesh_init_from_parameters
  end interface

  interface ! free
    subroutine pspio_f90_mesh_free(mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(inout) :: mesh
    end subroutine pspio_f90_mesh_free
  end interface

  interface ! get_info
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

  interface ! Np
    subroutine pspio_f90_mesh_get_np(mesh, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer(pspio_cint),    intent(out) :: np
    end subroutine pspio_f90_mesh_get_np
  end interface

  interface ! R
    subroutine pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      real(pspio_cdouble),    intent(out) :: r(*)
    end subroutine pspio_f90_mesh_get_r
  end interface

  interface ! rab
    subroutine pspio_f90_mesh_get_rab(mesh, rab)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      real(pspio_cdouble),    intent(out) :: rab(*)
    end subroutine pspio_f90_mesh_get_rab
  end interface


  !----------------------------------------------------------------------------!
  ! pspio_state                                                                !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_state_alloc(state, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(inout) :: state
      integer(pspio_cint),     intent(in)    :: np
    end function pspio_f90_state_alloc
  end interface

  interface ! set
    integer(pspio_cint) function pspio_f90_state_init(state, eigenval, qn, occ, rc, mesh, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(inout) :: state
      real(pspio_cdouble),     intent(in)    :: eigenval
      type(pspio_f90_qn_t),    intent(in)    :: qn
      real(pspio_cdouble),     intent(in)    :: occ
      real(pspio_cdouble),     intent(in)    :: rc
      type(pspio_f90_mesh_t),  intent(in)    :: mesh
      real(pspio_cdouble),     intent(in)    :: wf(*)
    end function pspio_f90_state_init
  end interface

  interface ! free
    subroutine pspio_f90_state_free(state)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(inout) :: state
    end subroutine pspio_f90_state_free
  end interface

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

  interface ! 
    subroutine pspio_f90_state_get_ev(state, ev)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_cdouble),     intent(out) :: ev
    end subroutine pspio_f90_state_get_ev
  end interface

  interface ! 
    subroutine pspio_f90_state_get_rc(state, rc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_cdouble),     intent(out) :: rc
    end subroutine pspio_f90_state_get_rc
  end interface


  !----------------------------------------------------------------------------!
  ! pspio_potential                                                            !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_potential_alloc(potential, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(inout) :: potential
      integer(pspio_cint),         intent(in)    :: np
    end function pspio_f90_potential_alloc
  end interface

  interface ! set
    integer(pspio_cint) function pspio_f90_potential_init(potential, qn, mesh, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(inout) :: potential
      type(pspio_f90_qn_t),        intent(in)    :: qn
      type(pspio_f90_mesh_t),      intent(in)    :: mesh
      real(pspio_cdouble),         intent(in)    :: v(*)
    end function pspio_f90_potential_init
  end interface

  interface ! free
    subroutine pspio_f90_potential_free(potential)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(inout) :: potential
    end subroutine pspio_f90_potential_free
  end interface

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


  !----------------------------------------------------------------------------!
  ! pspio_projector                                                            !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_projector_alloc(projector, np)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(inout) :: projector
      integer(pspio_cint),         intent(in)    :: np
    end function pspio_f90_projector_alloc
  end interface

  interface ! set
    integer(pspio_cint) function pspio_f90_projector_init(projector, qn, energy, mesh, proj)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(inout) :: projector
      type(pspio_f90_qn_t),        intent(in)    :: qn
      real(pspio_cdouble),         intent(in)    :: energy
      type(pspio_f90_mesh_t),      intent(in)    :: mesh
      real(pspio_cdouble),         intent(in)    :: proj(*)
    end function pspio_f90_projector_init
  end interface

  interface ! free
    subroutine pspio_f90_projector_free(projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(inout) :: projector
    end subroutine pspio_f90_projector_free
  end interface

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


  !----------------------------------------------------------------------------!
  ! pspio_xc                                                                   !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_xc_alloc(xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(inout) :: xc
    end function pspio_f90_xc_alloc
  end interface

  interface ! free
    subroutine pspio_f90_xc_free(xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(inout) :: xc
    end subroutine pspio_f90_xc_free
  end interface

  interface ! set_id
    subroutine pspio_f90_xc_set_id(xc, exchange, correlation)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(inout) :: xc
      integer(pspio_cint),  intent(in)    :: exchange
      integer(pspio_cint),  intent(in)    :: correlation
    end subroutine pspio_f90_xc_set_id
  end interface

  interface
    integer(pspio_cint) function pspio_f90_xc_set_nlcc_scheme(xc, nlcc_scheme)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(inout) :: xc
      integer(pspio_cint),  intent(in)    :: nlcc_scheme
    end function pspio_f90_xc_set_nlcc_scheme
  end interface

  interface pspio_f90_xc_set_nlcc_density
    integer(pspio_cint) function pspio_f90_xc_set_nlcc_density1(xc, mesh, cd)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t),   intent(inout) :: xc
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),    intent(in)    :: cd(*)
    end function pspio_f90_xc_set_nlcc_density1

    integer(pspio_cint) function pspio_f90_xc_set_nlcc_density2(xc, mesh, cd, cdp)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t),   intent(inout) :: xc
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),    intent(in)    :: cd(*)
      real(pspio_cdouble),    intent(in)    :: cdp(*)
    end function pspio_f90_xc_set_nlcc_density2

    integer(pspio_cint) function pspio_f90_xc_set_nlcc_density3(xc, mesh, cd, cdp, cdpp)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t),   intent(inout) :: xc
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),    intent(in)    :: cd(*)
      real(pspio_cdouble),    intent(in)    :: cdp(*)
      real(pspio_cdouble),    intent(in)    :: cdpp(*)
    end function pspio_f90_xc_set_nlcc_density3
  end interface

  interface ! get_correlation
    subroutine pspio_f90_xc_get_correlation(xc, correlation)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(out) :: correlation
    end subroutine pspio_f90_xc_get_correlation
  end interface

  interface ! get_exchange
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


  !----------------------------------------------------------------------------!
  ! pspio_qn                                                                   !
  !----------------------------------------------------------------------------!

  interface ! alloc
    integer(pspio_cint) function pspio_f90_qn_alloc(qn)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(inout) :: qn
    end function pspio_f90_qn_alloc
  end interface

  interface ! init
    integer(pspio_cint) function pspio_f90_qn_init(qn, n, l, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(inout) :: qn
      integer(pspio_cint),  intent(in)    :: n
      integer(pspio_cint),  intent(in)    :: l
      real(pspio_cdouble),  intent(in)    :: j
    end function pspio_f90_qn_init
  end interface

  interface ! free
    subroutine pspio_f90_qn_free(qn)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(inout) :: qn
    end subroutine pspio_f90_qn_free
  end interface

  interface ! get_n
    subroutine pspio_f90_qn_get_n(qn, n)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(in)  :: qn
      real(pspio_cint),     intent(out) :: n
    end subroutine pspio_f90_qn_get_n
  end interface

  interface ! get_l
    subroutine pspio_f90_qn_get_l(qn, l)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(in)  :: qn
      real(pspio_cint),     intent(out) :: l
    end subroutine pspio_f90_qn_get_l
  end interface

  interface ! get_j
    subroutine pspio_f90_qn_get_j(qn, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_qn_t), intent(in)  :: qn
      real(pspio_cdouble),  intent(out) :: j
    end subroutine pspio_f90_qn_get_j
  end interface


  !----------------------------------------------------------------------------!
  ! pspio_error                                                                !
  !----------------------------------------------------------------------------!

  interface
    subroutine pspio_f90_error_fetchall(err_msg)
      use pspio_f90_types_m, only:PSPIO_STRLEN_ERROR, pspio_cint
      implicit none
      character(len=PSPIO_STRLEN_ERROR), intent(out) :: err_msg
    end subroutine pspio_f90_error_fetchall

    subroutine pspio_f90_error_flush()
      use pspio_f90_types_m
      implicit none
    end subroutine pspio_f90_error_flush

    subroutine pspio_f90_error_free()
      use pspio_f90_types_m
      implicit none
    end subroutine pspio_f90_error_free

    integer(pspio_cint) function pspio_f90_error_get_last(routine)
      use pspio_f90_types_m
      implicit none
      character(len=*), intent(in) :: routine
    end function pspio_f90_error_get_last

    subroutine pspio_f90_error_str(error_id, error_msg)
      use pspio_f90_types_m
      implicit none
      integer, intent(in) :: error_id
      character(len=PSPIO_STRLEN_ERROR), intent(out) :: error_msg
    end subroutine pspio_f90_error_str
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
