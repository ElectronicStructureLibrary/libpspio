!! Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

  call c_to_f_string_ptr(pspio_error_string(error_id), error_msg)

end subroutine pspiof_error_string
