!! Copyright (C) 2015-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
!!                         Yann Pouillon <notifications@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! Libpspio is free software: you can redistribute it and/or modify it under
!! the terms of the GNU Lesser General Public License as published by the Free
!! Software Foundation, version 3 of the License, or (at your option) any later
!! version.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
!! more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
!! the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
!! 02110-1301  USA.


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

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
  pspdata%ptr = C_NULL_PTR

end subroutine fpspio_pspdata_free

! associated
logical function fpspio_pspdata_associated(pspdata) result(is_associated)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  is_associated = c_associated(pspdata%ptr)
  
end function fpspio_pspdata_associated


!*********************************************************************!
! Setters                                                             !
!*********************************************************************!

! pspinfo
integer function fpspio_pspdata_set_pspinfo(pspdata, pspinfo) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  type(fpspio_pspinfo_t), intent(in)    :: pspinfo

  ierr = pspio_pspdata_set_pspinfo(pspdata%ptr, pspinfo%ptr)

end function fpspio_pspdata_set_pspinfo

! symbol
integer function fpspio_pspdata_set_symbol(pspdata, symbol) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  character(len=*),       intent(in)    :: symbol

  ierr = pspio_pspdata_set_symbol(pspdata%ptr, f_to_c_string(symbol))

end function fpspio_pspdata_set_symbol

! z
integer function fpspio_pspdata_set_z(pspdata, z) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  real(8),                intent(in)    :: z

  ierr = pspio_pspdata_set_z(pspdata%ptr, z)

end function fpspio_pspdata_set_z

! zvalence
integer function fpspio_pspdata_set_zvalence(pspdata, zvalence) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  real(8),                intent(in)    :: zvalence

  ierr = pspio_pspdata_set_zvalence(pspdata%ptr, zvalence)

end function fpspio_pspdata_set_zvalence

! nelvalence
integer function fpspio_pspdata_set_nelvalence(pspdata, nelvalence) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  real(8),                intent(in)    :: nelvalence

  ierr = pspio_pspdata_set_nelvalence(pspdata%ptr, nelvalence)

end function fpspio_pspdata_set_nelvalence

! l_max
integer function fpspio_pspdata_set_l_max(pspdata, l_max) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: l_max

  ierr = pspio_pspdata_set_l_max(pspdata%ptr, l_max)

end function fpspio_pspdata_set_l_max

! wave_eq
integer function fpspio_pspdata_set_wave_eq(pspdata, wave_eq) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: wave_eq

  ierr = pspio_pspdata_set_wave_eq(pspdata%ptr, wave_eq)

end function fpspio_pspdata_set_wave_eq

! total_energy
integer function fpspio_pspdata_set_total_energy(pspdata, total_energy) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  real(8),                intent(in)    :: total_energy

  ierr = pspio_pspdata_set_total_energy(pspdata%ptr, total_energy)

end function fpspio_pspdata_set_total_energy

! mesh
integer function fpspio_pspdata_set_mesh(pspdata, mesh) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  type(fpspio_mesh_t),    intent(in)    :: mesh

  ierr = pspio_pspdata_set_mesh(pspdata%ptr, mesh%ptr)

end function fpspio_pspdata_set_mesh

! n_states
integer function fpspio_pspdata_set_n_states(pspdata, n_states) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: n_states

  ierr = pspio_pspdata_set_n_states(pspdata%ptr, n_states)

end function fpspio_pspdata_set_n_states

! state
integer function fpspio_pspdata_set_state(pspdata, index, state) result(ierr)
  type(fpspio_pspdata_t), intent(in) :: pspdata
  integer,                intent(in) :: index
  type(fpspio_state_t),   intent(in) :: state

  ierr = pspio_pspdata_set_state(pspdata%ptr, index-1, state%ptr)

end function fpspio_pspdata_set_state

! scheme
integer function fpspio_pspdata_set_scheme(pspdata, scheme) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: scheme

  ierr = pspio_pspdata_set_scheme(pspdata%ptr, scheme)

end function fpspio_pspdata_set_scheme

! n_potentials
integer function fpspio_pspdata_set_n_potentials(pspdata, n_potentials) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: n_potentials

  ierr = pspio_pspdata_set_n_potentials(pspdata%ptr, n_potentials)

end function fpspio_pspdata_set_n_potentials

! potential
integer function fpspio_pspdata_set_potential(pspdata, index, potential) result(ierr)
  type(fpspio_pspdata_t),   intent(inout) :: pspdata
  integer,                  intent(in)    :: index
  type(fpspio_potential_t), intent(in)    :: potential

  ierr = pspio_pspdata_set_potential(pspdata%ptr, index-1, potential%ptr)

end function fpspio_pspdata_set_potential

! n_projectors
integer function fpspio_pspdata_set_n_projectors(pspdata, n_projectors) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: n_projectors

  ierr = pspio_pspdata_set_n_projectors(pspdata%ptr, n_projectors)

end function fpspio_pspdata_set_n_projectors
  
! projector
integer function fpspio_pspdata_set_projector(pspdata, index, projector) result(ierr)
  type(fpspio_pspdata_t),   intent(inout) :: pspdata
  integer,                  intent(in)    :: index
  type(fpspio_projector_t), intent(in)    :: projector

  ierr = pspio_pspdata_set_projector(pspdata%ptr, index-1, projector%ptr)

end function fpspio_pspdata_set_projector

! projectors_l_max
integer function fpspio_pspdata_set_projectors_l_max(pspdata, l_max) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: l_max

  ierr = pspio_pspdata_set_projectors_l_max(pspdata%ptr, l_max)

end function fpspio_pspdata_set_projectors_l_max

! l_local
integer function fpspio_pspdata_set_l_local(pspdata, l_local) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  integer,                intent(in)    :: l_local

  ierr = pspio_pspdata_set_l_local(pspdata%ptr, l_local)

end function fpspio_pspdata_set_l_local

! vlocal
integer function fpspio_pspdata_set_vlocal(pspdata, vlocal) result(ierr)
  type(fpspio_pspdata_t),   intent(inout) :: pspdata
  type(fpspio_potential_t), intent(in)    :: vlocal

  ierr = pspio_pspdata_set_vlocal(pspdata%ptr, vlocal%ptr)

end function fpspio_pspdata_set_vlocal

! xc
integer function fpspio_pspdata_set_xc(pspdata, xc) result(ierr)
  type(fpspio_pspdata_t), intent(inout) :: pspdata
  type(fpspio_xc_t),      intent(in)    :: xc

  ierr = pspio_pspdata_set_xc(pspdata%ptr, xc%ptr)

end function fpspio_pspdata_set_xc

! rho_valence
integer function fpspio_pspdata_set_rho_valence(pspdata, rho_valence) result(ierr)
  type(fpspio_pspdata_t),  intent(inout) :: pspdata
  type(fpspio_meshfunc_t), intent(in)    :: rho_valence

  ierr = pspio_pspdata_set_rho_valence(pspdata%ptr, rho_valence%ptr)

end function fpspio_pspdata_set_rho_valence


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! format_guessed
integer function fpspio_pspdata_get_format_guessed(pspdata) result(format_guessed)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  format_guessed = pspio_pspdata_get_format_guessed(pspdata%ptr)

end function fpspio_pspdata_get_format_guessed

! pspinfo
type(fpspio_pspinfo_t) function fpspio_pspdata_get_pspinfo(pspdata) result(pspinfo)
  type(fpspio_pspdata_t), intent(in) :: pspdata
    
  pspinfo%ptr = pspio_pspdata_get_pspinfo(pspdata%ptr)

end function fpspio_pspdata_get_pspinfo

! symbol
character(len=3) function fpspio_pspdata_get_symbol(pspdata) result(symbol)
  type(fpspio_pspdata_t), intent(in) :: pspdata
 
  call c_to_f_string(pspio_pspdata_get_symbol(pspdata%ptr), symbol)

end function fpspio_pspdata_get_symbol

! z
real(8) function fpspio_pspdata_get_z(pspdata) result(z)
  type(fpspio_pspdata_t), intent(in)  :: pspdata

  z = pspio_pspdata_get_z(pspdata%ptr)

end function fpspio_pspdata_get_z

! zvalence
real(8) function fpspio_pspdata_get_zvalence(pspdata) result(zvalence)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  zvalence = pspio_pspdata_get_zvalence(pspdata%ptr)

end function fpspio_pspdata_get_zvalence

! nelvalence
real(8) function fpspio_pspdata_get_nelvalence(pspdata) result(nelvalence)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  nelvalence = pspio_pspdata_get_nelvalence(pspdata%ptr)

end function fpspio_pspdata_get_nelvalence

! l_max
integer function fpspio_pspdata_get_l_max(pspdata) result(l_max)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  l_max = pspio_pspdata_get_l_max(pspdata%ptr)

end function fpspio_pspdata_get_l_max

! wave_eq
integer function fpspio_pspdata_get_wave_eq(pspdata) result(wave_eq)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  wave_eq = pspio_pspdata_get_wave_eq(pspdata%ptr)

end function fpspio_pspdata_get_wave_eq

! total_energy
real(8) function fpspio_pspdata_get_total_energy(pspdata) result(total_energy)
  type(fpspio_pspdata_t), intent(in)  :: pspdata

  total_energy = pspio_pspdata_get_total_energy(pspdata%ptr)

end function fpspio_pspdata_get_total_energy

! mesh
type(fpspio_mesh_t) function fpspio_pspdata_get_mesh(pspdata) result(mesh)
  type(fpspio_pspdata_t), intent(in) :: pspdata
    
  mesh%ptr = pspio_pspdata_get_mesh(pspdata%ptr)

end function fpspio_pspdata_get_mesh

! n_states
integer function fpspio_pspdata_get_n_states(pspdata) result(n_states)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  n_states = pspio_pspdata_get_n_states(pspdata%ptr)

end function fpspio_pspdata_get_n_states

! state
type(fpspio_state_t) function fpspio_pspdata_get_state(pspdata, index) result(state)
  type(fpspio_pspdata_t), intent(in) :: pspdata
  integer,                intent(in) :: index

  state%ptr = pspio_pspdata_get_state(pspdata%ptr, index-1)

end function fpspio_pspdata_get_state

! scheme
integer function fpspio_pspdata_get_scheme(pspdata) result(scheme)
  type(fpspio_pspdata_t), intent(in)  :: pspdata

  scheme = pspio_pspdata_get_scheme(pspdata%ptr)

end function fpspio_pspdata_get_scheme

! n_potentials
integer function fpspio_pspdata_get_n_potentials(pspdata) result(n_potentials) 
  type(fpspio_pspdata_t), intent(in) :: pspdata

  n_potentials = pspio_pspdata_get_n_potentials(pspdata%ptr)

end function fpspio_pspdata_get_n_potentials

! potential
type(fpspio_potential_t) function fpspio_pspdata_get_potential(pspdata, index) result(potential)
  type(fpspio_pspdata_t), intent(in) :: pspdata
  integer,                intent(in) :: index
    
  potential%ptr = pspio_pspdata_get_potential(pspdata%ptr, index-1)

end function fpspio_pspdata_get_potential

! n_projectors
integer function fpspio_pspdata_get_n_projectors(pspdata) result(n_projectors)
  type(fpspio_pspdata_t), intent(in)  :: pspdata

  n_projectors = pspio_pspdata_get_n_projectors(pspdata%ptr)

end function fpspio_pspdata_get_n_projectors

! projector
type(fpspio_projector_t) function fpspio_pspdata_get_projector(pspdata, index) result(projector)
  type(fpspio_pspdata_t), intent(in) :: pspdata
  integer,                intent(in) :: index
    
  projector%ptr = pspio_pspdata_get_projector(pspdata%ptr, index-1)

end function fpspio_pspdata_get_projector

! projectors_l_max
integer function fpspio_pspdata_get_projectors_l_max(pspdata) result(l_max)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  l_max = pspio_pspdata_get_projectors_l_max(pspdata%ptr)

end function fpspio_pspdata_get_projectors_l_max

! l_local
integer function fpspio_pspdata_get_l_local(pspdata) result(l_local) 
  type(fpspio_pspdata_t), intent(in) :: pspdata

  l_local = pspio_pspdata_get_l_local(pspdata%ptr)

end function fpspio_pspdata_get_l_local

! vlocal
type(fpspio_potential_t) function fpspio_pspdata_get_vlocal(pspdata) result(vlocal)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  vlocal%ptr = pspio_pspdata_get_vlocal(pspdata%ptr)

end function fpspio_pspdata_get_vlocal

! xc
type(fpspio_xc_t) function fpspio_pspdata_get_xc(pspdata) result(xc)
  type(fpspio_pspdata_t), intent(in) :: pspdata

  xc%ptr = pspio_pspdata_get_xc(pspdata%ptr)

end function fpspio_pspdata_get_xc

! rho_valence
type(fpspio_meshfunc_t) function fpspio_pspdata_get_rho_valence(pspdata) result(rho_valence)
  type(fpspio_pspdata_t),  intent(in) :: pspdata

  rho_valence%ptr = pspio_pspdata_get_rho_valence(pspdata%ptr)

end function fpspio_pspdata_get_rho_valence
