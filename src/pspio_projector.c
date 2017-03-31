/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "pspio_projector.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_projector_alloc(pspio_projector_t **projector, int np)
{
  int ierr;

  assert(projector != NULL);
  assert(*projector == NULL);
  assert(np > 1);

  *projector = (pspio_projector_t *) malloc (sizeof(pspio_projector_t));
  FULFILL_OR_EXIT(*projector != NULL, PSPIO_ENOMEM);

  (*projector)->proj = NULL;
  ierr = pspio_meshfunc_alloc(&(*projector)->proj, np);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_projector_free(*projector);
    *projector = NULL;
    RETURN_WITH_ERROR( ierr );
  }

  (*projector)->qn = NULL;
  ierr = pspio_qn_alloc(&(*projector)->qn);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_projector_free(*projector);
    *projector = NULL;
    RETURN_WITH_ERROR( ierr );
  }

  return PSPIO_SUCCESS;
}

int pspio_projector_init(pspio_projector_t *projector, const pspio_qn_t *qn, 
			 double energy, const pspio_mesh_t *mesh,
			 const double *pofr)
{
  assert(projector != NULL);
  assert(qn != NULL);
  assert(mesh != NULL);
  assert(pofr != NULL);

  SUCCEED_OR_RETURN(pspio_qn_copy(&projector->qn, qn));
  projector->energy = energy;
  SUCCEED_OR_RETURN(pspio_meshfunc_init(projector->proj, mesh, pofr, NULL, NULL));

  return PSPIO_SUCCESS;
}

int pspio_projector_copy(pspio_projector_t **dst, const pspio_projector_t *src)
{
  int np;

  assert(src != NULL);

  np = pspio_mesh_get_np(src->proj->mesh);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_projector_alloc(dst, np) );
  }

  /* 
   * The mesh of the destination projector must have the same number
   * of points as the mesh of the source projector
   */
  if ( pspio_mesh_get_np((*dst)->proj->mesh) != np ) {
    pspio_projector_free(*dst);
    *dst = NULL;
    SUCCEED_OR_RETURN(pspio_projector_alloc(dst, np));
  }

  SUCCEED_OR_RETURN( pspio_meshfunc_copy(&(*dst)->proj, src->proj) );
  SUCCEED_OR_RETURN( pspio_qn_copy(&(*dst)->qn, src->qn) );
  (*dst)->energy = src->energy;

  return PSPIO_SUCCESS;
}

void pspio_projector_free(pspio_projector_t *projector)
{
  if (projector != NULL) {
    pspio_meshfunc_free(projector->proj);
    pspio_qn_free(projector->qn);
    free(projector);
  }
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

double pspio_projector_get_energy(const pspio_projector_t *projector)
{
  assert(projector != NULL);

  return projector->energy;
}

const pspio_qn_t *pspio_projector_get_qn(const pspio_projector_t *projector)
{
  assert(projector != NULL);

  return projector->qn;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_projector_cmp(const pspio_projector_t *projector1, const
                        pspio_projector_t *projector2)
{
  assert(projector1 != NULL);
  assert(projector2 != NULL);

  if ((pspio_qn_cmp(projector1->qn, projector2->qn) == PSPIO_DIFF) ||
      (pspio_meshfunc_cmp(projector1->proj, projector2->proj) == PSPIO_DIFF) ||
      (fabs(projector1->energy - projector2->energy) > 1e-10)) {
    return PSPIO_DIFF;
  } else {
    return PSPIO_EQUAL;
  }
}

double pspio_projector_eval(const pspio_projector_t *projector, double r)
{
  assert(projector != NULL);

  return pspio_meshfunc_eval(projector->proj, r);
}

double pspio_projector_eval_deriv(const pspio_projector_t *projector, double r)
{
  assert(projector != NULL);

  return pspio_meshfunc_eval_deriv(projector->proj, r);
}

double pspio_projector_eval_deriv2(const pspio_projector_t *projector, double r)
{
  assert(projector != NULL);

  return pspio_meshfunc_eval_deriv2(projector->proj, r);
}

int * pspio_projectors_per_l(pspio_projector_t ** const projectors, int nproj)
{
  int i, l;
  int * ret;

  assert(projectors != NULL);
  assert(nproj > 0);

  ret = (unsigned int *) malloc (6*sizeof(unsigned int));
  for (i=0; i<6; i++) {
    ret[i] = 0;
  }

  for (i=0;i<nproj;i++) {
    l = projectors[i]->qn->l;
    FULFILL_OR_EXIT( l < 6, PSPIO_ENOMEM );
    ret[l] += 1;
  }

  return ret;
}
