# These are real modules. If you need components from there you
# need to first add find_package(XXX) in your CMakeLists.txt

# General
sofa_add_module(SofaGeneralVisual SofaGeneralVisual ON)
sofa_add_module(SofaGraphComponent SofaGraphComponent ON)
sofa_add_module(SofaGeneralMeshCollision SofaGeneralMeshCollision ON)
sofa_add_module(SofaBoundaryCondition SofaBoundaryCondition ON)
sofa_add_module(SofaGeneralAnimationLoop SofaGeneralAnimationLoop ON)
sofa_add_module(SofaGeneralDeformable SofaGeneralDeformable ON)
sofa_add_module(SofaGeneralEngine SofaGeneralEngine ON)
sofa_add_module(SofaGeneralExplicitOdeSolver SofaGeneralExplicitOdeSolver ON)
sofa_add_module(SofaGeneralImplicitOdeSolver SofaGeneralImplicitOdeSolver ON)
sofa_add_module(SofaGeneralLinearSolver SofaGeneralLinearSolver ON)
sofa_add_module(SofaGeneralRigid SofaGeneralRigid ON)
sofa_add_module(SofaGeneralObjectInteraction SofaGeneralObjectInteraction ON)
sofa_add_module(SofaGeneralSimpleFem SofaGeneralSimpleFem ON)
sofa_add_module(SofaGeneralTopology SofaGeneralTopology ON)
sofa_add_module(SofaTopologyMapping SofaTopologyMapping ON)
sofa_add_module(SofaUserInteraction SofaUserInteraction ON)
sofa_add_module(SofaConstraint SofaConstraint ON)
sofa_add_module(SofaGeneralLoader SofaGeneralLoader ON)
sofa_add_module(SofaExporter SofaExporter ON)

sofa_add_module(SofaSparseSolver SofaSparseSolver ON)      #
sofa_add_module(SofaPreconditioner SofaPreconditioner ON)  # depends on SofaSparseSolver
sofa_add_module(SofaHaptics SofaHaptics ON)
sofa_add_module(SofaValidation SofaValidation ON)

find_package(Newmat QUIET)
sofa_add_module(SofaDenseSolver SofaDenseSolver ON
    WHEN_TO_SHOW "Newmat_FOUND"
    VALUE_IF_HIDDEN OFF)

sofa_add_module(SofaNonUniformFem SofaNonUniformFem ON)

find_package(Sofa.GL QUIET)
sofa_add_module(SofaOpenglVisual SofaOpenglVisual ON
    WHEN_TO_SHOW "Sofa.GL_FOUND"
    VALUE_IF_HIDDEN OFF)

# Misc
sofa_add_module(SofaMiscTopology SofaMiscTopology ON)
sofa_add_module(SofaMiscExtra SofaMiscExtra ON)
sofa_add_module(SofaMiscForceField SofaMiscForceField ON)
sofa_add_module(SofaMiscEngine SofaMiscEngine ON)
sofa_add_module(SofaMiscSolver SofaMiscSolver ON)
sofa_add_module(SofaMiscFem SofaMiscFem ON)
sofa_add_module(SofaMiscMapping SofaMiscMapping ON)
