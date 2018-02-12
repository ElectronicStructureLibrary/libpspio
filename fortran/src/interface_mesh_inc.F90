!! Copyright (C) 2015-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

  ! alloc
  integer(c_int) function pspio_mesh_alloc(mesh, np) bind(c)
    import
    type(c_ptr)           :: mesh
    integer(c_int), value :: np
  end function pspio_mesh_alloc

  ! init
  integer(c_int) function pspio_mesh_init(mesh, type, a, b, r, rab) bind(c)
    import
    type(c_ptr),    value :: mesh
    integer(c_int), value :: type
    real(c_double), value :: a
    real(c_double), value :: b
    real(c_double)        :: r(*)
    real(c_double)        :: rab(*)
  end function pspio_mesh_init

  ! init_from_points
  subroutine pspio_mesh_init_from_points(mesh, r, rab) bind(c)
    import
    type(c_ptr),    value :: mesh
    real(c_double)        :: r(*)
    type(c_ptr),    value :: rab
  end subroutine pspio_mesh_init_from_points

  ! init_from_parameters
  subroutine pspio_mesh_init_from_parameters(mesh, type, a, b) bind(c)
    import
    type(c_ptr),    value :: mesh
    integer(c_int), value :: type
    real(c_double), value :: a
    real(c_double), value :: b
  end subroutine pspio_mesh_init_from_parameters

  ! copy
  integer(c_int) function pspio_mesh_copy(dst, src) bind(c)
    import
    type(c_ptr)        :: dst
    type(c_ptr), value :: src
  end function pspio_mesh_copy

  ! free
  subroutine pspio_mesh_free(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end subroutine pspio_mesh_free


  !*********************************************************************!
  ! Getters                                                             !
  !*********************************************************************!

  ! np
  integer(c_int) function pspio_mesh_get_np(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end function pspio_mesh_get_np

  ! a
  real(c_double) function pspio_mesh_get_a(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end function pspio_mesh_get_a

  ! b
  real(c_double) function pspio_mesh_get_b(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end function pspio_mesh_get_b

  ! r
  type(c_ptr) function pspio_mesh_get_r(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end function pspio_mesh_get_r

  ! rab
  type(c_ptr) function pspio_mesh_get_rab(mesh) bind(c)
    import
    type(c_ptr), value :: mesh
  end function pspio_mesh_get_rab

  
  !*********************************************************************!
  ! Utility routines                                                    !
  !*********************************************************************!

  ! cmp
  integer(c_int) function pspio_mesh_cmp(mesh1, mesh2) bind(c)
    import
    type(c_ptr), value :: mesh1
    type(c_ptr), value :: mesh2
  end function pspio_mesh_cmp

end interface
