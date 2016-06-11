!! Copyright (C) 2015-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
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
