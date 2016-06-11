!! Copyright (C) 2016 Yann Pouillon <notifications@materialsevolution.es>
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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

module m_meshfunc_basket

  use fruit

  implicit none

contains

  subroutine m_meshfunc_test_all_tests()

    use m_meshfunc_test

    implicit none

    ! fpspio_meshfunc_alloc
    ! Note: no setup nor teardown for this test
    write(*, '(/A)') "  ..running test: test_meshfunc_alloc"
    call set_unit_name('test_meshfunc_alloc')
    call run_test_case(test_meshfunc_alloc, "test_meshfunc_alloc")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_meshfunc_alloc", "m_meshfunc_test")
    else
      call case_passed_xml("test_meshfunc_alloc", "m_meshfunc_test")
    end if

    ! fpspio_meshfunc_init 
    call setup()
    write(*, '(/A)') "  ..running test: test_meshfunc_init"
    call set_unit_name('test_meshfunc_init')
    call run_test_case(test_meshfunc_init, "test_meshfunc_init")
    if (.not. is_case_passed()) then
      call case_failed_xml("test_meshfunc_init", "m_meshfunc_test")
    else
      call case_passed_xml("test_meshfunc_init", "m_meshfunc_test")
    end if
    call teardown()

  end subroutine m_meshfunc_test_all_tests

  subroutine fruit_basket()

    implicit none

    call m_meshfunc_test_all_tests()

  end subroutine fruit_basket

end module m_meshfunc_basket
