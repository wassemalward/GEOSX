/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @file AggregationSinglePhaseFlow.hpp
 */

#ifndef SRC_COMPONENTS_CORE_SRC_PHYSICSSOLVERS_AGGREGATIONSINGLEPHASEFLOW_HPP_
#define SRC_COMPONENTS_CORE_SRC_PHYSICSSOLVERS_AGGREGATIONSINGLEPHASEFLOW_HPP_

#include "physicsSolvers/FiniteVolume/SinglePhaseFlow.hpp"
#include "finiteVolume/FluxApproximationBase.hpp"

class Epetra_FECrsGraph;

namespace geosx
{

namespace dataRepository
{
class ManagedGroup;
}
class FieldSpecificationBase;
class FiniteElementBase;
class DomainPartition;

/**
 * @class AggregationSinglePhaseFlow
 *
 * class to perform a single phase finite volume solve.
 */
class AggregationSinglePhaseFlow : public SinglePhaseFlow
{
public:
  /**
   * @brief main constructor for ManagedGroup Objects
   * @param name the name of this instantiation of ManagedGroup in the repository
   * @param parent the parent group of this instantiation of ManagedGroup
   */
  AggregationSinglePhaseFlow( const std::string& name,
                   ManagedGroup * const parent );


  /// deleted default constructor
  AggregationSinglePhaseFlow() = delete;

  /// deleted copy constructor
  AggregationSinglePhaseFlow( AggregationSinglePhaseFlow const & ) = delete;

  /// default move constructor
  AggregationSinglePhaseFlow( AggregationSinglePhaseFlow && ) = default;

  /// deleted assignment operator
  AggregationSinglePhaseFlow & operator=( AggregationSinglePhaseFlow const & ) = delete;

  /// deleted move operator
  AggregationSinglePhaseFlow & operator=( AggregationSinglePhaseFlow && ) = delete;

  /**
   * @brief default destructor
   */
  virtual ~AggregationSinglePhaseFlow() override = default;

  /**
   * @brief name of the node manager in the object catalog
   * @return string that contains the catalog name to generate a new NodeManager object through the object catalog.
   */
  static string CatalogName() { return "AggregationSinglePhaseFlow"; }

  virtual void InitializePreSubGroups(ManagedGroup * const rootGroup) override;

  virtual void RegisterDataOnMesh(ManagedGroup * const MeshBodies) override;

  virtual void Execute( real64 const & time_n,
                        real64 const & dt,
                        integer const cycleNumber,
                        integer const eventCounter,
                        real64 const & eventProgress,
                        dataRepository::ManagedGroup * domain ) override;

  virtual real64 SolverStep( real64 const& time_n,
                             real64 const& dt,
                             integer const cycleNumber,
                             DomainPartition * domain ) override;

  /**
   * @defgroup Solver Interface Functions
   *
   * These functions provide the primary interface that is required for derived classes
   */
  /**@{*/

  virtual void ImplicitStepSetup( real64 const& time_n,
                                  real64 const& dt,
                                  DomainPartition * const domain,
                                  systemSolverInterface::EpetraBlockSystem * const blockSystem ) override;


  virtual void AssembleSystem( DomainPartition * const domain,
                               systemSolverInterface::EpetraBlockSystem * const blockSystem,
                               real64 const time_n,
                               real64 const dt ) override;

  virtual void ApplyBoundaryConditions( DomainPartition * const domain,
                                        systemSolverInterface::EpetraBlockSystem * const blockSystem,
                                        real64 const time_n,
                                        real64 const dt ) override;

  virtual real64
  CalculateResidualNorm(systemSolverInterface::EpetraBlockSystem const *const blockSystem,
                        DomainPartition *const domain) override;

  virtual void SolveSystem( systemSolverInterface::EpetraBlockSystem * const blockSystem,
                            SystemSolverParameters const * const params ) override;

  virtual void
  ApplySystemSolution( systemSolverInterface::EpetraBlockSystem const * const blockSystem,
                       real64 const scalingFactor,
                       DomainPartition * const domain ) override;

  virtual void ResetStateToBeginningOfStep( DomainPartition * const domain ) override;

  virtual  void ImplicitStepComplete( real64 const & time,
                                      real64 const & dt,
                                      DomainPartition * const domain ) override;

  /**
   * @brief assembles the accumulation terms for all cells
   * @param domain the physical domain object
   * @param blockSystem the entire block system
   * @param time_n previous time value
   * @param dt time step
   */
  template< bool ISPORO >
  void AssembleAccumulationTerms( DomainPartition const * const domain,
                                  Epetra_FECrsMatrix * const jacobian,
                                  Epetra_FEVector * const residual,
                                  real64 const time_n,
                                  real64 const dt );

  /**
   * @brief assembles the flux terms for all cells
   * @param domain the physical domain object
   * @param blockSystem the entire block system
   * @param time_n previous time value
   * @param dt time step
   */
  void AssembleFluxTerms( DomainPartition const * const domain,
                          Epetra_FECrsMatrix * const jacobian,
                          Epetra_FEVector * const residual,
                          real64 const time_n,
                          real64 const dt );

  /**@}*/


  struct viewKeyStruct : FlowSolverBase::viewKeyStruct
  {
    // dof numbering
    static constexpr auto blockLocalDofNumberString = "blockLocalDofNumber_AggregationSinglePhaseFlow" ;

    // primary solution field
    static constexpr auto pressureString      = "pressure";
    static constexpr auto deltaPressureString = "deltaPressure";
    static constexpr auto facePressureString  = "facePressure";

    static constexpr auto deltaVolumeString = "deltaVolume";

    // these are used to store last converged time step values
    static constexpr auto densityString   = "density";
    static constexpr auto viscosityString = "viscosity";
    static constexpr auto porosityString  = "porosity";
    static constexpr auto coarsePorosityString  = "coarsePorosity";
    static constexpr auto oldPorosityString  = "oldPorosity";

    // these are used to visualize the elementary pressure fields generated
    static constexpr auto elementaryPressureString1      = "elementaryPressure1";
    static constexpr auto elementaryPressureString2      = "elementaryPressure2";
    static constexpr auto elementaryPressureString3      = "elementaryPressure3";

    // For the aggregation
    static constexpr auto initialBCsString = "initialBCs";
    

    using ViewKey = dataRepository::ViewKey;

    // dof numbering
    ViewKey blockLocalDofNumber = { blockLocalDofNumberString };

    // primary solution field
    ViewKey pressure      = { pressureString };
    ViewKey deltaPressure = { deltaPressureString };
    ViewKey facePressure  = { facePressureString };

    ViewKey deltaVolume   = { deltaVolumeString };

    // these are used to store last converged time step values
    ViewKey density      = { densityString };
    ViewKey viscosity    = { viscosityString };
    ViewKey porosity     = { porosityString };
    ViewKey oldPorosity  = { oldPorosityString };

  } viewKeysAggregationSinglePhaseFlow;

  viewKeyStruct & viewKeys() { return viewKeysAggregationSinglePhaseFlow; }
  viewKeyStruct const & viewKeys() const { return viewKeysAggregationSinglePhaseFlow; }

  struct groupKeyStruct : SolverBase::groupKeyStruct
  {
  } groupKeysAggregationSinglePhaseFlow;

  groupKeyStruct & groupKeys() { return groupKeysAggregationSinglePhaseFlow; }
  groupKeyStruct const & groupKeys() const { return groupKeysAggregationSinglePhaseFlow; }

protected:
  virtual void InitializePostInitialConditions_PreSubGroups( dataRepository::ManagedGroup * const rootGroup ) override;

private:

  void SetupSystem ( DomainPartition * const domain,
                     systemSolverInterface::EpetraBlockSystem * const blockSystem );

  /**
   * @brief set the sparsity pattern for the linear system
   * @param domain the domain partition
   * @param sparsity the sparsity pattern matrix
   */
  void SetSparsityPattern( DomainPartition const * const domain,
                           Epetra_FECrsGraph * const sparsity );

  /**
   * @brief sets the dof indices for this solver
   * @param meshLevel the mesh object (single level only)
   * @param numLocalRows the number of local rows on this partition
   * @param numGlobalRows the number of global rows in the problem
   * @param localIndices unused TODO delete
   * @param offset the DOF offset for this solver in the case of a non-block system
   *
   * This function sets the number of global rows, and sets the dof numbers for
   * this solver. dof numbers are referred to trilinosIndices currently.
   */
  void SetNumRowsAndTrilinosIndices( MeshLevel * const meshLevel,
                                     localIndex & numLocalRows,
                                     globalIndex & numGlobalRows,
                                     localIndex offset );

  /**
   * @brief Setup stored views into domain data for the current step
   */
  void ResetViews( DomainPartition * const domain ) override;

  /**
   * @brief Function to perform the Application of Dirichlet type BC's
   * @param object the target ObjectManager for the application of the BC.
   * @param time_n current time
   * @param blockSystem the entire block system
   */
  void ApplyDirichletBC_implicit( DomainPartition * object,
                                  real64 const time_n, real64 const dt,
                                  systemSolverInterface::EpetraBlockSystem * const blockSystem );

  /**
   * @brief Function to perform the application of Dirichlet BCs on faces
   * @param domain the domain
   * @param time_n current time
   * @param blockSystem the entire block system
   */
  void ApplyFaceDirichletBC_implicit( DomainPartition * domain,
                                      real64 const time_n, real64 const dt,
                                      systemSolverInterface::EpetraBlockSystem * const blockSystem );

  /**
   * @brief Function to update all constitutive models
   * @param domain the domain
   */
  void UpdateConstitutiveModels( DomainPartition * const domain );

  /// views into primary variable fields

  ElementRegionManager::ElementViewAccessor<arrayView1d<globalIndex>> m_dofNumber; // TODO will move to DofManager

  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_pressure;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_deltaPressure;

  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_deltaVolume;

  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_porosity;

  /// views into backup fields

  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_porosityOld;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_densityOld;

  /// views into material fields

  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_pvMult;
  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_dPvMult_dPres;

  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_density;
  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_dDens_dPres;

  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_viscosity;
  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_dVisc_dPres;

  /// coupling with mechanics

  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_totalMeanStressOld;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_totalMeanStress;

  ElementRegionManager::MaterialViewAccessor<arrayView2d<real64>> m_bulkModulus;
  ElementRegionManager::MaterialViewAccessor<real64>              m_biotCoefficient;

  /// For the aggregation
  string_array m_initialBCs;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_pressure1;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_pressure2;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_pressure3;
  ElementRegionManager::ElementViewAccessor<arrayView1d<real64>> m_coarsePorosity;
  StencilCollection< CellDescriptor, real64 > m_coarseStencil;

};


} /* namespace geosx */

#endif //SRC_COMPONENTS_CORE_SRC_PHYSICSSOLVERS_SINGLEPHASEFLOW_HPP_
