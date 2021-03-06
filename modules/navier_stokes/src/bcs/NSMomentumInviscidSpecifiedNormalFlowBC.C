#include "NSMomentumInviscidSpecifiedNormalFlowBC.h"

template<>
InputParameters validParams<NSMomentumInviscidSpecifiedNormalFlowBC>()
{
  InputParameters params = validParams<NSMomentumInviscidBC>();

  // Coupled variables
  params.addRequiredCoupledVar("pressure", "");

  // Required parameters
  params.addRequiredParam<Real>("rhou_udotn", "The _component'th entry of the (rho*u)(u.n) vector for this boundary");

  return params;
}




NSMomentumInviscidSpecifiedNormalFlowBC::NSMomentumInviscidSpecifiedNormalFlowBC(const std::string & name, InputParameters parameters)
    : NSMomentumInviscidBC(name, parameters),

      // Aux Variables
      _pressure(coupledValue("pressure")),

      // Required parameters
      _rhou_udotn(getParam<Real>("rhou_udotn"))
{
}




Real NSMomentumInviscidSpecifiedNormalFlowBC::computeQpResidual()
{
  return
    this->pressure_qp_residual(_pressure[_qp]) +
    this->convective_qp_residual(_rhou_udotn);
}



Real NSMomentumInviscidSpecifiedNormalFlowBC::computeQpJacobian()
{
  // There is no Jacobian for the convective term when (rho*u)(u.n) is specified,
  // so all we have left is the pressure jacobian.  The on-diagonal variable number
  // is _component+1
  return this->pressure_qp_jacobian(_component+1);
}



Real NSMomentumInviscidSpecifiedNormalFlowBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return this->pressure_qp_jacobian( this->map_var_number(jvar) );
}
