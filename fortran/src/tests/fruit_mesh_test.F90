!! Copyright (C) 2016-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

module fruit_mesh_test

  use pspiof_m
  use fruit

  implicit none

  integer, parameter, private :: dp = kind(0.0d0)
  integer, parameter, private :: mesh_size = 8
  real(dp), parameter, private :: mesh_tol = 1.0d-13

  type(pspiof_mesh_t), private :: m1, m2

  private :: mesh_compare_values

contains

  subroutine mesh_compare_values(mesh, a, b, r, rab, tol)

    implicit none

    type(pspiof_mesh_t), intent(in) :: mesh
    real(dp), intent(in) :: a, b, tol
    real(dp), dimension(mesh_size), intent(in) :: r, rab

    character(len=80) :: msg
    integer :: i, np
    real(dp), dimension(:), pointer :: r_chk, rab_chk

    call assert_equals(mesh_size, pspiof_mesh_get_np(mesh), &
&     "Mesh Init - Value comparison - Size check")
    call assert_equals(a, pspiof_mesh_get_a(mesh), &
&     "Mesh Init - Value comparison - Parameter check: a")
    call assert_equals(b, pspiof_mesh_get_b(mesh), &
&     "Mesh Init - Value comparison - Parameter check: b")

    np = pspiof_mesh_get_np(mesh)
    r_chk => pspiof_mesh_get_r(mesh)
    rab_chk => pspiof_mesh_get_rab(mesh)

    do i=1,np
      write(msg, &
&       '("Mesh Init - Value comparison: r(",I0,")")') i
      call assert_true( (abs(r_chk(i)-r(i)) .le. tol), trim(msg))
      write(msg, &
&       '("Mesh Init - Value comparison: rab(",I0,")")') i
      call assert_true( (abs(rab_chk(i)-rab(i)) .le. tol), trim(msg))
    end do

  end subroutine mesh_compare_values

  subroutine setup()

    implicit none

    integer :: eid

    call pspiof_mesh_free(m1)
    call pspiof_mesh_free(m2)
    eid = pspiof_mesh_alloc(m1, mesh_size)
    eid = pspiof_mesh_alloc(m2, mesh_size)

  end subroutine setup

  subroutine teardown()

    call pspiof_mesh_free(m1)
    call pspiof_mesh_free(m2)

  end subroutine teardown

  subroutine test_mesh_alloc()

    implicit none

    type(pspiof_mesh_t) :: m1

    call assert_equals(PSPIO_SUCCESS, pspiof_mesh_alloc(m1, 2*mesh_size+1), &
&     "Mesh allocation - Return value")
    call assert_equals(2*mesh_size+1, pspiof_mesh_get_np(m1), &
&     "Mesh allocation - Mesh size")

  end subroutine test_mesh_alloc

  ! Note: this test assumes mesh_size = 8
  subroutine test_mesh_init()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0
    real(dp), parameter :: r_init(mesh_size) = &
&     [0.00_dp, 0.05_dp, 0.10_dp, 0.20_dp, 0.40_dp, 0.65_dp, 0.85_dp, 1.00_dp]
    real(dp), parameter :: rab_init(mesh_size) = &
&     [0.05_dp, 0.05_dp, 0.20_dp, 0.20_dp, 0.20_dp, 0.20_dp, 0.05_dp, 0.05_dp]

    call assert_equals(PSPIO_SUCCESS, &
&     pspiof_mesh_init(m1, PSPIO_MESH_LOG1, a, b, r_init, rab_init), &
&     "Mesh init - Return value (generic)")
    call mesh_compare_values(m1, a, b, r_init, rab_init, mesh_tol)

  end subroutine test_mesh_init

  subroutine test_mesh_init_parameters_linear()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = a * i + b
      rab_chk(i) = a
    end do

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, a, b)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_parameters_linear

  subroutine test_mesh_init_parameters_log1()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = b * exp(a * i)
      rab_chk(i) = a * r_chk(i)
    end do

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, a, b)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_parameters_log1

  subroutine test_mesh_init_parameters_log2()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = b * (exp(a * i) - 1.0_dp)
      rab_chk(i) = a * r_chk(i) + a * b
    end do

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG2, a, b)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_parameters_log2

  subroutine test_mesh_init_points_linear()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = a * i + b
      rab_chk(i) = a
    end do

    call pspiof_mesh_init_from_points(m1, r_chk, rab_chk)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_points_linear

  subroutine test_mesh_init_points_log1()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = b * exp(a * i)
      rab_chk(i) = a * r_chk(i)
    end do

    call pspiof_mesh_init_from_points(m1, r_chk, rab_chk)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_points_log1

  subroutine test_mesh_init_points_log2()

    implicit none

    real(dp), parameter :: a = 1.0
    real(dp), parameter :: b = 2.0

    integer :: i
    real(dp), dimension(mesh_size) :: r_chk, rab_chk

    do i = 1,mesh_size
      r_chk(i) = b * (exp(a * i) - 1.0_dp)
      rab_chk(i) = a * r_chk(i) + a * b
    end do

    call pspiof_mesh_init_from_points(m1, r_chk, rab_chk)

    call mesh_compare_values(m1, a, b, r_chk, rab_chk, mesh_tol)

  end subroutine test_mesh_init_points_log2

  subroutine test_mesh_cmp_diff_type()

    implicit none

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LOG1, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_DIFF, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Different types")

  end subroutine test_mesh_cmp_diff_type

  subroutine test_mesh_cmp_diff_a()

    implicit none

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 2.0_dp, 2.0_dp)

    call assert_equals(PSPIO_DIFF, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Different a parameters")

  end subroutine test_mesh_cmp_diff_a

  subroutine test_mesh_cmp_diff_b()

    implicit none

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 2.0_dp, 3.0_dp)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LOG1, 2.0_dp, 2.0_dp)

    call assert_equals(PSPIO_DIFF, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Different b parameters")

  end subroutine test_mesh_cmp_diff_b

  subroutine test_mesh_cmp_diff_r()

    implicit none

    real(dp), dimension(mesh_size), parameter :: r1 = &
&     [0.00_dp, 0.05_dp, 0.10_dp, 0.20_dp, 0.40_dp, 0.65_dp, 0.85_dp, 1.00_dp]
    real(dp), dimension(mesh_size), parameter :: r2 = &
&     [0.00_dp, 0.05_dp, 0.10_dp, 0.25_dp, 0.40_dp, 0.65_dp, 0.85_dp, 1.00_dp]

    call pspiof_mesh_init_from_points(m1, r1)
    call pspiof_mesh_init_from_points(m2, r2)

    call assert_equals(PSPIO_DIFF, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Different r parameters")

  end subroutine test_mesh_cmp_diff_r

  subroutine test_mesh_cmp_mtequal()

    implicit none

    integer :: eid

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)
    call pspiof_mesh_free(m2)
    eid = pspiof_mesh_alloc(m2, 2*mesh_size+1)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_MTEQUAL, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Different sizes, equal types")

  end subroutine test_mesh_cmp_mtequal

  subroutine test_mesh_cmp_equal()

    implicit none

    integer :: eid

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_EQUAL, pspiof_mesh_cmp(m1, m2), &
&     "Mesh comparison - Identical meshes")

  end subroutine test_mesh_cmp_equal

  subroutine test_mesh_copy_null()

    implicit none

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_SUCCESS, pspiof_mesh_copy(m2, m1), &
&     "Mesh copy - Uninitialized destination - Data transfer")
    call assert_equals(PSPIO_EQUAL, pspiof_mesh_cmp(m1, m2), &
&     "Mesh copy - Uninitialized destination - Comparison")

  end subroutine test_mesh_copy_null

  subroutine test_mesh_copy_nonnull()

    implicit none

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0_dp, 2.0_dp)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_SUCCESS, pspiof_mesh_copy(m2, m1), &
&     "Mesh copy - Initialized destination - Data transfer")
    call assert_equals(PSPIO_EQUAL, pspiof_mesh_cmp(m1, m2), &
&     "Mesh copy - Initialized destination - Comparison")

  end subroutine test_mesh_copy_nonnull

  subroutine test_mesh_copy_nonnull_size()

    implicit none

    integer :: eid

    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0_dp, 2.0_dp)
    call pspiof_mesh_free(m2)
    eid = pspiof_mesh_alloc(m2, 2*mesh_size+1)
    call pspiof_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0_dp, 2.0_dp)

    call assert_equals(PSPIO_SUCCESS, pspiof_mesh_copy(m2, m1), &
&     "Mesh copy - Different sizes - Data transfer")
    call assert_equals(PSPIO_EQUAL, pspiof_mesh_cmp(m1, m2), &
&     "Mesh copy - Different sizes - Comparison")

  end subroutine test_mesh_copy_nonnull_size

end module fruit_mesh_test
