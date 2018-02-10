!! Copyright (C) 2016-2017 Yann Pouillon <devops@materialsevolution.es>
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

module fruit_meshfunc_test

  use pspiof_m
  use fruit

  implicit none

  integer, parameter, private :: dp = kind(0.0d0)
  integer, parameter, private :: mesh_size = 8
  real(dp), parameter, private :: meshfunc_tol = 1.0e-13_dp

  type(pspiof_mesh_t), private :: m1, m2
  type(pspiof_meshfunc_t), private :: mf11, mf12, mf20
  real(dp), dimension(:), allocatable, private :: f11, f11p, f11pp
  real(dp), dimension(:), allocatable, private :: f12, f12p, f12pp
  real(dp), dimension(:), allocatable, private :: f20, f20p, f20pp

  private :: meshfunc_compare_values

contains

  subroutine meshfunc_compare_values(mesh, meshfunc, f, fp, fpp, tol)

    implicit none

    type(pspiof_mesh_t), intent(in) :: mesh
    type(pspiof_meshfunc_t), intent(in) :: meshfunc
    real(dp), dimension(mesh_size), intent(in) :: f, fp, fpp
    real(dp), intent(in) :: tol

    character(len=80) :: msg
    integer :: i, np
    real(dp), dimension(mesh_size) :: f_chk, fp_chk, fpp_chk
    type(pspiof_mesh_t) :: m

    m = pspiof_meshfunc_get_mesh(meshfunc)
    np = pspiof_mesh_get_np(mesh)

    call assert_equals(mesh_size, np, &
&     "Mesh function comparison - Mesh size check")
    call assert_equals(PSPIO_EQUAL, pspiof_mesh_cmp(m, mesh), &
&     "Mesh function comparison - Mesh equality check")

    f_chk = pspiof_meshfunc_get_function(meshfunc)
    fp_chk = pspiof_meshfunc_get_deriv1(meshfunc)
    fpp_chk = pspiof_meshfunc_get_deriv2(meshfunc)
    do i=1,np
      write(*,*) "FP_CHK : ", i, " ERR = ", abs(fpp_chk(i) - fpp(i))
      write(msg, &
&       '("Mesh function comparison: f(",I0,")")') i
      call assert_true( (abs(f_chk(i)-f(i)) .le. tol), trim(msg))
      write(msg, &
&       '("Mesh function comparison: fp(",I0,")")') i
      call assert_true( (abs(fp_chk(i)-fp(i)) .le. (tol*2.0e3_dp)), trim(msg))
      write(msg, &
&       '("Mesh function comparison: fpp(",I0,")")') i
      call assert_true( (abs(fpp_chk(i)-fpp(i)) .le. tol), trim(msg))
    end do

  end subroutine meshfunc_compare_values

  ! Note: this routine assumes mesh_size = 8
  subroutine setup()

    implicit none

    real(dp), dimension(mesh_size), parameter :: r1 = &
&     [0.00_dp, 0.05_dp, 0.10_dp, 0.20_dp, 0.40_dp, 0.65_dp, 0.85_dp, 1.00_dp]
    real(dp), dimension(mesh_size), parameter :: r1ab = &
&     [0.05_dp, 0.05_dp, 0.20_dp, 0.20_dp, 0.20_dp, 0.20_dp, 0.05_dp, 0.05_dp]
    real(dp), parameter :: a = 1.0_dp
    real(dp), parameter :: b = 2.0_dp

    integer :: eid, i
    real(dp), dimension(:), pointer :: r2

    call pspiof_mesh_free(m1)
    eid = pspiof_mesh_alloc(m1, mesh_size)
    call pspiof_mesh_init_from_points(m1, r1, r1ab)

    call pspiof_mesh_free(m2)
    eid = pspiof_mesh_alloc(m2, 2*mesh_size+1)
    call pspiof_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, a, b)
    r2 => pspiof_mesh_get_r(m2)

    call pspiof_meshfunc_free(mf11)
    call pspiof_meshfunc_free(mf12)
    call pspiof_meshfunc_free(mf20)
    eid = pspiof_meshfunc_alloc(mf11, mesh_size)
    eid = pspiof_meshfunc_alloc(mf12, mesh_size)
    eid = pspiof_meshfunc_alloc(mf20, 2*mesh_size+1)

    allocate(f11(mesh_size), f11p(mesh_size), f11pp(mesh_size))
    do i=1,mesh_size
      f11(i) = r1(i) * r1(i)
      f11p(i) = 2.0_dp * r1(i)
      f11pp(i) = 2.0_dp
    end do

    allocate(f12(mesh_size), f12p(mesh_size), f12pp(mesh_size))
    do i=1,mesh_size
      f12(i) = r1(i) * r1(i) * r1(i)
      f12p(i) = 3.0_dp * r1(i) * r1(i)
      f12pp(i) = 6.0_dp * r1(i)
    end do

    allocate(f20(2*mesh_size+1), f20p(2*mesh_size+1), f20pp(2*mesh_size+1))
    do i=1,2*mesh_size+1
      f20(i) = r2(i) * r2(i)
      f20p(i) = 2.0_dp * r2(i)
      f20pp(i) = 2.0_dp
    end do

  end subroutine setup

  subroutine teardown()

    call pspiof_mesh_free(m1)
    call pspiof_mesh_free(m2)

    call pspiof_meshfunc_free(mf11)
    call pspiof_meshfunc_free(mf12)
    call pspiof_meshfunc_free(mf20)

    deallocate(f11)
    deallocate(f11p)
    deallocate(f11pp)
    deallocate(f12)
    deallocate(f12p)
    deallocate(f12pp)
    deallocate(f20)
    deallocate(f20p)
    deallocate(f20pp)

  end subroutine teardown

  subroutine test_meshfunc_alloc()

    implicit none

    type(pspiof_mesh_t), target :: m

    call assert_equals(PSPIO_SUCCESS, pspiof_meshfunc_alloc(mf11, 2*mesh_size+1), &
&     "Mesh function allocation - Return value")

    m = pspiof_meshfunc_get_mesh(mf11)

    call assert_equals(2*mesh_size+1, pspiof_mesh_get_np(m), &
&     "Mesh function allocation - Mesh size")

    call pspiof_meshfunc_free(mf11)

  end subroutine test_meshfunc_alloc

  ! Note: this test assumes mesh_size = 8
  subroutine test_meshfunc_init()

    implicit none

    real(dp), parameter :: fp_chk(mesh_size) = &
&     [2.8981947526e-02_dp, 9.2036104948e-02_dp, 2.0287363268e-01_dp, &
&      3.9868599401e-01_dp, 8.0213677058e-01_dp, 1.2920270399e+00_dp, &
&      1.7269932400e+00_dp, 1.9115033800e+00_dp]
    real(dp), parameter :: fpp_chk(mesh_size) = &
&     [0.0000000000e+00_dp, 2.5221662969e+00_dp, 1.9113348125e+00_dp, &
&      2.0049124140e+00_dp, 2.0295953518e+00_dp, 1.8895268024e+00_dp, &
&      2.4601351993e+00_dp, 0.0000000000e+00_dp]

    call assert_equals(PSPIO_SUCCESS, &
&     pspiof_meshfunc_init(mf11, m1, f11), &
&     "Mesh init - Return value (generic)")
    call meshfunc_compare_values(m1, mf11, f11, fp_chk, fpp_chk, meshfunc_tol)

  end subroutine test_meshfunc_init

end module fruit_meshfunc_test
