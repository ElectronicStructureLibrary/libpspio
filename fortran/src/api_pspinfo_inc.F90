!! Copyright (C) 2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
!!                    Yann Pouillon <notifications@materialsevolution.es>
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

! alloc
integer function pspiof_pspinfo_alloc(pspinfo) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo

  ierr = pspio_pspinfo_alloc(pspinfo%ptr)

end function pspiof_pspinfo_alloc

! init
integer function pspiof_pspinfo_init(pspinfo, author, code, date, description, scheme_name) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: author
  character(len=*),       intent(in)    :: code
  character(len=*),       intent(in)    :: date
  character(len=*),       intent(in)    :: description
  character(len=*),       intent(in)    :: scheme_name
  
  ierr = pspio_pspinfo_init(pspinfo%ptr, f_to_c_string(author), f_to_c_string(code), &
    f_to_c_string(date), f_to_c_string(description), f_to_c_string(scheme_name))

end function pspiof_pspinfo_init

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

! code
integer function pspiof_pspinfo_set_code(pspinfo, code) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: code

  ierr = pspio_pspinfo_set_code(pspinfo%ptr, f_to_c_string(code))

end function pspiof_pspinfo_set_code

! date
integer function pspiof_pspinfo_set_date(pspinfo, date) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: date

  ierr = pspio_pspinfo_set_date(pspinfo%ptr, f_to_c_string(date))

end function pspiof_pspinfo_set_date

! description
integer function pspiof_pspinfo_set_description(pspinfo, description) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: description

  ierr = pspio_pspinfo_set_description(pspinfo%ptr, f_to_c_string(description))

end function pspiof_pspinfo_set_description

! scheme_name
integer function pspiof_pspinfo_set_scheme_name(pspinfo, scheme_name) result(ierr)
  type(pspiof_pspinfo_t), intent(inout) :: pspinfo
  character(len=*),       intent(in)    :: scheme_name

  ierr = pspio_pspinfo_set_scheme_name(pspinfo%ptr, f_to_c_string(scheme_name))

end function pspiof_pspinfo_set_scheme_name


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! author
character(len=128) function pspiof_pspinfo_get_author(pspinfo) result(author)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_author(pspinfo%ptr), author)

end function pspiof_pspinfo_get_author

! code
character(len=128) function pspiof_pspinfo_get_code(pspinfo) result(code)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_code(pspinfo%ptr), code)

end function pspiof_pspinfo_get_code

! date
character(len=128) function pspiof_pspinfo_get_date(pspinfo) result(date)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_date(pspinfo%ptr), date)

end function pspiof_pspinfo_get_date

! description
character(len=256) function pspiof_pspinfo_get_description(pspinfo) result(description)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_description(pspinfo%ptr), description)

end function pspiof_pspinfo_get_description

! scheme_name
character(len=256) function pspiof_pspinfo_get_scheme_name(pspinfo) result(scheme_name)
  type(pspiof_pspinfo_t), intent(in) :: pspinfo
 
  call c_to_f_string(pspio_pspinfo_get_scheme_name(pspinfo%ptr), scheme_name)

end function pspiof_pspinfo_get_scheme_name


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function pspiof_pspinfo_cmp(pspinfo1, pspinfo2) result(cmp)
  type(pspiof_pspinfo_t) :: pspinfo1
  type(pspiof_pspinfo_t) :: pspinfo2

  cmp = pspio_pspinfo_cmp(pspinfo1%ptr, pspinfo2%ptr)

end function pspiof_pspinfo_cmp
