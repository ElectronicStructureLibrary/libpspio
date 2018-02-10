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

module fruit_mesh_basket

  use fruit

  implicit none

contains

  subroutine fruit_mesh_test_all_tests()

    use fruit_mesh_test

    implicit none

    ! pspiof_mesh_alloc
    ! Note: no setup for this test
    write(*, '(/A)') "  ..running test: test_mesh_alloc"
    call set_unit_name('test_mesh_alloc')
    call run_test_case(test_mesh_alloc, "test_mesh_alloc")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_alloc", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_alloc", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init 
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init"
    call set_unit_name('test_mesh_init')
    call run_test_case(test_mesh_init, "test_mesh_init")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_parameters(linear)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_linear"
    call set_unit_name('test_mesh_init_parameters_linear')
    call run_test_case(test_mesh_init_parameters_linear, &
&     "test_mesh_init_parameters_linear")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_linear", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_linear", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_parameters(log1)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_log1"
    call set_unit_name('test_mesh_init_parameters_log1')
    call run_test_case(test_mesh_init_parameters_log1, &
&     "test_mesh_init_parameters_log1")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_log1", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_log1", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_parameters(log2)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_log2"
    call set_unit_name('test_mesh_init_parameters_log2')
    call run_test_case(test_mesh_init_parameters_log2, &
&     "test_mesh_init_parameters_log2")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_log2", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_log2", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_points(linear)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_linear"
    call set_unit_name('test_mesh_init_points_linear')
    call run_test_case(test_mesh_init_points_linear, &
&     "test_mesh_init_points_linear")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_linear", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_linear", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_points(log1)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_log1"
    call set_unit_name('test_mesh_init_points_log1')
    call run_test_case(test_mesh_init_points_log1, &
&     "test_mesh_init_points_log1")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_log1", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_log1", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_init_from_points(log2)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_log2"
    call set_unit_name('test_mesh_init_points_log2')
    call run_test_case(test_mesh_init_points_log2, &
&     "test_mesh_init_points_log2")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_log2", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_log2", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_diff_type
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_type"
    call set_unit_name('test_mesh_cmp_diff_type')
    call run_test_case(test_mesh_cmp_diff_type, &
&     "test_mesh_cmp_diff_type")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_type", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_type", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_diff_a
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_a"
    call set_unit_name('test_mesh_cmp_diff_a')
    call run_test_case(test_mesh_cmp_diff_a, &
&     "test_mesh_cmp_diff_a")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_a", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_a", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_diff_b
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_b"
    call set_unit_name('test_mesh_cmp_diff_b')
    call run_test_case(test_mesh_cmp_diff_b, &
&     "test_mesh_cmp_diff_b")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_b", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_b", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_diff_r
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_r"
    call set_unit_name('test_mesh_cmp_diff_r')
    call run_test_case(test_mesh_cmp_diff_r, &
&     "test_mesh_cmp_diff_r")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_r", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_r", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_mtequal
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_mtequal"
    call set_unit_name('test_mesh_cmp_mtequal')
    call run_test_case(test_mesh_cmp_mtequal, &
&     "test_mesh_cmp_mtequal")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_mtequal", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_mtequal", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_cmp_equal
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_equal"
    call set_unit_name('test_mesh_cmp_equal')
    call run_test_case(test_mesh_cmp_equal, &
&     "test_mesh_cmp_equal")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_equal", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_equal", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_copy_null
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_null"
    call set_unit_name('test_mesh_copy_null')
    call run_test_case(test_mesh_copy_null, &
&     "test_mesh_copy_null")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_null", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_null", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_copy_nonnull
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_nonnull"
    call set_unit_name('test_mesh_copy_nonnull')
    call run_test_case(test_mesh_copy_nonnull, &
&     "test_mesh_copy_nonnull")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_nonnull", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_nonnull", "fruit_mesh_test")
    end if
    call teardown()

    ! pspiof_mesh_copy_nonnull_size
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_nonnull_size"
    call set_unit_name('test_mesh_copy_nonnull_size')
    call run_test_case(test_mesh_copy_nonnull_size, &
&     "test_mesh_copy_nonnull_size")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_nonnull_size", "fruit_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_nonnull_size", "fruit_mesh_test")
    end if
    call teardown()

  end subroutine fruit_mesh_test_all_tests

  subroutine fruit_basket()

    implicit none

    call fruit_mesh_test_all_tests()

  end subroutine fruit_basket

end module fruit_mesh_basket
