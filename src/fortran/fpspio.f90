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

  public :: fpspio_pspdata_t, &
            fpspio_mesh_t, &
            fpspio_state_t, &
            fpspio_potential_t, &
            fpspio_projector_t, &
            fpspio_xc_t, &
            fpspio_qn_t

  public :: fpspio_pspdata_alloc, &
            fpspio_pspdata_read, &
            fpspio_pspdata_write, &
            fpspio_pspdata_free, &
            fpspio_pspdata_get_format_guessed, &
            fpspio_pspdata_set_symbol, &
            fpspio_pspdata_get_symbol, &
            fpspio_pspdata_set_z, &
            fpspio_pspdata_get_z, &
            fpspio_pspdata_set_zvalence, &
            fpspio_pspdata_get_zvalence, &
            fpspio_pspdata_set_l_max, &
            fpspio_pspdata_get_l_max, &
            fpspio_pspdata_set_wave_eq, &
            fpspio_pspdata_get_wave_eq, &
            fpspio_pspdata_set_mesh, &
            fpspio_pspdata_get_mesh

  public :: fpspio_mesh_alloc, &
            fpspio_mesh_init, &
            fpspio_mesh_init_from_points, &
            fpspio_mesh_init_from_parameters, &
            fpspio_mesh_free, &
            fpspio_mesh_get_np, &
            fpspio_mesh_get_r, &
            fpspio_mesh_get_rab


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

  type fpspio_pspdata_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_pspdata_t

  type fpspio_mesh_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_mesh_t

  type fpspio_state_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_state_t

  type fpspio_potential_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_potential_t

  type fpspio_projector_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_projector_t

  type fpspio_xc_t
    private
    type(c_ptr) :: ptr = C_NULL_PTR
  end type fpspio_xc_t

  type, bind(c) :: fpspio_qn_t
    integer(c_int) :: n
    integer(c_int) :: l
    real(c_double) :: j
  end type fpspio_qn_t

  interface

    !----------------------------------------------------------------------------!
    ! pspio_pspdata                                                              !
    !----------------------------------------------------------------------------!

    ! init
    integer(c_int) function pspio_pspdata_alloc(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata      
    end function pspio_pspdata_alloc

    ! free
    subroutine pspio_pspdata_free(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end subroutine pspio_pspdata_free

    ! read
    integer(c_int) function pspio_pspdata_read(pspdata, format, filename) bind(c)
      import
      type(c_ptr)                   :: pspdata
      integer(c_int),         value :: format
      character(kind=c_char)        :: filename(*)
    end function pspio_pspdata_read

    ! write
    integer(c_int) function pspio_pspdata_write(pspdata, format, filename) bind(c)
      import
      type(c_ptr)                   :: pspdata
      integer(c_int),         value :: format
      character(kind=c_char)        :: filename(*)
    end function pspio_pspdata_write

    ! get_format_guessed
    integer(c_int) function pspio_pspdata_get_format_guessed(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_format_guessed

    ! set_symbol
    integer(c_int) function pspio_pspdata_set_symbol(pspdata, symbol) bind(c)
      import
      type(c_ptr)            :: pspdata
      character(kind=c_char) :: symbol(*)
    end function pspio_pspdata_set_symbol

    ! get_symbol
    character(kind=c_char) function pspio_pspdata_get_symbol(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_symbol

    ! set_z
    integer(c_int) function pspio_pspdata_set_z(pspdata, z) bind(c)
      import
      type(c_ptr)           :: pspdata
      real(c_double), value :: z
    end function pspio_pspdata_set_z

    ! get_z
    real(c_double) function pspio_pspdata_get_z(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_z

    ! set_zvalence
    integer(c_int) function pspio_pspdata_set_zvalence(pspdata, zvalence) bind(c)
      import
      type(c_ptr)           :: pspdata
      real(c_double), value :: zvalence
    end function pspio_pspdata_set_zvalence

    ! get_zvalence
    real(c_double) function pspio_pspdata_get_zvalence(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_zvalence

    ! set_l_max
    integer(c_int) function pspio_pspdata_set_l_max(pspdata, l_max) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: l_max
    end function pspio_pspdata_set_l_max

    ! get_l_max
    integer(c_int) function pspio_pspdata_get_l_max(pspdata) bind(c)
      import
      type(c_ptr)           :: pspdata
    end function pspio_pspdata_get_l_max

    ! set_wave_eq
    integer(c_int) function pspio_pspdata_set_wave_eq(pspdata, wave_eq) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: wave_eq
    end function pspio_pspdata_set_wave_eq

    ! get_wave_eq
    integer(c_int) function pspio_pspdata_get_wave_eq(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_wave_eq

    ! set_mesh
    integer(c_int) function pspio_pspdata_set_mesh(pspdata, mesh) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: mesh
    end function pspio_pspdata_set_mesh

    ! get_mesh
    type(c_ptr) function pspio_pspdata_get_mesh(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_mesh

    ! set_n_states
    integer(c_int) function pspio_pspdata_set_n_states(pspdata, n_states) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: n_states
    end function pspio_pspdata_set_n_states

    ! get_n_states
    integer(c_int) function pspio_pspdata_get_n_states(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_n_states

    ! set_states
    integer(c_int) function pspio_pspdata_set_states(pspdata, states) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: states(*)
    end function pspio_pspdata_set_states

    ! get_states
    type(c_ptr) function pspio_pspdata_get_states(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_states

    ! get_state
    type(c_ptr) function pspio_pspdata_get_state(pspdata, index) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: index
    end function pspio_pspdata_get_state

    ! set_scheme
    integer(c_int) function pspio_pspdata_set_scheme(pspdata, scheme) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: scheme
    end function pspio_pspdata_set_scheme

    ! get_scheme
    integer(c_int) function pspio_pspdata_get_scheme(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata      
    end function pspio_pspdata_get_scheme

    ! set_n_potentials
    integer(c_int) function pspio_pspdata_set_n_potentials(pspdata, n_potentials) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: n_potentials
    end function pspio_pspdata_set_n_potentials

    ! get_n_potentials
    integer(c_int) function pspio_pspdata_get_n_potentials(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_n_potentials

    ! set_potentials
    integer(c_int) function pspio_pspdata_set_potentials(pspdata, potentials) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: potentials(*)
    end function pspio_pspdata_set_potentials

    ! get_potentials
    type(c_ptr) function pspio_pspdata_get_potentials(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_potentials

    ! get_potential
    type(c_ptr) function pspio_pspdata_get_potential(pspdata, index) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: index
    end function pspio_pspdata_get_potential

    ! set_n_projectors
    integer(c_int) function pspio_pspdata_set_n_projectors(pspdata, n_projectors) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: n_projectors
    end function pspio_pspdata_set_n_projectors

    ! get_n_projectors
    integer(c_int) function pspio_pspdata_get_n_projectors(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_n_projectors

    ! set_projectors
    integer(c_int) function pspio_pspdata_set_projectors(pspdata, projectors) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: projectors(*)
    end function pspio_pspdata_set_projectors

   ! get_projectors
    type(c_ptr) function pspio_pspdata_get_projectors(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_projectors

   ! get_projector
    type(c_ptr) function pspio_pspdata_get_projector(pspdata, index) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: index
    end function pspio_pspdata_get_projector

    ! set_projectors_l_max
    integer(c_int) function pspio_pspdata_set_projectors_l_max(pspdata, l_max) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: l_max
    end function pspio_pspdata_set_projectors_l_max

    ! get_projectors_l_max
    integer(c_int) function pspio_pspdata_get_projectors_l_max(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_projectors_l_max
    
    ! set_l_local
    integer(c_int) function pspio_pspdata_set_l_local(pspdata, l_local) bind(c)
      import
      type(c_ptr)           :: pspdata
      integer(c_int), value :: l_local
    end function pspio_pspdata_set_l_local

    ! get_l_local
    integer(c_int) function pspio_pspdata_get_l_local(pspdata) bind(c)
      import
      type(c_ptr)    :: pspdata
    end function pspio_pspdata_get_l_local
    
    ! set_vlocal
    integer(c_int) function pspio_pspdata_set_vlocal(pspdata, vlocal) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: vlocal
    end function pspio_pspdata_set_vlocal

    ! get_vlocal
    type(c_ptr) function pspio_pspdata_get_vlocal(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_vlocal

    ! set_xc
    integer(c_int) function pspio_pspdata_set_xc(pspdata, xc) bind(c)
      import
      type(c_ptr) :: pspdata
      type(c_ptr) :: xc
    end function pspio_pspdata_set_xc

    ! get_xc
    type(c_ptr) function pspio_pspdata_get_xc(pspdata) bind(c)
      import
      type(c_ptr) :: pspdata
    end function pspio_pspdata_get_xc

    ! set_rho_valence
    integer(c_int) function pspio_pspdata_set_rho_valence(pspdata, rho_valence) bind(c)
      import
      type(c_ptr)    :: pspdata
      real(c_double) :: rho_valence(*)
    end function pspio_pspdata_set_rho_valence

    ! rho_valence_eval
    real(c_double) function pspio_pspdata_rho_valence_eval(pspdata, r) bind(c)
      import
      type(c_ptr)           :: pspdata
      real(c_double), value :: r
    end function pspio_pspdata_rho_valence_eval


    !----------------------------------------------------------------------------!
    ! pspio_mesh                                                                 !
    !----------------------------------------------------------------------------!

    ! alloc
    integer(c_int) function pspio_mesh_alloc(mesh, np) bind(c)
      import
      type(c_ptr)           :: mesh
      integer(c_int), value :: np
    end function pspio_mesh_alloc

    ! init
    integer(c_int) function pspio_mesh_init(mesh, type, a, b, r, rab) bind(c)
      import
      type(c_ptr)           :: mesh
      integer(c_int), value :: type
      real(c_double), value :: a
      real(c_double), value :: b
      real(c_double)        :: r(*)
      real(c_double)        :: rab(*)
    end function pspio_mesh_init

    ! init_from_points
    integer(c_int) function pspio_mesh_init_from_points(mesh, r, rab) bind(c)
      import
      type(c_ptr)    :: mesh
      real(c_double) :: r(*)
      real(c_double) :: rab(*)
    end function pspio_mesh_init_from_points

    ! init_from_parameters
    integer(c_int) function pspio_mesh_init_from_parameters(mesh, type, a, b) bind(c)
      import
      type(c_ptr)           :: mesh
      integer(c_int), value :: type
      real(c_double), value :: a
      real(c_double), value :: b
    end function pspio_mesh_init_from_parameters

    ! free
    subroutine pspio_mesh_free(mesh) bind(c)
      import
      type(c_ptr) :: mesh
    end subroutine pspio_mesh_free

    ! np
    integer(c_int) function pspio_mesh_get_np(mesh) bind(c)
      import
      type(c_ptr) :: mesh
    end function pspio_mesh_get_np

    ! r
    type(c_ptr) function pspio_mesh_get_r(mesh) bind(c)
      import
      type(c_ptr) :: mesh      
    end function pspio_mesh_get_r

    ! rab
    type(c_ptr) function pspio_mesh_get_rab(mesh) bind(c)
      import
      type(c_ptr) :: mesh
    end function pspio_mesh_get_rab

    !----------------------------------------------------------------------------!
    ! pspio_state                                                                !
    !----------------------------------------------------------------------------!

    ! alloc
    integer(c_int) function pspio_state_alloc(state, np) bind(c)
      import
      type(c_ptr)           :: state
      integer(c_int), value :: np
    end function pspio_state_alloc

    ! init
    integer(c_int) function pspio_state_init(state, eigenval, qn, occ, rc, mesh, wf, label) bind(c)
      import
      type(c_ptr)                  :: state
      real(c_double),        value :: eigenval
      type(fpspio_qn_t)            :: qn
      real(c_double),        value :: occ
      real(c_double),        value :: rc
      type(c_ptr)                  :: mesh
      real(c_double)               :: wf(*)
      character(kind=c_char)       :: label(*)
    end function pspio_state_init

    ! free
    subroutine pspio_state_free(state) bind(c)
      import
      type(c_ptr) :: state
    end subroutine pspio_state_free

    ! wf_eval
    real(c_double) function pspio_state_wf_eval(state, r) bind(c)
      import
      type(c_ptr)           :: state
      real(c_double), value :: r
    end function pspio_state_wf_eval

    ! get_qn
    type(fpspio_qn_t) function pspio_state_get_qn(state) bind(c)
      import
      type(c_ptr) :: state
    end function pspio_state_get_qn

    ! get_occ
    real(c_double) function pspio_state_get_occ(state) bind(c)
      import
      type(c_ptr) :: state      
    end function pspio_state_get_occ

    ! get_ev
    real(c_double) function pspio_state_get_ev(state) bind(c)
      import
      type(c_ptr) :: state      
    end function pspio_state_get_ev

    ! get_rc
    real(c_double) function  pspio_state_get_rc(state) bind(c)
      import
      type(c_ptr) :: state
    end function pspio_state_get_rc

    !----------------------------------------------------------------------------!
    ! pspio_potential                                                            !
    !----------------------------------------------------------------------------!

    ! alloc
    integer(c_int) function pspio_potential_alloc(potential, np) bind(c)
      import
      type(c_ptr)           :: potential
      integer(c_int), value :: np
    end function pspio_potential_alloc

    ! init
    integer(c_int) function pspio_potential_init(potential, qn, mesh, v) bind(c)
      import
      type(c_ptr)       :: potential
      type(fpspio_qn_t) :: qn
      type(c_ptr)       :: mesh
      real(c_double)    :: v(*)
    end function pspio_potential_init

    ! free
    subroutine pspio_potential_free(potential) bind(c)
      import
      type(c_ptr), intent(inout) :: potential
    end subroutine pspio_potential_free

    ! eval
    real(c_double) function pspio_potential_eval(potential, r) bind(c)
      import
      type(c_ptr)           :: potential
      real(c_double), value :: r
    end function pspio_potential_eval

    !----------------------------------------------------------------------------!
    ! pspio_projector                                                            !
    !----------------------------------------------------------------------------!

    ! alloc
    integer(c_int) function pspio_projector_alloc(projector, np) bind(c)
      import
      type(c_ptr)           :: projector
      integer(c_int), value :: np
    end function pspio_projector_alloc

    ! set
    integer(c_int) function pspio_projector_init(projector, qn, energy, mesh, proj) bind(c)
      import
      type(c_ptr)             :: projector
      type(fpspio_qn_t)       :: qn
      real(c_double),   value :: energy
      type(c_ptr)             :: mesh
      real(c_double)          :: proj(*)
    end function pspio_projector_init

    ! free
    subroutine pspio_projector_free(projector) bind(c)
      import
      type(c_ptr) :: projector
    end subroutine pspio_projector_free

    ! eval
    real(c_double) function pspio_projector_eval(projector, r) bind(c)
      import
      type(c_ptr)           :: projector
      real(c_double), value :: r
    end function pspio_projector_eval

    ! get_energy
    real(c_double) function pspio_projector_get_energy(projector) bind(c)
      import
      type(c_ptr) :: projector
    end function pspio_projector_get_energy

    ! get_qn
    type(fpspio_qn_t) function pspio_projector_get_qn(projector) bind(c)
      import
      type(c_ptr) :: projector      
    end function pspio_projector_get_qn


    !----------------------------------------------------------------------------!
    ! pspio_xc                                                                   !
    !----------------------------------------------------------------------------!

    ! alloc
    integer(c_int) function pspio_xc_alloc(xc) bind(c)
      import
      type(c_ptr) :: xc
    end function pspio_xc_alloc

    ! free
    subroutine pspio_xc_free(xc) bind(c)
      import
      type(c_ptr) :: xc
    end subroutine pspio_xc_free

    ! set_exchange
    integer(c_int) function pspio_xc_set_exchange(xc, exchange) bind(c)
      import
      type(c_ptr)           :: xc
      integer(c_int), value :: exchange
    end function pspio_xc_set_exchange

    ! set_correlation
    integer(c_int) function pspio_xc_set_correlation(xc, correlation) bind(c)
      import
      type(c_ptr)           :: xc
      integer(c_int), value :: correlation
    end function pspio_xc_set_correlation

    ! set_nlcc_scheme
    integer(c_int) function pspio_xc_set_nlcc_scheme(xc, nlcc_scheme) bind(c)
      import
      type(c_ptr)           :: xc
      integer(c_int), value :: nlcc_scheme
    end function pspio_xc_set_nlcc_scheme

    ! set_nlcc_density
    integer(c_int) function pspio_xc_set_nlcc_density(xc, mesh, cd, cdp, cdpp) bind(c)
      import
      type(c_ptr)    :: xc
      type(c_ptr)    :: mesh
      real(c_double) :: cd(*)
      real(c_double) :: cdp(*)
      real(c_double) :: cdpp(*)
    end function pspio_xc_set_nlcc_density

    ! get_correlation
    integer(c_int) function pspio_xc_get_correlation(xc) bind(c)
      import
      type(c_ptr) :: xc
    end function pspio_xc_get_correlation

    ! get_exchange
    integer(c_int) function pspio_xc_get_exchange(xc) bind(c)
      import
      type(c_ptr) :: xc
    end function pspio_xc_get_exchange

    ! has_nlcc_init
    integer(c_int) function pspio_xc_has_nlcc(xc) bind(c)
      import
      type(c_ptr)    :: xc
    end function pspio_xc_has_nlcc

    ! nlcc_density_eval
    real(c_double) function pspio_xc_nlcc_density_eval(xc, r) bind(c)
      import
      type(c_ptr)           :: xc
      real(c_double), value :: r
    end function pspio_xc_nlcc_density_eval


    !----------------------------------------------------------------------------!
    ! pspio_qn                                                                   !
    !----------------------------------------------------------------------------!

    ! init
    integer(c_int) function fpspio_qn_init(qn, n, l, j) bind(c, name="pspio_qn_init")
      import
      type(fpspio_qn_t)       :: qn
      integer(c_int),   value :: n
      integer(c_int),   value :: l
      real(c_double),   value :: j
    end function fpspio_qn_init


    !----------------------------------------------------------------------------!
    ! pspio_error                                                                !
    !----------------------------------------------------------------------------!

    ! fetchall
    subroutine pspio_error_fetchall(error_msg)  bind(c)
      import
      character(kind=c_char) :: error_msg(PSPIO_STRLEN_ERROR)
    end subroutine pspio_error_fetchall

    ! flush
    subroutine fpspio_error_flush() bind(c, name="pspio_error_flush")
    end subroutine fpspio_error_flush

    ! free
    subroutine fpspio_error_free() bind(c, name="pspio_error_free")
    end subroutine fpspio_error_free

    ! get_last
    integer(c_int) function pspio_error_get_last(routine) bind(c)
      import
      character(kind=c_char) :: routine(*)
    end function pspio_error_get_last

    ! str
    character(kind=c_char) function pspio_error_string(error_id) bind(c) result(error_msg)
      import
      integer(c_int), value  :: error_id
    end function pspio_error_string


    !----------------------------------------------------------------------------!
    ! pspio_version                                                              !
    !----------------------------------------------------------------------------!
    
    ! version
    integer(c_int) function pspio_version(major, minor, micro) bind(c)
      import
      integer(c_int) :: major, minor, micro
    end function pspio_version


  end interface


contains

  !----------------------------------------------------------------------------!
  ! fpspio_pspdata                                                             !
  !----------------------------------------------------------------------------!

  ! init
  integer function fpspio_pspdata_alloc(pspdata) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata

    ierr = pspio_pspdata_alloc(pspdata%ptr)

  end function fpspio_pspdata_alloc

  ! read
  integer function fpspio_pspdata_read(pspdata, format, filename) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: format
    character(len=*),       intent(in)    :: filename

    ierr = pspio_pspdata_read(pspdata%ptr, format, f_to_c_string(filename))

  end function fpspio_pspdata_read

  ! write
  integer function fpspio_pspdata_write(pspdata, format, filename) result(ierr)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    integer,                intent(in) :: format
    character(len=*),       intent(in) :: filename

    ierr = pspio_pspdata_write(pspdata%ptr, format, f_to_c_string(filename))

  end function fpspio_pspdata_write

  ! free
  subroutine fpspio_pspdata_free(pspdata)
    type(fpspio_pspdata_t), intent(inout) :: pspdata

    call pspio_pspdata_free(pspdata%ptr)

  end subroutine fpspio_pspdata_free

  ! get_format_guessed
  integer function fpspio_pspdata_get_format_guessed(pspdata) result(format_guessed)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    format_guessed = pspio_pspdata_get_format_guessed(pspdata%ptr)

  end function fpspio_pspdata_get_format_guessed

  ! set_symbol
  integer function fpspio_pspdata_set_symbol(pspdata, symbol) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    character(len=*),       intent(in)    :: symbol

    ierr = pspio_pspdata_set_symbol(pspdata%ptr, f_to_c_string(symbol))

  end function fpspio_pspdata_set_symbol

  ! get_symbol
  character(len=3) function fpspio_pspdata_get_symbol(pspdata) result(symbol)
    type(fpspio_pspdata_t), intent(in) :: pspdata
 
    call c_to_f_string(pspio_pspdata_get_symbol(pspdata%ptr), symbol)

  end function fpspio_pspdata_get_symbol

  ! set_z
  integer function fpspio_pspdata_set_z(pspdata, z) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    real(8),                intent(in)    :: z

    ierr = pspio_pspdata_set_z(pspdata%ptr, z)

  end function fpspio_pspdata_set_z

  ! get_z
  real(8) function fpspio_pspdata_get_z(pspdata) result(z)
    type(fpspio_pspdata_t), intent(in)  :: pspdata

    z = pspio_pspdata_get_z(pspdata%ptr)

  end function fpspio_pspdata_get_z

  ! set_zvalence
  integer function fpspio_pspdata_set_zvalence(pspdata, zvalence) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    real(8),                intent(in)    :: zvalence

    ierr = pspio_pspdata_set_zvalence(pspdata%ptr, zvalence)

  end function fpspio_pspdata_set_zvalence

  ! get_zvalence
  real(8) function fpspio_pspdata_get_zvalence(pspdata) result(zvalence)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    zvalence = pspio_pspdata_get_zvalence(pspdata%ptr)

  end function fpspio_pspdata_get_zvalence

  ! set_l_max
  integer function fpspio_pspdata_set_l_max(pspdata, l_max) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: l_max

    ierr = pspio_pspdata_set_l_max(pspdata%ptr, l_max)

  end function fpspio_pspdata_set_l_max

  ! get_l_max
  integer function fpspio_pspdata_get_l_max(pspdata) result(l_max)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    l_max = pspio_pspdata_get_l_max(pspdata%ptr)

  end function fpspio_pspdata_get_l_max

  ! set_wave_eq
  integer function fpspio_pspdata_set_wave_eq(pspdata, wave_eq) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: wave_eq

    ierr = pspio_pspdata_set_wave_eq(pspdata%ptr, wave_eq)

  end function fpspio_pspdata_set_wave_eq

  ! get_wave_eq
  integer function fpspio_pspdata_get_wave_eq(pspdata) result(wave_eq)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    wave_eq = pspio_pspdata_get_wave_eq(pspdata%ptr)

  end function fpspio_pspdata_get_wave_eq

  ! set_mesh
  integer function fpspio_pspdata_set_mesh(pspdata, mesh) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    type(fpspio_mesh_t),    intent(in)    :: mesh

    ierr = pspio_pspdata_set_mesh(pspdata%ptr, mesh%ptr)

  end function fpspio_pspdata_set_mesh

  ! get_mesh
  type(fpspio_mesh_t) function fpspio_pspdata_get_mesh(pspdata) result(mesh)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    
    mesh%ptr = pspio_pspdata_get_mesh(pspdata%ptr)

  end function fpspio_pspdata_get_mesh

  ! set_n_states
  integer function fpspio_pspdata_set_n_states(pspdata, n_states) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: n_states

    ierr = pspio_pspdata_set_n_states(pspdata%ptr, n_states)

  end function fpspio_pspdata_set_n_states

  ! get_n_states
  integer function fpspio_pspdata_get_n_states(pspdata) result(n_states)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    n_states = pspio_pspdata_get_n_states(pspdata%ptr)

  end function fpspio_pspdata_get_n_states

  ! set_state
  integer function fpspio_pspdata_set_states(pspdata, states) result(ierr)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    type(fpspio_state_t),   intent(in) :: states(*)

    ierr = pspio_pspdata_set_states(pspdata%ptr, states%ptr)

  end function fpspio_pspdata_set_states
  
  ! get_state
  type(fpspio_state_t) function fpspio_pspdata_get_state(pspdata, i) result(state)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    integer,                intent(in) :: i

!    state%ptr = pspio_pspdata_get_state(pspdata%ptr)

  end function fpspio_pspdata_get_state

  ! set_scheme
  integer function fpspio_pspdata_set_scheme(pspdata, scheme) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: scheme

    ierr = pspio_pspdata_set_scheme(pspdata%ptr, scheme)

  end function fpspio_pspdata_set_scheme

  ! get_scheme
  integer function fpspio_pspdata_get_scheme(pspdata) result(scheme)
    type(fpspio_pspdata_t), intent(in)  :: pspdata

    scheme = pspio_pspdata_get_scheme(pspdata%ptr)

  end function fpspio_pspdata_get_scheme

  ! set_n_potentials
  integer function fpspio_pspdata_set_n_potentials(pspdata, n_potentials) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: n_potentials

    ierr = pspio_pspdata_set_n_potentials(pspdata%ptr, n_potentials)

  end function fpspio_pspdata_set_n_potentials

  ! get_n_potentials
  integer function fpspio_pspdata_get_n_potentials(pspdata) result(n_potentials) 
    type(fpspio_pspdata_t), intent(in) :: pspdata

    n_potentials = pspio_pspdata_get_n_potentials(pspdata%ptr)

  end function fpspio_pspdata_get_n_potentials

  ! set_potentials
  integer function fpspio_pspdata_set_potentials(pspdata, potentials) result(ierr)
    type(fpspio_pspdata_t),   intent(inout) :: pspdata
    type(fpspio_potential_t), intent(in)    :: potentials(*)

    ierr = pspio_pspdata_set_potentials(pspdata%ptr, potentials%ptr)

  end function fpspio_pspdata_set_potentials

  ! get_potential
  type(fpspio_potential_t) function fpspio_pspdata_get_potential(pspdata, i) result(potential)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    integer,                intent(in) :: i
    
!    potential%ptr = pspio_pspdata_get_potential(pspdata%ptr)

  end function fpspio_pspdata_get_potential

  ! set_n_projectors
  integer function fpspio_pspdata_set_n_projectors(pspdata, n_projectors) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: n_projectors

    ierr = pspio_pspdata_set_n_projectors(pspdata%ptr, n_projectors)

  end function fpspio_pspdata_set_n_projectors

  ! get_n_projectors
  integer function fpspio_pspdata_get_n_projectors(pspdata) result(n_projectors)
    type(fpspio_pspdata_t), intent(in)  :: pspdata

    n_projectors = pspio_pspdata_get_n_projectors(pspdata%ptr)

  end function fpspio_pspdata_get_n_projectors
  
  ! set_projectors
  integer function fpspio_pspdata_set_projectors(pspdata, projectors) result(ierr)
    type(fpspio_pspdata_t),   intent(inout) :: pspdata
    type(fpspio_projector_t), intent(in)    :: projectors(*)

    ierr = pspio_pspdata_set_projectors(pspdata%ptr, projectors%ptr)

  end function fpspio_pspdata_set_projectors

  ! get_projector
  type(fpspio_projector_t) function fpspio_pspdata_get_projector(pspdata, i) result(projector)
    type(fpspio_pspdata_t), intent(in) :: pspdata
    integer,                intent(in) :: i
    
!    projector%ptr = pspio_pspdata_get_projector(pspdata%ptr)

  end function fpspio_pspdata_get_projector

  ! set_projectors_l_max
  integer function fpspio_pspdata_set_projectors_l_max(pspdata, l_max) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: l_max

    ierr = pspio_pspdata_set_projectors_l_max(pspdata%ptr, l_max)

  end function fpspio_pspdata_set_projectors_l_max

  ! get_projectors_l_max
  integer function fpspio_pspdata_get_projectors_l_max(pspdata) result(l_max)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    l_max = pspio_pspdata_get_projectors_l_max(pspdata%ptr)

  end function fpspio_pspdata_get_projectors_l_max
    
  ! set_l_local
  integer function fpspio_pspdata_set_l_local(pspdata, l_local) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    integer,                intent(in)    :: l_local

    ierr = pspio_pspdata_set_l_local(pspdata%ptr, l_local)

  end function fpspio_pspdata_set_l_local

  ! get_l_local
  integer function fpspio_pspdata_get_l_local(pspdata) result(l_local) 
    type(fpspio_pspdata_t), intent(in) :: pspdata

    l_local = pspio_pspdata_get_l_local(pspdata%ptr)

  end function fpspio_pspdata_get_l_local
    
  ! set_vlocal
  integer function fpspio_pspdata_set_vlocal(pspdata, vlocal) result(ierr)
    type(fpspio_pspdata_t),   intent(inout) :: pspdata
    type(fpspio_potential_t), intent(in)    :: vlocal

    ierr = pspio_pspdata_set_vlocal(pspdata%ptr, vlocal%ptr)

  end function fpspio_pspdata_set_vlocal
  
  ! get_vlocal
  type(fpspio_potential_t) function fpspio_pspdata_get_vlocal(pspdata) result(vlocal)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    vlocal%ptr = pspio_pspdata_get_vlocal(pspdata%ptr)

  end function fpspio_pspdata_get_vlocal

  ! set_xc
  integer function fpspio_pspdata_set_xc(pspdata, xc) result(ierr)
    type(fpspio_pspdata_t), intent(inout) :: pspdata
    type(fpspio_xc_t),      intent(in)    :: xc

    ierr = pspio_pspdata_set_xc(pspdata%ptr, xc%ptr)

  end function fpspio_pspdata_set_xc

  ! get_xc
  type(fpspio_xc_t) function fpspio_pspdata_get_xc(pspdata) result(xc)
    type(fpspio_pspdata_t), intent(in) :: pspdata

    xc%ptr = pspio_pspdata_get_xc(pspdata%ptr)

  end function fpspio_pspdata_get_xc

  ! set_rho_valence
  integer function fpspio_pspdata_set_rho_valence(pspdata, rho_valence) result(ierr)
    type(fpspio_pspdata_t),  intent(inout) :: pspdata
    real(8),                 intent(in)    :: rho_valence(*)

    ierr = pspio_pspdata_set_rho_valence(pspdata%ptr, rho_valence)

  end function fpspio_pspdata_set_rho_valence

  ! rho_valence_eval_s
  subroutine fpspio_pspdata_rho_valence_eval_s(pspdata, r, rho)
    type(fpspio_pspdata_t), intent(in)  :: pspdata
    real(8),                intent(in)  :: r
    real(8),                intent(out) :: rho


  end subroutine fpspio_pspdata_rho_valence_eval_s

  ! rho_valence_eval_v
  subroutine fpspio_pspdata_rho_valence_eval_v(pspdata, np, r, rho)
    type(fpspio_pspdata_t), intent(in)  :: pspdata
    integer,                intent(in)  :: np
    real(8),                intent(in)  :: r(np)
    real(8),                intent(out) :: rho(np)


  end subroutine fpspio_pspdata_rho_valence_eval_v



  !----------------------------------------------------------------------------!
  ! fpspio_mesh                                                                !
  !----------------------------------------------------------------------------!

  ! alloc
  integer function fpspio_mesh_alloc(mesh, np) result(ierr)
    type(fpspio_mesh_t), intent(inout) :: mesh
    integer,             intent(in)    :: np

    ierr = pspio_mesh_alloc(mesh%ptr, np)

  end function fpspio_mesh_alloc
  
  ! init
  integer function fpspio_mesh_init(mesh, type, a, b, r, rab) result(ierr)
    type(fpspio_mesh_t), intent(inout) :: mesh
    integer,             intent(in)    :: type
    real(8),             intent(in)    :: a
    real(8),             intent(in)    :: b
    real(8),             intent(in)    :: r(*)
    real(8),             intent(in)   :: rab(*)

    ierr = pspio_mesh_init(mesh%ptr, type, a, b, r, rab)

  end function fpspio_mesh_init

  ! init_from_points
  integer function fpspio_mesh_init_from_points(mesh, r, rab) result(ierr)
    type(fpspio_mesh_t), intent(inout) :: mesh
    real(8),             intent(in)    :: r(*)
    real(8),             intent(in)    :: rab(*)
    
    ierr = pspio_mesh_init_from_points(mesh%ptr, r, rab)

  end function fpspio_mesh_init_from_points

  ! init_from_parameters
  integer function fpspio_mesh_init_from_parameters(mesh, type, a, b) result(ierr)
    type(fpspio_mesh_t), intent(inout) :: mesh
    integer,             intent(in)    :: type
    real(8),             intent(in)    :: a
    real(8),             intent(in)    :: b

    ierr = pspio_mesh_init_from_parameters(mesh%ptr, type, a, b)

  end function fpspio_mesh_init_from_parameters

  ! free
  subroutine fpspio_mesh_free(mesh)
    type(fpspio_mesh_t), intent(inout) :: mesh

    call pspio_mesh_free(mesh%ptr)

  end subroutine fpspio_mesh_free

  ! get_np
  integer function fpspio_mesh_get_np(mesh) result(np)
    type(fpspio_mesh_t), intent(in)  :: mesh

    np = pspio_mesh_get_np(mesh%ptr)

  end function fpspio_mesh_get_np

  ! get_r
  function fpspio_mesh_get_r(mesh) result(r)
    type(fpspio_mesh_t), intent(in)  :: mesh
    real(8), pointer :: r(:)

!    r = pspio_mesh_get_r(mesh%ptr)

  end function fpspio_mesh_get_r

  ! get_rab
  function fpspio_mesh_get_rab(mesh) result(rab)
    type(fpspio_mesh_t), intent(in)  :: mesh
    real(8), pointer :: rab(:)

!    rab = pspio_mesh_get_rab(mesh%ptr)

  end function fpspio_mesh_get_rab


  !----------------------------------------------------------------------------!
  ! fpspio_state                                                               !
  !----------------------------------------------------------------------------!

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

  ! free
  subroutine fpspio_state_free(state)
    type(fpspio_state_t), intent(inout) :: state

    call pspio_state_free(state%ptr)

  end subroutine fpspio_state_free

  ! wf_eval_s
  subroutine fpspio_state_wf_eval_s(state, r, wf)
    type(fpspio_state_t), intent(in)  :: state
    real(8),     intent(in)  :: r
    real(8),     intent(out) :: wf

!    call pspio_state_wf_eval_s(state%ptr, r, wf)

  end subroutine fpspio_state_wf_eval_s

  ! wf_eval_v
  subroutine fpspio_state_wf_eval_v(state, np, r, wf)
    type(fpspio_state_t), intent(in)  :: state
    integer,     intent(in)  :: np
    real(8),     intent(in)  :: r(np)
    real(8),     intent(out) :: wf(np)

!    call pspio_state_wf_eval_v(state%ptr, np, r, wf)

  end subroutine fpspio_state_wf_eval_v

  type(fpspio_qn_t) function fpspio_state_get_qn(state) result(qn)
    type(fpspio_state_t), intent(in)  :: state

    qn = pspio_state_get_qn(state%ptr)

  end function fpspio_state_get_qn

  ! get_occ
  real(8) function fpspio_state_get_occ(state) result(occ)
    type(fpspio_state_t), intent(in)  :: state

    occ = pspio_state_get_occ(state%ptr)

  end function fpspio_state_get_occ

  ! get_ev
  real(8) function fpspio_state_get_ev(state) result(ev)
    type(fpspio_state_t), intent(in)  :: state
    
    ev = pspio_state_get_ev(state%ptr)
    
  end function fpspio_state_get_ev

  ! get_rc
  real(8) function fpspio_state_get_rc(state) result(rc)
    type(fpspio_state_t), intent(in)  :: state
     
    rc = pspio_state_get_rc(state%ptr)
   
  end function fpspio_state_get_rc


  !----------------------------------------------------------------------------!
  ! fpspio_potential                                                           !
  !----------------------------------------------------------------------------!

  ! alloc
  integer function fpspio_potential_alloc(potential, np) result(ierr)
    type(fpspio_potential_t), intent(inout) :: potential
    integer,                  intent(in)    :: np

    ierr = pspio_potential_alloc(potential%ptr, np)

  end function fpspio_potential_alloc

  ! set
  integer function fpspio_potential_init(potential, qn, mesh, v) result(ierr)
    type(fpspio_potential_t), intent(inout) :: potential
    type(fpspio_qn_t),        intent(in)    :: qn
    type(fpspio_mesh_t),      intent(in)    :: mesh
    real(8),                  intent(in)    :: v(*)

    ierr = pspio_potential_init(potential%ptr, qn, mesh%ptr, v)

  end function fpspio_potential_init

  ! free
  subroutine fpspio_potential_free(potential)
    type(fpspio_potential_t), intent(inout) :: potential

    call pspio_potential_free(potential%ptr)

  end subroutine fpspio_potential_free

  ! eval_s
  subroutine fpspio_potential_eval_s(potential, r, v)
    type(fpspio_potential_t), intent(in)  :: potential
    real(8),                  intent(in)  :: r
    real(8),                  intent(out) :: v

!    call  pspio_potential_eval_s(potential%ptr, r, v)

  end subroutine fpspio_potential_eval_s

  ! eval_v
  subroutine fpspio_potential_eval_v(potential, np, r, v)
    type(fpspio_potential_t), intent(in)  :: potential
    integer,                  intent(in)  :: np
    real(8),                  intent(in)  :: r(np)
    real(8),                  intent(out) :: v(np)

!    call pspio_potential_eval_v(potential%ptr, np, r, v) 

  end subroutine fpspio_potential_eval_v


  !----------------------------------------------------------------------------!
  ! fpspio_projector                                                           !
  !----------------------------------------------------------------------------!
    
  ! alloc
  integer function fpspio_projector_alloc(projector, np) result(ierr)
    type(fpspio_projector_t), intent(inout) :: projector
    integer,                  intent(in)    :: np

    ierr = pspio_projector_alloc(projector%ptr, np)

  end function fpspio_projector_alloc

  ! init
  integer function fpspio_projector_init(projector, qn, energy, mesh, proj) result(ierr)
    type(fpspio_projector_t), intent(inout) :: projector
    type(fpspio_qn_t),        intent(in)    :: qn
    real(8),                  intent(in)    :: energy
    type(fpspio_mesh_t),      intent(in)    :: mesh
    real(8),                  intent(in)    :: proj(*)

    ierr = pspio_projector_init(projector%ptr, qn, energy, mesh%ptr, proj)

  end function fpspio_projector_init

  ! free
  subroutine fpspio_projector_free(projector)
    type(fpspio_projector_t), intent(inout) :: projector

    call pspio_projector_free(projector%ptr)

  end subroutine fpspio_projector_free

  ! eval_s
  subroutine fpspio_projector_eval_s(projector, r, p)
    type(fpspio_projector_t), intent(in)  :: projector
    real(8),                  intent(in)  :: r
    real(8),                  intent(out) :: p

  end subroutine fpspio_projector_eval_s

  ! eval_v
  subroutine fpspio_projector_eval_v(projector, np, r, p)
    type(fpspio_projector_t), intent(in)  :: projector
    integer,                  intent(in)  :: np
    real(8),                  intent(in)  :: r(np)
    real(8),                  intent(out) :: p(np)

  end subroutine fpspio_projector_eval_v

  ! get_energy
  real(8) function fpspio_projector_get_energy(projector) result(energy)
    type(fpspio_projector_t), intent(in)  :: projector

    energy = pspio_projector_get_energy(projector%ptr)

  end function fpspio_projector_get_energy

  ! get_qn
  type(fpspio_qn_t) function fpspio_projector_get_qn(projector) result(qn)
    type(fpspio_projector_t), intent(in)  :: projector

    qn = pspio_projector_get_qn(projector%ptr)

  end function fpspio_projector_get_qn


  !----------------------------------------------------------------------------!
  ! fpspio_xc                                                                  !
  !----------------------------------------------------------------------------!

  ! alloc
  integer function fpspio_xc_alloc(xc) result(ierr)
    type(fpspio_xc_t), intent(inout) :: xc

    ierr = pspio_xc_alloc(xc%ptr)

  end function fpspio_xc_alloc

  ! free
  subroutine fpspio_xc_free(xc)
    type(fpspio_xc_t), intent(inout) :: xc

    call pspio_xc_free(xc%ptr)

  end subroutine fpspio_xc_free

  ! set_exchange
  integer function fpspio_xc_set_exchange(xc, exchange) result(ierr)
    type(fpspio_xc_t), intent(inout) :: xc
    integer,           intent(in)    :: exchange

    ierr = pspio_xc_set_exchange(xc%ptr, exchange)

  end function fpspio_xc_set_exchange

  ! set_correlation
  integer function fpspio_xc_set_correlation(xc, correlation) result(ierr)
    type(fpspio_xc_t), intent(inout) :: xc
    integer,           intent(in)    :: correlation

    ierr = pspio_xc_set_correlation(xc%ptr, correlation)

  end function fpspio_xc_set_correlation

  ! set_nlcc_scheme
  integer function fpspio_xc_set_nlcc_scheme(xc, nlcc_scheme) result(ierr)
    type(fpspio_xc_t), intent(inout) :: xc
    integer,           intent(in)    :: nlcc_scheme

    ierr = pspio_xc_set_nlcc_scheme(xc%ptr, nlcc_scheme)

  end function fpspio_xc_set_nlcc_scheme

  ! set_nlcc_density
  integer function fpspio_xc_set_nlcc_density(xc, mesh, cd, cdp, cdpp) result(ierr)
    type(fpspio_xc_t),   intent(inout) :: xc
    type(fpspio_mesh_t), intent(in)    :: mesh
    real(8),             intent(in)    :: cd(*)
    real(8),             intent(in)    :: cdp(*)
    real(8),             intent(in)    :: cdpp(*)

    ierr = pspio_xc_set_nlcc_density(xc%ptr, mesh%ptr, cd, cdp, cdpp)

  end function fpspio_xc_set_nlcc_density

  ! get_correlation
  integer function fpspio_xc_get_correlation(xc) result(correlation)
    type(fpspio_xc_t), intent(in)  :: xc

    correlation = pspio_xc_get_correlation(xc%ptr)

  end function fpspio_xc_get_correlation

  ! get_exchange
  integer function fpspio_xc_get_exchange(xc) result(exchange)
    type(fpspio_xc_t), intent(in)  :: xc

    exchange = pspio_xc_get_exchange(xc%ptr)

  end function fpspio_xc_get_exchange

  ! has_nlcc
  logical function fpspio_xc_has_nlcc(xc) result(has_nlcc)
    type(fpspio_xc_t), intent(in)  :: xc

    has_nlcc = pspio_xc_has_nlcc(xc%ptr) /= 0

  end function fpspio_xc_has_nlcc

  ! nlcc_density_eval_s
  subroutine fpspio_xc_nlcc_density_eval_s(xc, r, nlcc_dens)
    type(fpspio_xc_t), intent(in)  :: xc
    real(8),           intent(in)  :: r
    real(8),           intent(out) :: nlcc_dens

  end subroutine fpspio_xc_nlcc_density_eval_s

  ! nlcc_density_eval_v
  subroutine fpspio_xc_nlcc_density_eval_v(xc, np, r, nlcc_dens)
    type(fpspio_xc_t), intent(in)  :: xc
    integer,           intent(in)  :: np
    real(8),           intent(in)  :: r(np)
    real(8),           intent(out) :: nlcc_dens(np)

  end subroutine fpspio_xc_nlcc_density_eval_v


  !----------------------------------------------------------------------------!
  ! fpspio_qn                                                                  !
  !----------------------------------------------------------------------------!


  !----------------------------------------------------------------------------!
  ! fpspio_error                                                               !
  !----------------------------------------------------------------------------!

  ! fetchall
  subroutine fpspio_error_fetchall(error_msg)
    character(len=PSPIO_STRLEN_ERROR), intent(out) :: error_msg

    character(kind=c_char) :: c_error_msg(PSPIO_STRLEN_ERROR)
    
    call pspio_error_fetchall(c_error_msg)
    call c_to_f_string(c_error_msg, error_msg)

  end subroutine fpspio_error_fetchall

  ! get_last
  integer function fpspio_error_get_last(routine) result(last)
    character(len=*), intent(in) :: routine

    last = pspio_error_get_last(f_to_c_string(routine))

  end function fpspio_error_get_last

  ! str
  subroutine fpspio_error_string(error_id, error_msg)
    integer,                           intent(in)  :: error_id
    character(len=PSPIO_STRLEN_ERROR), intent(out) :: error_msg

    character(kind=c_char) :: c_error_msg(PSPIO_STRLEN_ERROR)

    c_error_msg = pspio_error_string(error_id)
    call c_to_f_string(c_error_msg, error_msg)

  end subroutine fpspio_error_string


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
