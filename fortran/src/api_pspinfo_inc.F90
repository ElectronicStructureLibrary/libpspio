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


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

! alloc
integer function pspiof_pspinfo_alloc(pspinfo) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo

  ierr = pspio_pspinfo_alloc(pspinfo%ptr)

end function pspiof_pspinfo_alloc

! copy
integer function pspiof_pspinfo_copy(src, dst) result(ierr)
  type(pspiof_pspinfo_t), intent(in)    :: src
  type(pspiof_pspinfo_t), intent(inout) :: dst

  ierr = pspio_pspinfo_copy(src%ptr, dst%ptr)

end function pspiof_pspinfo_copy

! free
subroutine pspiof_pspinfo_free(pspinfo)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo

  call pspio_pspinfo_free(pspinfo%ptr)
  pspinfo%ptr = C_NULL_PTR

end subroutine pspiof_pspinfo_free

! associated
logical function pspiof_pspinfo_associated(pspinfo) result(is_associated)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo

  is_associated = c_associated(pspinfo%ptr)

end function pspiof_pspinfo_associated


!*********************************************************************!
! Setters                                                             !
!*********************************************************************!

! author
integer function pspiof_pspinfo_set_author(pspinfo, author) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: author

  ierr = pspio_pspinfo_set_author(pspinfo%ptr, f_to_c_string(author))

end function pspiof_pspinfo_set_author

! code_name
integer function pspiof_pspinfo_set_code_name(pspinfo, code_name) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: code_name

  ierr = pspio_pspinfo_set_code_name(pspinfo%ptr, f_to_c_string(code_name))

end function pspiof_pspinfo_set_code_name

! code_name
integer function pspiof_pspinfo_set_code_version(pspinfo, code_version) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: code_version

  ierr = pspio_pspinfo_set_code_version(pspinfo%ptr, f_to_c_string(code_version))

end function pspiof_pspinfo_set_code_version

! generation_day
integer function pspiof_pspinfo_set_generation_day(pspinfo, day) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  integer(c_int),         intent(in)    :: day

  ierr = pspio_pspinfo_set_generation_day(pspinfo%ptr, day)

end function pspiof_pspinfo_set_generation_day

! generation_month
integer function pspiof_pspinfo_set_generation_month(pspinfo, month) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  integer(c_int),         intent(in)    :: month

  ierr = pspio_pspinfo_set_generation_month(pspinfo%ptr, month)

end function pspiof_pspinfo_set_generation_month

! generation_year
integer function pspiof_pspinfo_set_generation_year(pspinfo, year) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  integer(c_int),         intent(in)    :: year

  ierr = pspio_pspinfo_set_generation_year(pspinfo%ptr, year)

end function pspiof_pspinfo_set_generation_year

! description
integer function pspiof_pspinfo_set_description(pspinfo, description) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: description

  ierr = pspio_pspinfo_set_description(pspinfo%ptr, f_to_c_string(description))

end function pspiof_pspinfo_set_description


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! author
character(len=256) function pspiof_pspinfo_get_author(pspinfo) result(author)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_author(pspinfo%ptr), author)

end function pspiof_pspinfo_get_author

! code_name
character(len=256) function pspiof_pspinfo_get_code_name(pspinfo) result(code_name)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_code_name(pspinfo%ptr), code_name)

end function pspiof_pspinfo_get_code_name

! code_version
character(len=256) function pspiof_pspinfo_get_code_version(pspinfo) result(code_version)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo

  call c_to_f_string(pspio_pspinfo_get_code_version(pspinfo%ptr), code_version)

end function pspiof_pspinfo_get_code_version

! generation_day
integer function pspiof_pspinfo_get_generation_day(pspinfo) result(day)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  day = pspio_pspinfo_get_generation_day(pspinfo%ptr)

end function pspiof_pspinfo_get_generation_day

! generation_month
integer function pspiof_pspinfo_get_generation_month(pspinfo) result(month)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo

  month = pspio_pspinfo_get_generation_month(pspinfo%ptr)

end function pspiof_pspinfo_get_generation_month

! generation_year
integer function pspiof_pspinfo_get_generation_year(pspinfo) result(year)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo

  year = pspio_pspinfo_get_generation_year(pspinfo%ptr)

end function pspiof_pspinfo_get_generation_year

! description
character(len=256) function pspiof_pspinfo_get_description(pspinfo) result(description)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_description(pspinfo%ptr), description)

end function pspiof_pspinfo_get_description


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_pspinfo_cmp(pspinfo1, pspinfo2) result(cmp)
  type(pspiof_pspinfo_t) :: pspinfo1
  type(pspiof_pspinfo_t) :: pspinfo2

  cmp = pspio_pspinfo_cmp(pspinfo1%ptr, pspinfo2%ptr)

end function pspiof_pspinfo_cmp
