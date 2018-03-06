!! Copyright (C) 2015-2018 Damien Caliste <dcaliste@free.fr>
!!                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

interface

  !*********************************************************************!
  ! Global routines                                                     !
  !*********************************************************************!

  ! init
  integer(c_int) function pspio_pspdata_alloc(pspdata) bind(c)
    import
    type(c_ptr) :: pspdata
  end function pspio_pspdata_alloc

  ! free
  subroutine pspio_pspdata_free(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end subroutine pspio_pspdata_free

  ! read
  integer(c_int) function pspio_pspdata_read(pspdata, format, filename) bind(c)
    import
    type(c_ptr),            value :: pspdata
    integer(c_int),         value :: format
    character(kind=c_char)        :: filename(*)
  end function pspio_pspdata_read

  ! write
  integer(c_int) function pspio_pspdata_write(pspdata, format, filename) bind(c)
    import
    type(c_ptr),            value :: pspdata
    integer(c_int),         value :: format
    character(kind=c_char)        :: filename(*)
  end function pspio_pspdata_write


  !*********************************************************************!
  ! Setters                                                             !
  !*********************************************************************!

  ! pspinfo
  integer(c_int) function pspio_pspdata_set_pspinfo(pspdata, pspinfo) bind(c)
    import
    type(c_ptr), value :: pspdata
    type(c_ptr), value :: pspinfo
  end function pspio_pspdata_set_pspinfo
  
  ! symbol
  integer(c_int) function pspio_pspdata_set_symbol(pspdata, symbol) bind(c)
    import
    type(c_ptr),            value :: pspdata
    character(kind=c_char)        :: symbol(*)
  end function pspio_pspdata_set_symbol

  ! z
  integer(c_int) function pspio_pspdata_set_z(pspdata, z) bind(c)
    import
    type(c_ptr),    value :: pspdata
    real(c_double), value :: z
  end function pspio_pspdata_set_z

  ! zvalence
  integer(c_int) function pspio_pspdata_set_zvalence(pspdata, zvalence) bind(c)
    import
    type(c_ptr),    value :: pspdata
    real(c_double), value :: zvalence
  end function pspio_pspdata_set_zvalence

  ! nelvalence
  integer(c_int) function pspio_pspdata_set_nelvalence(pspdata, nelvalence) bind(c)
    import
    type(c_ptr),    value :: pspdata
    real(c_double), value :: nelvalence
  end function pspio_pspdata_set_nelvalence

  ! l_max
  integer(c_int) function pspio_pspdata_set_l_max(pspdata, l_max) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: l_max
  end function pspio_pspdata_set_l_max

  ! wave_eq
  integer(c_int) function pspio_pspdata_set_wave_eq(pspdata, wave_eq) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: wave_eq
  end function pspio_pspdata_set_wave_eq

  ! total_energy
  integer(c_int) function pspio_pspdata_set_total_energy(pspdata, total_energy) bind(c)
    import
    type(c_ptr),    value :: pspdata
    real(c_double), value :: total_energy
  end function pspio_pspdata_set_total_energy

  ! mesh
  integer(c_int) function pspio_pspdata_set_mesh(pspdata, mesh) bind(c)
    import
    type(c_ptr), value :: pspdata
    type(c_ptr), value :: mesh
  end function pspio_pspdata_set_mesh

  ! n_states
  integer(c_int) function pspio_pspdata_set_n_states(pspdata, n_states) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: n_states
  end function pspio_pspdata_set_n_states

  ! state
  integer(c_int) function pspio_pspdata_set_state(pspdata, index, state) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
    type(c_ptr),    value :: state
  end function pspio_pspdata_set_state

  ! scheme
  integer(c_int) function pspio_pspdata_set_scheme(pspdata, scheme) bind(c)
    import
    type(c_ptr)           :: pspdata
    integer(c_int), value :: scheme
  end function pspio_pspdata_set_scheme

  ! n_potentials
  integer(c_int) function pspio_pspdata_set_n_potentials(pspdata, n_potentials) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: n_potentials
  end function pspio_pspdata_set_n_potentials

  ! potential
  integer(c_int) function pspio_pspdata_set_potential(pspdata, index, potential) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
    type(c_ptr),    value :: potential
  end function pspio_pspdata_set_potential

  ! n_projectors
  integer(c_int) function pspio_pspdata_set_n_projectors(pspdata, n_projectors) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: n_projectors
  end function pspio_pspdata_set_n_projectors

  ! projector
  integer(c_int) function pspio_pspdata_set_projector(pspdata, index, projector) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
    type(c_ptr),    value :: projector
  end function pspio_pspdata_set_projector

  ! projectors_l_max
  integer(c_int) function pspio_pspdata_set_projectors_l_max(pspdata, l_max) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: l_max
  end function pspio_pspdata_set_projectors_l_max

  ! l_local
  integer(c_int) function pspio_pspdata_set_l_local(pspdata, l_local) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: l_local
  end function pspio_pspdata_set_l_local

  ! vlocal
  integer(c_int) function pspio_pspdata_set_vlocal(pspdata, vlocal) bind(c)
    import
    type(c_ptr), value :: pspdata
    type(c_ptr), value :: vlocal
  end function pspio_pspdata_set_vlocal

  ! xc
  integer(c_int) function pspio_pspdata_set_xc(pspdata, xc) bind(c)
    import
    type(c_ptr), value :: pspdata
    type(c_ptr), value :: xc
  end function pspio_pspdata_set_xc

  ! rho_valence
  integer(c_int) function pspio_pspdata_set_rho_valence(pspdata, rho_valence) bind(c)
    import
    type(c_ptr), value :: pspdata
    type(c_ptr), value :: rho_valence
  end function pspio_pspdata_set_rho_valence


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! format_guessed
  integer(c_int) function pspio_pspdata_get_format_guessed(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_format_guessed

  ! pspinfo
  type(c_ptr) function pspio_pspdata_get_pspinfo(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_pspinfo
  
  ! symbol
  type(c_ptr) function pspio_pspdata_get_symbol(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_symbol

  ! z
  real(c_double) function pspio_pspdata_get_z(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_z

  ! zvalence
  real(c_double) function pspio_pspdata_get_zvalence(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_zvalence

  ! nelvalence
  real(c_double) function pspio_pspdata_get_nelvalence(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_nelvalence

  ! l_max
  integer(c_int) function pspio_pspdata_get_l_max(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_l_max

  ! wave_eq
  integer(c_int) function pspio_pspdata_get_wave_eq(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_wave_eq

  ! total_energy
  real(c_double) function pspio_pspdata_get_total_energy(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_total_energy

  ! mesh
  type(c_ptr) function pspio_pspdata_get_mesh(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_mesh

  ! n_states
  integer(c_int) function pspio_pspdata_get_n_states(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_n_states

  ! state
  type(c_ptr) function pspio_pspdata_get_state(pspdata, index) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
  end function pspio_pspdata_get_state

  ! scheme
  integer(c_int) function pspio_pspdata_get_scheme(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata      
  end function pspio_pspdata_get_scheme

  ! n_potentials
  integer(c_int) function pspio_pspdata_get_n_potentials(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_n_potentials

  ! potential
  type(c_ptr) function pspio_pspdata_get_potential(pspdata, index) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
  end function pspio_pspdata_get_potential

  ! n_projectors
  integer(c_int) function pspio_pspdata_get_n_projectors(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_n_projectors

  ! projector
  type(c_ptr) function pspio_pspdata_get_projector(pspdata, index) bind(c)
    import
    type(c_ptr),    value :: pspdata
    integer(c_int), value :: index
  end function pspio_pspdata_get_projector

  ! projectors_l_max
  integer(c_int) function pspio_pspdata_get_projectors_l_max(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_projectors_l_max

  ! l_local
  integer(c_int) function pspio_pspdata_get_l_local(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_l_local

  ! vlocal
  type(c_ptr) function pspio_pspdata_get_vlocal(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_vlocal

  ! xc
  type(c_ptr) function pspio_pspdata_get_xc(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_xc

  ! rho_valence
  type(c_ptr) function pspio_pspdata_get_rho_valence(pspdata) bind(c)
    import
    type(c_ptr), value :: pspdata
  end function pspio_pspdata_get_rho_valence

  ! projector energy
  real(c_double) function pspio_pspdata_get_projector_energy(pspdata, i, j) bind(c)
    import
    type(c_ptr), value :: pspdata
    integer(c_int), value :: i, j
  end function pspio_pspdata_get_projector_energy

end interface
