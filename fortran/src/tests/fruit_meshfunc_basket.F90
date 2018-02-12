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

module fruit_meshfunc_basket

  use fruit

  implicit none

contains

  subroutine fruit_meshfunc_test_all_tests()

    use fruit_meshfunc_test

    implicit none

    ! pspiof_meshfunc_alloc
    ! Note: no setup nor teardown for this test
    write(*, '(/A)') "  ..running test: test_meshfunc_alloc"
    call set_unit_name('test_meshfunc_alloc')
    call run_test_case(test_meshfunc_alloc, "test_meshfunc_alloc")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_meshfunc_alloc", "fruit_meshfunc_test")
    else
      call case_passed_xml("test_meshfunc_alloc", "fruit_meshfunc_test")
    end if

    ! pspiof_meshfunc_init 
    call setup()
    write(*, '(/A)') "  ..running test: test_meshfunc_init"
    call set_unit_name('test_meshfunc_init')
    call run_test_case(test_meshfunc_init, "test_meshfunc_init")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_meshfunc_init", "fruit_meshfunc_test")
    else
      call case_passed_xml("test_meshfunc_init", "fruit_meshfunc_test")
    end if
    call teardown()

  end subroutine fruit_meshfunc_test_all_tests

  subroutine fruit_basket()

    implicit none

    call fruit_meshfunc_test_all_tests()

  end subroutine fruit_basket

end module fruit_meshfunc_basket
