#  This is a benchmark test that checks constraint based frictionless
#  contact using the penalty method.  In this test a constant
#  displacement is applied in the horizontal direction to simulate
#  a small block come sliding down a larger block.
#
#  The gold file is run on one processor
#  and the benchmark case is run on a minimum of 4 processors to ensure no
#  parallel variability in the contact pressure and penetration results.
#

[Mesh]
  file = sliding_elastic_blocks_2d.e
  displacements = 'disp_x disp_y'
  patch_size = 80
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./penetration]
    order = FIRST
    family = LAGRANGE
  [../]
  [./inc_slip_x]
  [../]
  [./inc_slip_y]
  [../]
  [./accum_slip_x]
  [../]
  [./accum_slip_y]
  [../]
[]

[Functions]
  [./vertical_movement]
    type = ParsedFunction
    value = -t
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[AuxKernels]
  [./zeroslip_x]
    type = ConstantAux
    variable = inc_slip_x
    boundary = 3
    execute_on = timestep_begin
    value = 0.0
  [../]
  [./zeroslip_y]
    type = ConstantAux
    variable = inc_slip_y
    boundary = 3
    execute_on = timestep_begin
    value = 0.0
  [../]
  [./accum_slip_x]
    type = AccumulateAux
    variable = accum_slip_x
    accumulate_from_variable = inc_slip_x
    execute_on = timestep_end
  [../]
  [./accum_slip_y]
    type = AccumulateAux
    variable = accum_slip_y
    accumulate_from_variable = inc_slip_y
    execute_on = timestep_end
  [../]
  [./penetration]
    type = PenetrationAux
    variable = penetration
    boundary = 3
    paired_boundary = 2
  [../]
[]

[Postprocessors]
  [./nonlinear_its]
    type = NumNonlinearIterations
    execute_on = timestep_end
  [../]
  [./penetration]
    type = NodalVariableValue
    variable = penetration
    nodeid = 222
  [../]
  [./contact_pressure]
    type = NodalVariableValue
    variable = contact_pressure
    nodeid = 222
  [../]
[]

[BCs]
  [./left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 1
    value = 0.0
  [../]
  [./left_y]
    type = DirichletBC
    variable = disp_y
    boundary = 1
    value = 0.0
  [../]
  [./right_x]
    type = PresetBC
    variable = disp_x
    boundary = 4
    value = -0.02
  [../]
  [./right_y]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 4
    function = vertical_movement
  [../]
[]

[Materials]
  [./left]
    type = LinearIsotropicMaterial
    block = 1
    disp_y = disp_y
    disp_x = disp_x
    poissons_ratio = 0.3
    youngs_modulus = 1e6
  [../]
  [./right]
    type = LinearIsotropicMaterial
    block = 2
    disp_y = disp_y
    disp_x = disp_x
    poissons_ratio = 0.3
    youngs_modulus = 1e6
  [../]
[]

[Executioner]
  type = Transient

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -sub_pc_type -pc_asm_overlap -ksp_gmres_restart'
  petsc_options_value = 'asm     lu    20    101'


  line_search = 'none'

  l_max_its = 100
  nl_max_its = 1000
  dt = 0.1
  end_time = 15
  num_steps = 1000
  l_tol = 1e-6
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-6
  dtmin = 0.01

  [./Predictor]
    type = SimplePredictor
    scale = 1.0
  [../]
[]

[Outputs]
  file_base = frictionless_penalty_out
  interval = 10
  output_on = 'initial timestep_end'
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    perf_log = true
    max_rows = 5
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

[Contact]
  [./leftright]
    slave = 3
    disp_y = disp_y
    disp_x = disp_x
    master = 2
    model = frictionless
    penalty = 1e+7
    formulation = penalty
    system = constraint
    normal_smoothing_distance = 0.1
  [../]
[]
