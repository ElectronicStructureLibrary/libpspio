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
  interface
    integer(pspio_cint) function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer(pspio_cint) function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer(pspio_cint),                   intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer(pspio_cint) function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      integer(pspio_cint),                   intent(in) :: format
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

    real(pspio_cdouble) function pspio_f90_pspdata_get_z(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_z

    real(pspio_cdouble) function pspio_f90_pspdata_get_zvalence(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_zvalence

    integer(pspio_cint) function pspio_f90_pspdata_get_l_max(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_l_max

    integer(pspio_cint) function pspio_f90_pspdata_get_wave_eq(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_wave_eq

    subroutine pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end subroutine pspio_f90_pspdata_get_mesh

    integer(pspio_cint) function pspio_f90_pspdata_get_n_states(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_n_states

    subroutine pspio_f90_pspdata_get_state(pspdata, i, state)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),                   intent(in)  :: i
      type(pspio_f90_state_t),   intent(out) :: state
    end subroutine pspio_f90_pspdata_get_state

    integer(pspio_cint) function pspio_f90_pspdata_get_n_potentials(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_n_potentials

    subroutine pspio_f90_pspdata_get_potential(pspdata, i, potential)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer(pspio_cint),                     intent(in)  :: i
      type(pspio_f90_potential_t), intent(out) :: potential
    end subroutine pspio_f90_pspdata_get_potential

    integer(pspio_cint) function pspio_f90_pspdata_get_n_kbproj(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_n_kbproj

    subroutine pspio_f90_pspdata_get_kb_projector(pspdata, i, projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer(pspio_cint),                     intent(in)  :: i
      type(pspio_f90_projector_t), intent(out) :: projector
    end subroutine pspio_f90_pspdata_get_kb_projector

    integer(pspio_cint) function pspio_f90_pspdata_get_l_local(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_l_local

    integer(pspio_cint) function pspio_f90_pspdata_get_kb_l_max(pspdata)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
    end function pspio_f90_pspdata_get_kb_l_max

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

  interface pspio_f90_pspdata_rho_valence_eval
    subroutine pspio_f90_pspdata_rho_valence_eval_s(pspdata, r, rho)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_cdouble),      intent(in)  :: r
      real(pspio_cdouble),      intent(out) :: rho
    end subroutine pspio_f90_pspdata_rho_valence_eval_s

    subroutine pspio_f90_pspdata_rho_valence_eval_v(pspdata, np, r, rho)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer(pspio_cint),                   intent(in)  :: np
      real(pspio_cdouble),      intent(in)  :: r(np)
      real(pspio_cdouble),      intent(out) :: rho(np)
    end subroutine pspio_f90_pspdata_rho_valence_eval_v
  end interface

  ! pspio_mesh
  interface
    subroutine pspio_f90_mesh_get_info(mesh, mesh_type, np, a, b)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer(pspio_cint),                intent(out) :: mesh_type
      integer(pspio_cint),                intent(out) :: np
      real(pspio_cdouble),   intent(out) :: a
      real(pspio_cdouble),   intent(out) :: b
    end subroutine pspio_f90_mesh_get_info

    integer(pspio_cint) function pspio_f90_mesh_get_np(mesh)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)  :: mesh
    end function pspio_f90_mesh_get_np

    subroutine pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble),   intent(out) :: r(*)
    end subroutine pspio_f90_mesh_get_r

    subroutine pspio_f90_mesh_get_rab(mesh, rab)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_cdouble), dimension(*), intent(out) :: rab
    end subroutine pspio_f90_mesh_get_rab
  end interface

  ! pspio_state
  interface pspio_f90_state_wf_eval
    subroutine pspio_f90_state_wf_eval_s(state, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_cdouble),    intent(in)  :: r
      real(pspio_cdouble),    intent(out) :: wf
    end subroutine pspio_f90_state_wf_eval_s

    subroutine pspio_f90_state_wf_eval_v(state, np, r, wf)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer(pspio_cint),                 intent(in)  :: np
      real(pspio_cdouble),    intent(in)  :: r(np)
      real(pspio_cdouble),    intent(out) :: wf(np)
    end subroutine pspio_f90_state_wf_eval_v
  end interface

  interface
    subroutine pspio_f90_state_get_qn(state, n, l, j)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
      integer(pspio_cint),                 intent(out) :: n
      integer(pspio_cint),                 intent(out) :: l
      real(pspio_cdouble),    intent(out) :: j
    end subroutine pspio_f90_state_get_qn

    real(pspio_cdouble) function pspio_f90_state_get_occ(state)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_state_t), intent(in)  :: state
    end function pspio_f90_state_get_occ
  end interface

  ! pspio_potential
  interface pspio_f90_potential_eval
    subroutine pspio_f90_potential_eval_s(potential, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      real(pspio_cdouble),        intent(in)  :: r
      real(pspio_cdouble),        intent(out) :: v
    end subroutine pspio_f90_potential_eval_s

    subroutine pspio_f90_potential_eval_v(potential, np, r, v)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_potential_t), intent(in)  :: potential
      integer(pspio_cint),                     intent(in)  :: np
      real(pspio_cdouble),        intent(in)  :: r(np)
      real(pspio_cdouble),        intent(out) :: v(np)
    end subroutine pspio_f90_potential_eval_v
  end interface pspio_f90_potential_eval

  ! pspio_projector
  interface pspio_f90_projector_eval
    subroutine pspio_f90_projector_eval_s(projector, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_cdouble),        intent(in)  :: r
      real(pspio_cdouble),        intent(out) :: p
    end subroutine pspio_f90_projector_eval_s

    subroutine pspio_f90_projector_eval_v(projector, np, r, p)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer(pspio_cint),                     intent(in)  :: np
      real(pspio_cdouble),        intent(in)  :: r(np)
      real(pspio_cdouble),        intent(out) :: p(np)
    end subroutine pspio_f90_projector_eval_v
  end interface

  interface
    real(pspio_cdouble) function pspio_f90_projector_get_energy(projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
    end function pspio_f90_projector_get_energy

    integer(pspio_cint) function pspio_f90_projector_get_l(projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
    end function pspio_f90_projector_get_l

    real(pspio_cdouble) function pspio_f90_projector_get_j(projector)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_projector_t), intent(in)  :: projector
    end function pspio_f90_projector_get_j
  end interface

  ! pspio_xc
  interface
    integer(pspio_cint) function pspio_f90_xc_get_correlation(xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
    end function pspio_f90_xc_get_correlation
  end interface

  interface
    integer(pspio_cint) function pspio_f90_xc_get_exchange(xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
    end function pspio_f90_xc_get_exchange
  end interface

  interface
    integer(pspio_cint) function pspio_f90_xc_has_nlcc_int(xc)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
    end function pspio_f90_xc_has_nlcc_int
  end interface

  interface pspio_f90_xc_core_density_eval
    subroutine pspio_f90_xc_core_density_eval_s(xc, r, core_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      real(pspio_cdouble),  intent(in)  :: r
      real(pspio_cdouble),  intent(out) :: core_dens
    end subroutine pspio_f90_xc_core_density_eval_s

    subroutine pspio_f90_xc_core_density_eval_v(xc, np, r, core_dens)
      use pspio_f90_types_m
      implicit none
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer(pspio_cint),  intent(in)  :: np
      real(pspio_cdouble),  intent(in)  :: r(np)
      real(pspio_cdouble),  intent(out) :: core_dens(np)
    end subroutine pspio_f90_xc_core_density_eval_v
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
    
    pspio_f90_xc_has_nlcc = pspio_f90_xc_has_nlcc_int(xc) /= 0

  end function pspio_f90_xc_has_nlcc


end module pspio_f90_lib_m
