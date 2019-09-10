/* Copyright (C) 2018-2019 Damien Caliste <damien.caliste@cea.fr>
 *
 * This file is part of Libpspio. It is a C parser for CASTEP / ONETEP
 * recpot norm conserving pseudopotential files.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "recpot.h"
#include "pspio_common.h"
#include "pspio_error.h"
#include "util.h"

#define NQMAX 5000
#define NLMAX 5
struct RecPot {
  int version;
  char comment[PSPIO_STRLEN_DESCRIPTION];
  unsigned int nq;
  double q2Vlq[NQMAX], q[NQMAX];
  double Vlq0;
  unsigned int nl;
  unsigned int ls[NLMAX];
  double etas[NLMAX];
  double projs[NLMAX][NQMAX];
};
/* Hard-coded values in CASTEP. */
#define bohrad 0.529177249
#define ryd   13.6056981
#define dq     0.05
/* Hard coded for our real space representation. */
#define NP     2000
#define LOG_A  0.008
#define LOG_B  0.00001

static int recpot_read(FILE *fp, struct RecPot *recpot)
{
  char line[PSPIO_STRLEN_LINE];
  size_t idx, l;
  size_t descEnd = PSPIO_STRLEN_DESCRIPTION;

  idx = NQMAX;
  recpot->nq = NQMAX;
  recpot->version = 0;
  recpot->comment[0] = '\0';
  while (fgets(line, PSPIO_STRLEN_LINE, fp)) {
    if (!strncmp(line, "START COMMENT", 13)) {
      descEnd = 0;
      recpot->version = -1;
    } else if (!strncmp(line, "END COMMENT", 11)) {
      recpot->version = 0;
      descEnd = PSPIO_STRLEN_DESCRIPTION;
    } else if (descEnd < PSPIO_STRLEN_DESCRIPTION - 1) {
      size_t len = strlen(line);
      size_t n = min(PSPIO_STRLEN_DESCRIPTION - descEnd - 1, len);
      memcpy(recpot->comment + descEnd, line, n);
      descEnd += n;
      recpot->comment[descEnd] = '\0';
    } else if (!recpot->version) {
      int a, b;
      FULFILL_OR_RETURN( sscanf(line, "%d %d", &a, &b) == 2, PSPIO_EFILE_FORMAT );
      recpot->version = b;
      recpot->nq = 0;
    } else if (!recpot->nq) {
      char *end;
      double cutoff = strtod(line, &end);
      FULFILL_OR_RETURN( end != line, PSPIO_EFILE_FORMAT );
      recpot->nq = cutoff / dq + 1;
      if (recpot->nq >= NQMAX)
        recpot->nq = NQMAX;
      idx = 0;
    } else if (idx < recpot->nq) {
      const double fact = 1. / ryd / bohrad / bohrad / bohrad / 4. / M_PI;
      char *at = line;
      do {
        char *end;
        double v = strtod(at, &end);
        if (end != at) {
          recpot->q[idx] = dq * idx * bohrad;
          recpot->q2Vlq[idx] = recpot->q[idx] * recpot->q[idx] * v * fact;
          if (!idx)
            recpot->Vlq0 = v;
          idx += 1;
          at = end;
        } else {
          at = NULL;
        }
      } while (at && idx < recpot->nq);
      if (idx == recpot->nq)
        break;
    }
  }
  /* Correct q2Vlq at q = 0. */
  recpot->q2Vlq[0] = 2. * recpot->q2Vlq[1] - recpot->q2Vlq[2];

  idx = NQMAX;
  l = NLMAX;
  recpot->nl = 0;
  recpot->etas[0] = 0.;
  recpot->etas[1] = 0.;
  recpot->etas[2] = 0.;
  recpot->etas[3] = 0.;
  recpot->etas[4] = 0.;
  while (fgets(line, PSPIO_STRLEN_LINE, fp)) {
    if (l == NLMAX) {
      char *end;
      l = strtol(line, &end, 10);
      FULFILL_OR_RETURN( end != line, PSPIO_EFILE_FORMAT );
      if (l == 1000)
        break;
      FULFILL_OR_RETURN( l >= 0 && l < NLMAX, PSPIO_EFILE_FORMAT );
      recpot->ls[recpot->nl] = l;
    } else if (l >= 0 && l < NLMAX && !recpot->etas[recpot->nl]) {
      char *end;
      recpot->etas[recpot->nl] = strtod(line, &end) / ryd;
      FULFILL_OR_RETURN( end != line, PSPIO_EFILE_FORMAT );
      idx = 0;
    } else if (l >= 0 && l < NLMAX && idx < recpot->nq) {
      const double fact = 1. / ryd / sqrt(bohrad * bohrad * bohrad);
      char *at = line;
      do {
        char *end;
        double v = strtod(at, &end);
        if (end != at) {
          recpot->projs[recpot->nl][idx] = recpot->q[idx] * recpot->q[idx] * v * fact;
          idx += 1;
          at = end;
        } else {
          at = NULL;
        }
      } while (at && idx < recpot->nq);
      if (idx == recpot->nq) {
        recpot->nl += 1;
        l = NLMAX;
        idx = NQMAX;
      }
    }
  }

  return PSPIO_SUCCESS;
}

static double _trapz(const double *arr, const double *qs, size_t n)
{
  size_t i;
  double sum;

  sum = 0.5 * arr[0];
  for (i = 1; i < n - 1; i++)
    sum += arr[i];
  sum += 0.5 * arr[i];
  return sum * 2. / M_PI * dq * bohrad;
}
static double _trapz_r(const double *arr, const double *rs, double rcut, size_t n)
{
  size_t i;
  double sum, z;

  z = round(arr[n - 1] * rs[n - 1]);
  sum = 0.5 * (arr[0] * rs[0] - z) * rs[0] * (rs[1] - rs[0]);
  for (i = 1; i < n - 1 && rs[i] < rcut; i++)
    sum += 0.5 * (arr[i] * rs[i] - z) * rs[i] * (rs[i+1] - rs[i - 1]);
  sum += 0.5 * (arr[i] * rs[i] - z) * rs[i] * (rs[i] - rs[i - 1]);
  return sum * 4. * M_PI * ryd * bohrad * bohrad * bohrad;
}
static double _trapz_q(const double *arr, const double *rs, double rcut, size_t n)
{
  size_t i;
  double sum;

  sum = 0.5 * arr[0] * rs[0] * rs[0] * (rs[1] - rs[0]);
  for (i = 1; i < n - 1 && rs[i] < rcut; i++)
    sum += 0.5 * arr[i] * rs[i] * rs[i] * (rs[i+1] - rs[i - 1]);
  sum += 0.5 * arr[i] * rs[i] * rs[i] * (rs[i] - rs[i - 1]);
  return sum * ryd * sqrt(bohrad * bohrad * bohrad);
}
static double _trapz_qr(const double *arr, const double *rs, double q, double rcut, size_t n)
{
  size_t i;
  double sum, z;

  z = round(arr[n - 1] * rs[n - 1]);
  sum = 0.5 * (arr[0] * rs[0] - z) * sin(rs[0] * q) / q * (rs[1] - rs[0]);
  for (i = 1; i < n - 1 && rs[i] < rcut; i++)
    sum += 0.5 * (arr[i] * rs[i] - z) * sin(rs[i] * q) / q * (rs[i+1] - rs[i - 1]);
  sum += 0.5 * (arr[i] * rs[i] - z) * sin(rs[i] * q) / q * (rs[i] - rs[i - 1]);
  return (sum + z / q / q) * 4. * M_PI * ryd * bohrad * bohrad * bohrad;
}
static double _trapz_int(const double *arr, const double *rs, double rcut, size_t n)
{
  size_t i;
  double sum;

  sum = 0.5 * arr[0] * arr[0] * rs[0] * rs[0] * (rs[1] - rs[0]);
  for (i = 1; i < n - 1 && rs[i] < rcut; i++)
    sum += 0.5 * arr[i] * arr[i] * rs[i] * rs[i] * (rs[i+1] - rs[i - 1]);
  sum += 0.5 * arr[i] * arr[i] * rs[i] * rs[i] * (rs[i] - rs[i - 1]);
  return sum;
}

static double _trapz_jl(unsigned int l, const double *arr, const double *qs, double r, size_t n)
{
  size_t i;
  double sum;

  if (l == 0) {
    sum = 0.5 * arr[0];
    for (i = 1; i < n - 1; i++)
      sum += arr[i] * sin(r * qs[i]) / r / qs[i];
    sum += 0.5 * arr[i] * sin(r * qs[i]) / r / qs[i];
    return sum * 2. / M_PI * dq * bohrad;
  } else if (l == 1) {
    sum = 0.;
    for (i = 1; i < n - 1; i++)
      sum += arr[i] * ((sin(r * qs[i]) / r / qs[i] - cos(r * qs[i])) / r / qs[i]);
    sum += 0.5 * arr[i] * (sin(r * qs[i]) / r / qs[i] / r / qs[i] - cos(r * qs[i]) / r / qs[i]);
    return sum * 2. / M_PI * dq * bohrad;
  } else if (l == 2) {
    sum = 0.;
    for (i = 1; i < n - 1; i++)
      sum += arr[i] * (sin(r * qs[i]) / r / qs[i] * (3. / r / qs[i] / r / qs[i] - 1.) - 3. * cos(r * qs[i]) / r / qs[i] / r / qs[i]);
    sum += 0.5 * arr[i] * (sin(r * qs[i]) / r / qs[i] * (3. / r / qs[i] / r / qs[i] - 1.) - 3. * cos(r * qs[i]) / r / qs[i] / r / qs[i]);
    return sum * 2. / M_PI * dq * bohrad;
  }
  return -1.;
}

static double _trapz_jlq(unsigned int l, const double *arr, const double *rs, const double q, const double rcut, size_t n)
{
  size_t i;
  double sum;

  if (l == 0) {
    sum = 0.5 * arr[0] * rs[0] * rs[0] * (rs[1] - rs[0]);
    for (i = 1; i < n - 1 && rs[i] < rcut; i++)
      sum += 0.5 * arr[i] * sin(q * rs[i]) / q * rs[i] * (rs[i+1] - rs[i - 1]);
    sum += 0.5 * arr[i] * sin(q * rs[i]) / q * rs[i] * (rs[i] - rs[i - 1]);
    return sum * ryd * sqrt(bohrad * bohrad * bohrad);
  } else if (l == 1) {
    sum = 0.;
    for (i = 1; i < n - 1 && rs[i] < rcut; i++)
      sum += 0.5 * arr[i] * ((sin(q * rs[i]) / q / rs[i] - cos(q * rs[i])) / q * rs[i]) * (rs[i+1] - rs[i - 1]);
    sum += 0.5 * arr[i] * ((sin(q * rs[i]) / q / rs[i] - cos(q * rs[i])) / q * rs[i]) * (rs[i] - rs[i - 1]);
    return sum * ryd * sqrt(bohrad * bohrad * bohrad);
  } else if (l == 2) {
    sum = 0.;
    for (i = 1; i < n - 1 && rs[i] < rcut; i++)
      sum += 0.5 * arr[i] * (sin(q * rs[i]) / q * rs[i] * (3. / q / rs[i] / q / rs[i] - 1.) - 3. * cos(q * rs[i]) / q / q) * (rs[i+1] - rs[i - 1]);
    sum += 0.5 * arr[i] * (sin(q * rs[i]) / q * rs[i] * (3. / q / rs[i] / q / rs[i] - 1.) - 3. * cos(q * rs[i]) / q / q) * (rs[i] - rs[i - 1]);
    return sum * ryd * sqrt(bohrad * bohrad * bohrad);
  }
  return -1.;
}

static int _to_vr(pspio_potential_t *pot, const double *qs, const double *vq, size_t nq)
{
  const double *rs;

  pspio_mesh_init_from_parameters(pot->v->mesh,
                                  PSPIO_MESH_LOG2, LOG_A, LOG_B);
  rs = pspio_mesh_get_r(pot->v->mesh);

  pot->v->f[0] = !pot->qn->l ? 0.5 * _trapz(vq, qs, nq) : 0.;
  for (int ir = 1; ir < pot->v->mesh->np; ir ++)
    pot->v->f[ir] = 0.5 * _trapz_jl(pot->qn->l, vq, qs, rs[ir], nq);
  SUCCEED_OR_RETURN( pspio_interp_init(pot->v->f_interp,
                                       pot->v->mesh, pot->v->f) );
  for (int ir = 1; ir < pot->v->mesh->np; ir ++)
    pot->v->fp[ir] = pspio_interp_eval_deriv(pot->v->f_interp, rs[ir]);
  SUCCEED_OR_RETURN( pspio_interp_init(pot->v->fp_interp,
                                       pot->v->mesh, pot->v->fp) );
  for (int ir = 1; ir < pot->v->mesh->np; ir ++)
    pot->v->fpp[ir] = pspio_interp_eval_deriv2(pot->v->f_interp, rs[ir]);
  SUCCEED_OR_RETURN( pspio_interp_init(pot->v->fpp_interp,
                                       pot->v->mesh, pot->v->fpp) );

  return PSPIO_SUCCESS;
}

static int recpot_to_vlocr(struct RecPot *recpot, pspio_pspdata_t *pspdata)
{
  const double *rs;

  SUCCEED_OR_RETURN( pspio_potential_alloc(&pspdata->vlocal, NP) );

  SUCCEED_OR_RETURN( pspio_qn_init(pspdata->vlocal->qn, 0, 0, 0.) );

  SUCCEED_OR_RETURN( _to_vr(pspdata->vlocal, recpot->q, recpot->q2Vlq, recpot->nq) );

  rs = pspio_mesh_get_r(pspdata->vlocal->v->mesh);
  /* fprintf(stderr, "%20.12f %20.12f\n", _trapz_r(pspdata->vlocal->v->f, rs, 2., pspdata->vlocal->v->mesh->np), recpot->Vlq0); */

  return PSPIO_SUCCESS;
}

static int _to_projs(pspio_projector_t *p, const double *qs, const double *vq, size_t nq)
{
  const double *rs;

  pspio_mesh_init_from_parameters(p->proj->mesh,
                                  PSPIO_MESH_LOG2, LOG_A, LOG_B);
  rs = pspio_mesh_get_r(p->proj->mesh);

  p->proj->f[0] = !p->qn->l ? 0.5 * _trapz(vq, qs, nq) : 0.;
  for (int ir = 1; ir < p->proj->mesh->np; ir ++)
    p->proj->f[ir] = 0.5 * _trapz_jl(p->qn->l, vq, qs, rs[ir], nq);
  SUCCEED_OR_RETURN( pspio_interp_init(p->proj->f_interp,
                                       p->proj->mesh, p->proj->f) );
  for (int ir = 1; ir < p->proj->mesh->np; ir ++)
    p->proj->fp[ir] = pspio_interp_eval_deriv(p->proj->f_interp, rs[ir]);
  SUCCEED_OR_RETURN( pspio_interp_init(p->proj->fp_interp,
                                       p->proj->mesh, p->proj->fp) );
  for (int ir = 1; ir < p->proj->mesh->np; ir ++)
    p->proj->fpp[ir] = pspio_interp_eval_deriv2(p->proj->f_interp, rs[ir]);
  SUCCEED_OR_RETURN( pspio_interp_init(p->proj->fpp_interp,
                                       p->proj->mesh, p->proj->fpp) );

  return PSPIO_SUCCESS;
}

static int recpot_to_projs(struct RecPot *recpot, pspio_pspdata_t *pspdata)
{
  pspio_pspdata_set_n_projectors(pspdata, recpot->nl);

  for (int il = 0; il  < recpot->nl; il++) {
    pspio_projector_alloc(pspdata->projectors + il, NP);
    pspio_qn_init(pspdata->projectors[il]->qn, 0, recpot->ls[il], 0.);
    SUCCEED_OR_RETURN( _to_projs(pspdata->projectors[il], recpot->q, recpot->projs[il], recpot->nq) );
    pspdata->projectors[il]->energy = 2. * recpot->etas[il];

    /* fprintf(stderr, "%20.12f %20.12f\n", _trapz_int(pspdata->projectors[il]->proj->f, pspdata->projectors[il]->proj->mesh->r, 2., pspdata->projectors[il]->proj->mesh->np), pspdata->projectors[il]->energy); */
  }
  /* for (int ir = 0; ir < pspdata->projectors[0]->proj->mesh->np; ir++) { */
  /*   fprintf(stdout, "%20.12f %20.12f %20.12f %20.12f\n", pspdata->projectors[0]->proj->mesh->r[ir], pspdata->projectors[0]->proj->f[ir], pspdata->projectors[1]->proj->f[ir], pspdata->projectors[2]->proj->f[ir]); */
  /* } */
  /* for (int iq = 0; iq < recpot->nq; iq++) { */
  /*   fprintf(stdout, "%20.12f %20.12f %20.12f %20.12f\n", recpot->q[iq], recpot->projs[0][iq], recpot->projs[1][iq], recpot->projs[2][iq]); */
  /* } */

  return PSPIO_SUCCESS;
}

int pspio_recpot_read(FILE *fp, pspio_pspdata_t *pspdata)
{
  struct RecPot recpot;
  double z;

  SUCCEED_OR_RETURN( recpot_read(fp, &recpot) );

  pspio_pspinfo_alloc(&pspdata->pspinfo);
  pspio_pspinfo_set_description(pspdata->pspinfo, recpot.comment);
  SUCCEED_OR_RETURN( recpot_to_vlocr(&recpot, pspdata) );
  SUCCEED_OR_RETURN( recpot_to_projs(&recpot, pspdata) );

  z = round(pspdata->vlocal->v->f[pspdata->vlocal->v->mesh->np - 1] * pspdata->vlocal->v->mesh->r[pspdata->vlocal->v->mesh->np - 1]);
  pspio_pspdata_set_zvalence(pspdata, - z);
  pspio_pspdata_set_nelvalence(pspdata, - z);

  return PSPIO_SUCCESS;
}

static int _write_str(FILE *fp, const char *msg)
{
  size_t len = strlen(msg);
  size_t lw = 0;

  do {
    size_t out = fwrite(msg + lw, 1, len - lw, fp);
    if (!out)
      return PSPIO_EIO;
    lw += out;
  } while (lw < len);

  return PSPIO_SUCCESS;
}

int pspio_recpot_write(FILE *fp, const pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  const int nq = 2000;
  double vlq[nq + 1];

  fp = stdout;
  if (pspdata->pspinfo && pspio_pspinfo_get_description(pspdata->pspinfo)) {
    _write_str(fp, "START COMMENT \n");
    _write_str(fp, pspio_pspinfo_get_description(pspdata->pspinfo));
    _write_str(fp, "END COMMENT \n");
  }
  snprintf(line, PSPIO_STRLEN_LINE, " %d %d \n", 3, 5);
  _write_str(fp, line);
  snprintf(line, PSPIO_STRLEN_LINE, "%18.8f\n", dq * nq);
  _write_str(fp, line);

  vlq[0] = 2. * _trapz_r(pspdata->vlocal->v->f, pspdata->vlocal->v->mesh->r, 2., pspdata->vlocal->v->mesh->np);
  for (int iq = 1; iq < nq + 1; iq ++)
    vlq[iq] = 2. * _trapz_qr(pspdata->vlocal->v->f, pspdata->vlocal->v->mesh->r, dq * iq * bohrad, 20., pspdata->vlocal->v->mesh->np);
  for (int iq = 0; iq < nq + 1; iq += 3) {
    snprintf(line, PSPIO_STRLEN_LINE, " %19.10f %19.10f %19.10f\n", vlq[iq], vlq[iq + 1], vlq[iq + 2]);
    _write_str(fp, line);
  }

  for (int il = 0; il < pspio_pspdata_get_n_projectors(pspdata); il++) {
    const pspio_projector_t *proj = pspio_pspdata_get_projector(pspdata, il);
    snprintf(line, PSPIO_STRLEN_LINE, "%d\n", pspio_qn_get_l(proj->qn));
    _write_str(fp, line);
    snprintf(line, PSPIO_STRLEN_LINE, "%20.10f\n", 0.5 * pspio_projector_get_energy(proj) * ryd);
    _write_str(fp, line);
    vlq[0] = !pspio_qn_get_l(proj->qn) ? 2. * _trapz_q(proj->proj->f, proj->proj->mesh->r, 20., proj->proj->mesh->np): 0.;
    for (int iq = 1; iq < nq + 1; iq ++)
      vlq[iq] = 2. * _trapz_jlq(pspio_qn_get_l(proj->qn), proj->proj->f, proj->proj->mesh->r, dq * iq * bohrad, 2., proj->proj->mesh->np);
    for (int iq = 0; iq < nq + 1; iq += 3) {
      snprintf(line, PSPIO_STRLEN_LINE, " %19.10f %19.10f %19.10f\n", vlq[iq], vlq[iq + 1], vlq[iq + 2]);
      _write_str(fp, line);
    }
  }

  _write_str(fp, "1000\n");

  return PSPIO_SUCCESS;
}
