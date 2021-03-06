/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef RESURRECTOR_H
#define RESURRECTOR_H

#include "Moose.h"
#include "MaterialPropertyIO.h"
#include "RestartableDataIO.h"

#include <string>
#include <list>

class FEProblem;

/**
 * Class for doing restart.
 *
 * It takes care of writing and reading the restart files.
 */
class Resurrector
{
public:
  Resurrector(FEProblem & fe_problem);
  virtual ~Resurrector();

  /**
   * Set the file base name from which we will restart
   * @param file_base The file base name of a restart file
   */
  void setRestartFile(const std::string & file_base);

  /**
   * Perform a restart from a file
   */
  void restartFromFile();

  void restartStatefulMaterialProps();

  void restartRestartableData();

protected:

  /// Reference to a FEProblem being restarted
  FEProblem & _fe_problem;

  /// name of the file that we restart from
  std::string _restart_file_base;

  /// Stateful material property output
  MaterialPropertyIO _mat;

  /// Restartable Data
  RestartableDataIO _restartable;

  static const std::string MAT_PROP_EXT;
  static const std::string RESTARTABLE_DATA_EXT;
};

#endif /* RESURRECTOR_H */
