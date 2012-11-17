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
!! $Id$


module pspio_f90_types_m
  implicit none

  integer, public, parameter :: pspio_f90_kind = selected_real_kind(14)

  ! All the following types are just pointers to the actual data structures
  type pspio_f90_pspdata_t
    private
    integer, pointer :: buffer
  end type pspio_f90_pspdata_t

  type pspio_f90_mesh_t
    private
    integer, pointer :: buffer
  end type pspio_f90_mesh_t

  type pspio_f90_state_t
    private
    integer, pointer :: buffer
  end type pspio_f90_state_t

  type pspio_f90_projector_t
    private
    integer, pointer :: buffer
  end type pspio_f90_projector_t

  type pspio_f90_potential_t
    private
    integer, pointer :: buffer
  end type pspio_f90_potential_t

  type pspio_f90_xc_t
    private
    integer, pointer :: buffer
  end type pspio_f90_xc_t

end module pspio_f90_types_m


module pspio_f90_lib_m

  use pspio_f90_types_m

  implicit none

  integer, parameter ::       &
       PSPIO_SCHRODINGER = 1, &
       PSPIO_SCALAR_REL  = 2, &
       PSPIO_DIRAC       = 3

  integer, parameter ::     &
       PSPIO_UNKNOWN = -1,  &
       PSPIO_UPF     = 9

  integer, parameter ::        &
       PSPIO_SUCCESS      = 0, &
       PSPIO_ERROR        = -1, &
       PSPIO_ENOFILE      =  1, &
       PSPIO_EIO          =  2, &
       PSPIO_EVALUE       =  3, &
       PSPIO_EFILE_FORMAT =  4, &
       PSPIO_ETYPE        =  5, &
       PSPIO_EGSL         =  6, &
       PSPIO_ENOMEM       =  7, &
       PSPIO_ENOSUPPORT   =  8


  ! pspio_pspdata
  interface
    integer function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer,                   intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      integer,                   intent(in) :: format
      character(len=*),          intent(in) :: filename
    end function pspio_f90_pspdata_write

    subroutine pspio_f90_pspdata_free(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end subroutine pspio_f90_pspdata_free

    subroutine pspio_f90_pspdata_get_symbol(pspdata, symbol)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      character(len=*),          intent(out) :: symbol
    end subroutine pspio_f90_pspdata_get_symbol

    subroutine pspio_f90_pspdata_get_z(pspdata, z)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: z
    end subroutine pspio_f90_pspdata_get_z

    subroutine pspio_f90_pspdata_get_zvalence(pspdata, zvalence)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: zvalence
    end subroutine pspio_f90_pspdata_get_zvalence

    subroutine pspio_f90_pspdata_get_l_max(pspdata, l_max)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_max
    end subroutine pspio_f90_pspdata_get_l_max

    subroutine pspio_f90_pspdata_get_wave_eq(pspdata, wave_eq)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: wave_eq
    end subroutine pspio_f90_pspdata_get_wave_eq

    subroutine pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end subroutine pspio_f90_pspdata_get_mesh

    subroutine pspio_f90_pspdata_get_n_states(pspdata, n_states)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_states
    end subroutine pspio_f90_pspdata_get_n_states

    subroutine pspio_f90_pspdata_get_state(pspdata, i, state)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(in)  :: i
      type(pspio_f90_state_t),   intent(out) :: state
    end subroutine pspio_f90_pspdata_get_state

    subroutine pspio_f90_pspdata_get_n_kbproj(pspdata, n_kbproj)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_kbproj
    end subroutine pspio_f90_pspdata_get_n_kbproj

    subroutine pspio_f90_pspdata_get_kb_projector(pspdata, i, projector)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer,                     intent(in)  :: i
      type(pspio_f90_projector_t), intent(out) :: projector
    end subroutine pspio_f90_pspdata_get_kb_projector

    subroutine pspio_f90_pspdata_get_l_local(pspdata, l_local)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_local
    end subroutine pspio_f90_pspdata_get_l_local

    subroutine pspio_f90_pspdata_get_kb_l_max(pspdata, kb_l_max)
      use pspio_f90_types_m
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
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_xc_t),      intent(out) :: xc
    end subroutine pspio_f90_pspdata_get_xc
  end interface

  ! pspio_mesh
  interface
    subroutine pspio_f90_mesh_get_np(mesh, np)
      use pspio_f90_types_m
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer,                intent(out) :: np
    end subroutine pspio_f90_mesh_get_np

    subroutine pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_f90_kind),   intent(inout) :: r
    end subroutine pspio_f90_mesh_get_r
  end interface

  ! pspio_state
  interface pspio_f90_state_wf_eval
    subroutine pspio_f90_state_wf_eval_s(state, r, wf)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(in)  :: r
      real(pspio_f90_kind),    intent(out) :: wf
    end subroutine pspio_f90_state_wf_eval_s

    subroutine pspio_f90_state_wf_eval_v(state, np, r, wf)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      integer,                 intent(in)  :: np
      real(pspio_f90_kind),    intent(in)  :: r(np)
      real(pspio_f90_kind),    intent(out) :: wf(np)
    end subroutine pspio_f90_state_wf_eval_v
  end interface

  interface
    subroutine pspio_f90_state_get_qn(state, n, l, j)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      integer,                 intent(out) :: n
      integer,                 intent(out) :: l
      real(pspio_f90_kind),    intent(out) :: j
    end subroutine pspio_f90_state_get_qn

    subroutine pspio_f90_state_get_occ(state, occ)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(out) :: occ
    end subroutine pspio_f90_state_get_occ
  end interface

  ! pspio_potential
  interface pspio_f90_potential_eval
    subroutine pspio_f90_potential_eval_s(potential, r, v)
      use pspio_f90_types_m
      type(pspio_f90_potential_t), intent(in)  :: potential
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: v
    end subroutine pspio_f90_potential_eval_s

    subroutine pspio_f90_potential_eval_v(potential, np, r, v)
      use pspio_f90_types_m
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
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: p
    end subroutine pspio_f90_projector_eval_s

    subroutine pspio_f90_projector_eval_v(projector, np, r, p)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer,                     intent(in)  :: np
      real(pspio_f90_kind),        intent(in)  :: r(np)
      real(pspio_f90_kind),        intent(out) :: p(np)
    end subroutine pspio_f90_projector_eval_v
  end interface

  interface
    subroutine pspio_f90_projector_get_energy(projector, e)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: e
    end subroutine pspio_f90_projector_get_energy

    subroutine pspio_f90_projector_get_l(projector, l)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer,                     intent(out) :: l
    end subroutine pspio_f90_projector_get_l

    subroutine pspio_f90_projector_get_j(projector, j)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: j
    end subroutine pspio_f90_projector_get_j
  end interface

  ! pspio_xc
  interface
    subroutine pspio_f90_xc_has_nlcc_int(xc, has_nlcc)
      use pspio_f90_types_m
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(out) :: has_nlcc
    end subroutine pspio_f90_xc_has_nlcc_int
  end interface

  interface pspio_f90_xc_nlcc_eval
    subroutine pspio_f90_xc_nlcc_eval_s(xc, r, core_dens)
      use pspio_f90_types_m
      type(pspio_f90_xc_t), intent(in)  :: xc
      real(pspio_f90_kind), intent(in)  :: r
      real(pspio_f90_kind), intent(out) :: core_dens
    end subroutine pspio_f90_xc_nlcc_eval_s

    subroutine pspio_f90_xc_nlcc_eval_v(xc, np, r, core_dens)
      use pspio_f90_types_m
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(in)  :: np
      real(pspio_f90_kind), intent(in)  :: r(np)
      real(pspio_f90_kind), intent(out) :: core_dens(np)
    end subroutine pspio_f90_xc_nlcc_eval_v
  end interface

  ! pspio_error
  interface
    integer function pspio_f90_error_add(lineno, filename)
      integer, intent(in) :: lineno
      character(len=*), intent(in) :: filename
    end function pspio_f90_error_add

    integer function pspio_f90_error_fetchall(err_msg)
      character(len=:), pointer, intent(out) :: err_msg
    end function pspio_f90_error_fetchall

    integer function pspio_f90_error_flush()
    end function pspio_f90_error_flush

    integer function pspio_f90_error_free()
    end function pspio_f90_error_free

    integer function pspio_f90_error_len()
    end function pspio_f90_error_len

    integer function pspio_f90_error_get()
    end function pspio_f90_error_get

    subroutine pspio_f90_error_set(eid)
      integer, intent(in) :: eid
    end subroutine pspio_f90_error_set
  end interface

  ! pspio_info
  interface
    integer function pspio_f90_version(major, minor, micro)
      integer, intent(out) :: major, minor, micro
    end function pspio_f90_version
  end interface

contains

  subroutine pspio_f90_xc_has_nlcc(xc, has_nlcc)
    use pspio_f90_types_m
    type(pspio_f90_xc_t), intent(in)  :: xc
    logical,              intent(out) :: has_nlcc
    
    integer :: i

    call pspio_f90_xc_has_nlcc_int(xc, i)
    has_nlcc = i /= 0

  end subroutine pspio_f90_xc_has_nlcc


end module pspio_f90_lib_m
