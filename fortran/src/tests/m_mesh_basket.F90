!! Copyright (C) 2016 Y. Pouillon
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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

module m_mesh_basket

  use fruit

  implicit none

contains

  subroutine m_mesh_test_all_tests()

    use m_mesh_test

    implicit none

    ! fpspio_mesh_alloc
    ! Note: no setup for this test
    write(*, '(/A)') "  ..running test: test_mesh_alloc"
    call set_unit_name('test_mesh_alloc')
    call run_test_case(test_mesh_alloc, "test_mesh_alloc")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_alloc", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_alloc", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init 
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init"
    call set_unit_name('test_mesh_init')
    call run_test_case(test_mesh_init, "test_mesh_init")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_parameters(linear)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_linear"
    call set_unit_name('test_mesh_init_parameters_linear')
    call run_test_case(test_mesh_init_parameters_linear, &
&     "test_mesh_init_parameters_linear")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_linear", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_linear", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_parameters(log1)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_log1"
    call set_unit_name('test_mesh_init_parameters_log1')
    call run_test_case(test_mesh_init_parameters_log1, &
&     "test_mesh_init_parameters_log1")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_log1", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_log1", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_parameters(log2)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_parameters_log2"
    call set_unit_name('test_mesh_init_parameters_log2')
    call run_test_case(test_mesh_init_parameters_log2, &
&     "test_mesh_init_parameters_log2")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_parameters_log2", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_parameters_log2", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_points(linear)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_linear"
    call set_unit_name('test_mesh_init_points_linear')
    call run_test_case(test_mesh_init_points_linear, &
&     "test_mesh_init_points_linear")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_linear", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_linear", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_points(log1)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_log1"
    call set_unit_name('test_mesh_init_points_log1')
    call run_test_case(test_mesh_init_points_log1, &
&     "test_mesh_init_points_log1")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_log1", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_log1", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_init_from_points(log2)
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_init_points_log2"
    call set_unit_name('test_mesh_init_points_log2')
    call run_test_case(test_mesh_init_points_log2, &
&     "test_mesh_init_points_log2")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_init_points_log2", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_init_points_log2", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_diff_type
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_type"
    call set_unit_name('test_mesh_cmp_diff_type')
    call run_test_case(test_mesh_cmp_diff_type, &
&     "test_mesh_cmp_diff_type")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_type", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_type", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_diff_a
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_a"
    call set_unit_name('test_mesh_cmp_diff_a')
    call run_test_case(test_mesh_cmp_diff_a, &
&     "test_mesh_cmp_diff_a")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_a", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_a", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_diff_b
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_b"
    call set_unit_name('test_mesh_cmp_diff_b')
    call run_test_case(test_mesh_cmp_diff_b, &
&     "test_mesh_cmp_diff_b")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_b", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_b", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_diff_r
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_diff_r"
    call set_unit_name('test_mesh_cmp_diff_r')
    call run_test_case(test_mesh_cmp_diff_r, &
&     "test_mesh_cmp_diff_r")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_diff_r", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_diff_r", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_mtequal
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_mtequal"
    call set_unit_name('test_mesh_cmp_mtequal')
    call run_test_case(test_mesh_cmp_mtequal, &
&     "test_mesh_cmp_mtequal")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_mtequal", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_mtequal", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_cmp_equal
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_cmp_equal"
    call set_unit_name('test_mesh_cmp_equal')
    call run_test_case(test_mesh_cmp_equal, &
&     "test_mesh_cmp_equal")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_cmp_equal", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_cmp_equal", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_copy_null
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_null"
    call set_unit_name('test_mesh_copy_null')
    call run_test_case(test_mesh_copy_null, &
&     "test_mesh_copy_null")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_null", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_null", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_copy_nonnull
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_nonnull"
    call set_unit_name('test_mesh_copy_nonnull')
    call run_test_case(test_mesh_copy_nonnull, &
&     "test_mesh_copy_nonnull")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_nonnull", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_nonnull", "m_mesh_test")
    end if
    call teardown()

    ! fpspio_mesh_copy_nonnull_size
    call setup()
    write(*, '(/A)') "  ..running test: test_mesh_copy_nonnull_size"
    call set_unit_name('test_mesh_copy_nonnull_size')
    call run_test_case(test_mesh_copy_nonnull_size, &
&     "test_mesh_copy_nonnull_size")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_mesh_copy_nonnull_size", "m_mesh_test")
    else
      call case_passed_xml("test_mesh_copy_nonnull_size", "m_mesh_test")
    end if
    call teardown()

  end subroutine m_mesh_test_all_tests

  subroutine fruit_basket()

    implicit none

    call m_mesh_test_all_tests()

  end subroutine fruit_basket

end module m_mesh_basket
