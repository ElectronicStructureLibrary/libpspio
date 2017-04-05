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
! Routines                                                            !
!*********************************************************************!

! add (only used for tests)
integer function pspiof_error_add(err_code, err_file, err_line, err_func) &
&                  result(ferr_code)
  integer, intent(in) :: err_code, err_line
  character(len=*), intent(in) :: err_file, err_func

  character(kind=c_char) :: c_err_file(len_trim(err_file)+1)
  character(kind=c_char) :: c_err_func(len_trim(err_func)+1)

  c_err_file = f_to_c_string(trim(err_file))
  c_err_func = f_to_c_string(trim(err_func))

  ferr_code = pspio_error_add(err_code, c_err_file, err_line, c_err_func)

end function pspiof_error_add

! fetchall
function pspiof_error_fetchall() result(error_msg)
  character(len=PSPIO_STRLEN_ERROR) :: error_msg
  type(c_ptr) :: c_error_msg

  c_error_msg = pspio_error_fetchall()
  call c_to_f_string_ptr(c_error_msg, error_msg)

end function pspiof_error_fetchall

! get_last
integer function pspiof_error_get_last(routine) result(last)
  character(len=*), intent(in) :: routine

  last = pspio_error_get_last(f_to_c_string(routine))

end function pspiof_error_get_last

! len
integer function pspiof_error_len() result(len)

  len = pspio_error_len()

end function pspiof_error_len

! string
subroutine pspiof_error_string(error_id, error_msg)
  integer,                           intent(in)  :: error_id
  character(len=PSPIO_STRLEN_ERROR), intent(out) :: error_msg

  character(kind=c_char) :: c_error_msg(PSPIO_STRLEN_ERROR)

  c_error_msg = pspio_error_string(error_id)
  call c_to_f_string(c_error_msg, error_msg)

end subroutine pspiof_error_string
