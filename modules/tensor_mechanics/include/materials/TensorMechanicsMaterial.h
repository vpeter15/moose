// Original class author: A.M. Jokisaari,  O. Heinonen, M.R. Tonks

#ifndef TENSORMECHANICSMATERIAL_H
#define TENSORMECHANICSMATERIAL_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "ElasticityTensorR4.h"
#include "RotationTensor.h"

//Forward declaration
class TensorMechanicsMaterial;

template<>
InputParameters validParams<TensorMechanicsMaterial>();

/**
 * TensorMechanicsMaterial handles a fully anisotropic, single-crystal material's elastic
 * constants.  It takes all 21 independent stiffness tensor inputs, or only 9, depending on the
 * boolean input value given.  This can be extended or simplified to specify HCP, monoclinic,
 * cubic, etc as needed.
 */
class TensorMechanicsMaterial : public Material
{
public:
  TensorMechanicsMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();
  virtual void computeQpElasticityTensor();
  virtual void computeStrain();

  virtual void computeQpStrain() = 0;
  virtual void computeQpStress() = 0;

  VariableGradient & _grad_disp_x;
  VariableGradient & _grad_disp_y;
  VariableGradient & _grad_disp_z;

  VariableGradient & _grad_disp_x_old;
  VariableGradient & _grad_disp_y_old;
  VariableGradient & _grad_disp_z_old;

  /// Material property base name to allow for multiple TensorMechanicsMaterial to coexist in the same simulation
  std::string _base_name;

  MaterialProperty<RankTwoTensor> & _stress;
  MaterialProperty<RankTwoTensor> & _total_strain;
  MaterialProperty<RankTwoTensor> & _elastic_strain;

  std::string _elasticity_tensor_name;
  MaterialProperty<ElasticityTensorR4> & _elasticity_tensor;

  /// derivative of stress w.r.t. strain (_dstress_dstrain)
  MaterialProperty<ElasticityTensorR4> & _Jacobian_mult;

  RealVectorValue _Euler_angles;

  /// Individual material information
  ElasticityTensorR4 _Cijkl;

  RankTwoTensor _strain_increment;

  /// initial stress components
  std::vector<Function *> _initial_stress;
};

#endif //TENSORMECHANICSMATERIAL_H
