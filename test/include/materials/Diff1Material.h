#ifndef DIFF1MATERIAL_H
#define DIFF1MATERIAL_H

#include "Material.h"
// libMesh
#include "libmesh/dense_matrix.h"

//Forward Declarations
class Diff1Material;

template<>
InputParameters validParams<Diff1Material>();

/**
 * Simple material with constant properties.
 */
class Diff1Material : public Material
{
public:
  Diff1Material(const std::string & name, InputParameters parameters);

protected:
  virtual void computeQpProperties();

  Real _diff;         // the value read from the input file
  MaterialProperty<Real> & _diffusivity;
  MaterialProperty<std::vector<Real> > & _vprop;

  MaterialProperty<DenseMatrix<Real> > & _matrix_mat;   // to ensure that we are able to use Matrix-valued material properties
};

#endif //DIFF1MATERIAL_H
