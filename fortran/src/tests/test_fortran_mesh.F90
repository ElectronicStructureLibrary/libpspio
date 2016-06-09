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

program test_fortran_mesh

  use fruit
  use m_mesh_basket

  implicit none

  call init_fruit()
  call init_fruit_xml()

  call fruit_basket()

  call fruit_summary()
  call fruit_summary_xml()
  call fruit_finalize()

end program test_fortran_mesh
