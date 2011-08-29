#include <gsl/gsl_spline.h>

/**
 * Wave-equations 
*/
#define SCHRODINGER 1
#define SCALAR_REL 2
#define DIRAC 3

/**
* Pseudopotential generation schemes
*/
#define BHS 1
#define HSC 2
#define HAMANN 3
#define KERKER 4
#define TM 5
#define TM2 6
#define MRPP 7
#define HGH 8
#define GTH 9
#define RRKJ 10

/**
* Main structure for pseudopotential data
*/
struct psp_data{
  double z; /**< Atomic number */
  char *symbol; /**< Atomic symbol */
  int wave_eq; /**< type of wave equation which was solved: Dirac, Scalar Relativistic, or Schroedinger */
  int ixc; /**< exchange-correlation functional id, taken from libxc conventions */
  int scheme; /**< scheme used to generate the pseudopotentials */
  mesh_t *mesh; /**< Radial mesh */
  int n_states; /**< number of electronic states */
  states_t *states[]; /**< struct with electronic states */
  int n_potentials; /**< Number of pseudopotentials */
  potential_t potentials[]; /**< struc with pseudopotentials */ 
  kb_projector_t *kb_projector; /**< Kleinman and Bylander projectors */
};


//Other imports from ape:
/**
* Mesh structure
*/
struct mesh_t{
  int type; /**< Type of mesh */
  int np; /**< Number of points in mesh */
  double a, b; /**< Mesh parameters */
  double r[]; /**< Mesh points */
};


//Other imports from ape:
/**
* Potential structure
*/
struct potential_t{
  qn_t *qn;  /**< struct with quantum numbers n l j for the potential */
  double psp[]; /**< pseudopotential, on a radial grid */
  gsl_spline *psp_spl; /**< spline type, for future interpolation before output to calling code */
};

/**
*General information about the state
*/
struct state_t{
  qn_t *qn;  /**< quantum numbers n l j for this wavefunction */
  double *occ;  /**< occupation of the electronic state */
  double *eigenval; /**< eigenvalue of electronic state*/
  char *label;  /**< string describing the electronic state - eg 2s or 4d1.5  */
  double *rc; /**< cutoff radii used for pseudopotential generation */
  /// The wavefunctions 
  int np; /**< number of points */
  double wf[]; /**< Wavefunction */
  double wfp[]; /**<Derivative of the wavefunction */
};

/** 
* The quantum numbers
*/
struct qn_t{
  int n; /**< main quantum number */
  int l; /**< angular momentum quantum number */
  double j; /**< total angular momentum quantum number */
};


struct kb_projector_t {
int l_local; /**< angular momentum channel of local potential */
  potential_t *vlocal; /**< local potential */
  int nproj; /**< number of projectors below */
  qn_t qn[];  /**< quantum numbers for present projector */
  double en[]; /**< Kleinman Bylander energies */
  double proj[][]; /**< Kleinman Bylander projectors */
  gsl_spline p_spl[]; /**< spline structures for interpolating projectors*/
};


