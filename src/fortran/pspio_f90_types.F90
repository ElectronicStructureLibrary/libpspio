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


module pspio_f90_types_m

  implicit none

  integer, parameter, public :: pspio_cdouble = selected_real_kind(14)
  ! Should be replaced by C_INT if iso_c_binding is available
  integer, parameter, public :: pspio_cint = 4 

  ! DO NOT EDIT THE FOLLOWING SECTION - ALL CHANGES WILL BE OVERWRITTEN!
  ! Add new definitions into pspio_common.h instead
  !%%% BEGIN PSPIO CONSTANTS
  integer(pspio_cint), parameter, public :: PSPIO_STRLEN_ERROR = 1024
  integer(pspio_cint), parameter, public :: PSPIO_STRLEN_LINE = 256
  integer(pspio_cint), parameter, public :: PSPIO_STRLEN_TITLE = 80
  integer(pspio_cint), parameter, public :: PSPIO_SUCCESS = 0
  integer(pspio_cint), parameter, public :: PSPIO_ERROR = -1
  integer(pspio_cint), parameter, public :: PSPIO_EFILE_CORRUPT = 1
  integer(pspio_cint), parameter, public :: PSPIO_EFILE_FORMAT = 2
  integer(pspio_cint), parameter, public :: PSPIO_EGSL = 3
  integer(pspio_cint), parameter, public :: PSPIO_EIO = 4
  integer(pspio_cint), parameter, public :: PSPIO_ENOFILE = 5
  integer(pspio_cint), parameter, public :: PSPIO_ENOMEM = 6
  integer(pspio_cint), parameter, public :: PSPIO_ENOSUPPORT = 7
  integer(pspio_cint), parameter, public :: PSPIO_ETYPE = 8
  integer(pspio_cint), parameter, public :: PSPIO_EVALUE = 9
  integer(pspio_cint), parameter, public :: PSPIO_FMT_NFORMATS = 19
  integer(pspio_cint), parameter, public :: PSPIO_FMT_UNKNOWN = -1
  integer(pspio_cint), parameter, public :: PSPIO_FMT_NONE = 0
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_1 = 1
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_2 = 2
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_3 = 3
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_4 = 4
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_5 = 5
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_6 = 6
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_7 = 7
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_8 = 8
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_9 = 9
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_10 = 10
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_11 = 11
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ABINIT_17 = 12
  integer(pspio_cint), parameter, public :: PSPIO_FMT_ATOM = 13
  integer(pspio_cint), parameter, public :: PSPIO_FMT_FHI98PP = 14
  integer(pspio_cint), parameter, public :: PSPIO_FMT_OCTOPUS_HGH = 15
  integer(pspio_cint), parameter, public :: PSPIO_FMT_SIESTA = 16
  integer(pspio_cint), parameter, public :: PSPIO_FMT_UPF = 17
  integer(pspio_cint), parameter, public :: PSPIO_FMT_XML = 18
  integer(pspio_cint), parameter, public :: PSPIO_EQN_DIRAC = 1
  integer(pspio_cint), parameter, public :: PSPIO_EQN_SCALAR_REL = 2
  integer(pspio_cint), parameter, public :: PSPIO_EQN_SCHRODINGER = 3
  integer(pspio_cint), parameter, public :: PSPIO_SCM_BHS = 1
  integer(pspio_cint), parameter, public :: PSPIO_SCM_GTH = 2
  integer(pspio_cint), parameter, public :: PSPIO_SCM_HAMANN = 3
  integer(pspio_cint), parameter, public :: PSPIO_SCM_HGH = 4
  integer(pspio_cint), parameter, public :: PSPIO_SCM_HSC = 5
  integer(pspio_cint), parameter, public :: PSPIO_SCM_KERKER = 6
  integer(pspio_cint), parameter, public :: PSPIO_SCM_MRPP = 7
  integer(pspio_cint), parameter, public :: PSPIO_SCM_RRKJ = 8
  integer(pspio_cint), parameter, public :: PSPIO_SCM_TM = 9
  integer(pspio_cint), parameter, public :: PSPIO_SCM_TM2 = 10
  integer(pspio_cint), parameter, public :: PSPIO_MESH_UNKNOWN = -1
  integer(pspio_cint), parameter, public :: PSPIO_MESH_NONE = 0
  integer(pspio_cint), parameter, public :: PSPIO_MESH_LOG1 = 1
  integer(pspio_cint), parameter, public :: PSPIO_MESH_LOG2 = 2
  integer(pspio_cint), parameter, public :: PSPIO_MESH_LINEAR = 3
  integer(pspio_cint), parameter, public :: PSPIO_QN_DIFF = -1
  integer(pspio_cint), parameter, public :: PSPIO_QN_EQUAL = -2
  integer(pspio_cint), parameter, public :: PSPIO_QN_MTEQUAL = -3
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_UNKNOWN = -1
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_NONE = 0
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_FHI = 1
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_LOUIE = 2
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_TETER1 = 3
  integer(pspio_cint), parameter, public :: PSPIO_NLCC_TETER2 = 4
  !%%% END PSPIO CONSTANTS

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
